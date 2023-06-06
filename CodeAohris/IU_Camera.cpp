#include "IU_Camera.h"
#include "UC_PlayerAttackComponent.h"
#include "C_AimAssistComponent.h"
#include "HUD_Game.h"
#include "IU_HealthSystemComponent.h"
#include "M_GameMode.h"
#include "STRUCT_GameInput.h"
#include "S_PlayerSave.h"
#include "Camera/CameraShakeSourceActor.h"
#include "Camera/CameraShakeSourceComponent.h"
#include "Components/ForceFeedbackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AIU_Camera::AIU_Camera()
{
	PrimaryActorTick.bCanEverTick = true;

	render = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	RootComponent = render;
	aimAssist = CreateDefaultSubobject<UC_AimAssistComponent>("AimAssistComponent");
	AddOwnedComponent(aimAssist);

	frustumChecker = CreateDefaultSubobject<UC_FrustumChecker>("FrustumChecker");
	AddOwnedComponent(frustumChecker);
}

void AIU_Camera::BeginPlay()
{
	Super::BeginPlay();
	SetTickGroup(TG_PostPhysics);
	currentDistanceFromTarget = distanceFromTarget;
	const FGameInput _gameInput;
	GetWorld()->GetFirstPlayerController()->InputComponent->BindAxis(_gameInput.HorizontalMovementAxis.AxisName, this, &AIU_Camera::MoveYawWithMovement).bConsumeInput = false;
	InitShake();
}

void AIU_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isActive)return;
	CheckGroundCollisions();
	CheckCeilingCollisions();
	UpdateLocation(DeltaTime);
	UpdateYawPitchViaAimAssist();
	if (!movingCamera)
	{
		if (passiveReactivationTime > passiveReactivationDelay)
		{
			passiveBehaviour = true;
			passiveReactivationTime = 0.0f;
			return;
		}
		passiveReactivationTime += DeltaTime;
	}
	else
	{
		passiveBehaviour = false;
		passiveReactivationTime = 0.0f;
	}
}

void AIU_Camera::UpdateLocation(const float& _deltaTime)
{
	
}

void AIU_Camera::ResetCamera(const FVector& _location, const float& _rotationZ)
{
	SetActorLocation(_location);
	pitch = 0.0f;
	if (_rotationZ < 0)
	{
		yaw = _rotationZ + 180.0f;
	}

	else
	{
		yaw = _rotationZ - 180.0f;
	}
}

void AIU_Camera::UpdateCameraFromSave(const US_PlayerSave* _playerSave)
{
	if (!_playerSave) return;

	InitializeCameraConfig(_playerSave->GetCameraConfig());
	InitializeGraphicConfig(_playerSave->GetGraphicConfig());
	bCanSpawnVibroManette = _playerSave->GetPlayerConfig().GetVibrationEnable();
}

void AIU_Camera::InitializeCameraConfig(const FCameraConfig& _config)
{
	invertXAxis = _config.GetInvertAxisX();
	invertYAxis = _config.GetInvertAxisY();
	mouseSensitivitySpeed = _config.GetMouseSensitivity();
	sensitivityXSpeed = _config.GetGamepadSensitivityX();
	sensitivityYSpeed = _config.GetGamepadSensitivityY();
	aimRotateFactor = _config.GetAimSensitivity();
}

void AIU_Camera::InitializeGraphicConfig(const FGraphicsConfig& _config)
{
	render->PostProcessSettings.bOverride_MotionBlurAmount = _config.GetMotionBlurEnable();
	render->PostProcessSettings.MotionBlurAmount = _config.GetMotionBlurValue();
}

void AIU_Camera::InitShake()
{
	if (cameraShakeToSpawn.Get())
	{
		cameraShaker = GetWorld()->SpawnActor<ACameraShakeSourceActor>(cameraShakeToSpawn, GetActorLocation(), FRotator::ZeroRotator);
		if (cameraShaker)
		{
			cameraShaker->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
			{
				if (const AIU_Player* _player = _gameMode->GetPlayer())
				{
					if (UIU_HealthSystemComponent* _health = _player->HealthComponent())
						Event_AddUnique(_health->OnTakingHit(), AIU_Camera::StartShake);
					if (AHUD_Game* _hudGame = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Game>())
					{
						Event_AddUnique(_hudGame->OnGamePause(), AIU_Camera::DestroyVibro);
					}
					else
						LOG("AIU_Camera::InitShake -> Invalid HUD");
				}
				else
					LOG("AIU_Camera::InitShake -> Invalid player");
			}
			else
				LOG("AIU_Camera::InitShake -> Invalid gamemode");
		}
		else
			LOG("AIU_Camera::InitShake -> Invalid camera shaker");
	}
}

void AIU_Camera::StartShake(bool _hasTakeHit)
{
	if (_hasTakeHit)
	{
		if (cameraShaker)
			cameraShaker->GetCameraShakeSourceComponent()->Start();
		else
			LOG("AIU_Camera::StartShake -> Invalid camera shaker");
		
		if (vibroManetteToSpawn && bCanSpawnVibroManette)
		{
			vibroManetteComponent = UGameplayStatics::SpawnForceFeedbackAttached(vibroManetteToSpawn, RootComponent);
		}
		else
			LOG("AIU_Camera::InitShake -> Invalid vibro manette to spawn");
	}
}

void AIU_Camera::DestroyVibro()
{
	if (IsValid(cameraShaker))
	{
		cameraShaker->GetCameraShakeSourceComponent()->StopAllCameraShakes();
	}
	if (IsValid(vibroManetteComponent))
	{
		vibroManetteComponent->Stop();
	}
}

#pragma region Yaw

void AIU_Camera::MoveYaw(float _yawValue)
{
	if (!bCanMove) return;
	MoveYawSuper(_yawValue, mouseSensitivitySpeed * (target->GetCharacterAttackComponent()->IsPlayerChargingShoot() ? aimRotateFactor : 1.0f));
}

void AIU_Camera::MoveYawController(float _yawValue)
{
	if (!bCanMove) return;
	MoveYawSuper(_yawValue, sensitivityXSpeed * (target->GetCharacterAttackComponent()->IsPlayerChargingShoot() ? aimRotateFactor : 1.0f));
}

void AIU_Camera::MoveYawSuper(const float& _yawValue, const float& _sens, const bool& _ignoreAcceptance, const bool& _ignoreInvert)
{
	// Move around the target
	if (FMath::Abs(_yawValue) > axisAcceptance || _ignoreAcceptance)
	{
		const float& _result = _yawValue * yawSpeed * GetWorld()->DeltaTimeSeconds * aimAssistSpeed * _sens;
		yaw -= invertXAxis && !_ignoreInvert ? -_result : _result;
		yaw = yaw >= 360.0f
			      ? yaw - 360.0f
			      : yaw <= -360.0f
			      ? yaw + 360.0f
			      : yaw;

		if (!_ignoreAcceptance)
		{
			movingCamera = true;
		}
	}
	else
		movingCamera = false;
}

void AIU_Camera::MoveYawWithMovement(float _yawValue)
{
	if (!bCanMove || !passiveBehaviour || target->GetCharacterAttackComponent()->IsPlayerChargingShoot()) return;

	if (FMath::Abs(_yawValue) > axisAcceptance)
	{
		const float& _result = _yawValue * passiveYawSpeed * GetWorld()->DeltaTimeSeconds * aimAssistSpeed;
		yaw -= invertXAxis ? -_result : _result;
		yaw = yaw >= 360.0f
			      ? yaw - 360.0f
			      : yaw <= -360.0f
			      ? yaw + 360.0f
			      : yaw;
	}
}

#pragma endregion

#pragma region Pitch

void AIU_Camera::MovePitch(float _pitchValue)
{
	if (!bCanMove) return;
	MovePitchSuper(_pitchValue, mouseSensitivitySpeed * (target->GetCharacterAttackComponent()->IsPlayerChargingShoot() ? aimRotateFactor : 1.0f));
}

void AIU_Camera::MovePitchController(float _pitchValue)
{
	if (!bCanMove) return;
	MovePitchSuper(_pitchValue, sensitivityYSpeed * (target->GetCharacterAttackComponent()->IsPlayerChargingShoot() ? aimRotateFactor : 1.0f));
}

void AIU_Camera::MovePitchSuper(const float& _pitchValue, const float& _sens, const bool& _ignoreAcceptance, const bool& _ignoreInvert)
{
}

void AIU_Camera::UpdateYawPitchViaAimAssist()
{
	// if (aimAssistVector == FVector::Zero()) return;
	//
	// const float _aimAssistYaw = aimAssistVector.X;
	// const float _aimAssistPitch = -aimAssistVector.Z;
	//
	// if (_aimAssistYaw != 0.0f)
	// {
	// 	MoveYawSuper(_aimAssistYaw, true, true);
	// }
	//
	// if (_aimAssistPitch != 0.0f)
	// {
	// 	MovePitchSuper(_aimAssistPitch, true, true);
	// }
}

#pragma endregion

FVector AIU_Camera::ComputeNewPosition(const float& _distance, const float& _pitch, const float& _yaw, const FVector& _targetLocation, const float& _minHeight)
{
	FVector _newLocation = FVector::ZeroVector;
	const float& _pitchInRad = FMath::DegreesToRadians(_pitch);
	const float& _yawInRad = FMath::DegreesToRadians(_yaw);

	_newLocation.Z = FMath::Sin(_pitchInRad) * _distance + _targetLocation.Z;
	_newLocation.Z = _newLocation.Z > _minHeight ? _newLocation.Z < cameraMaxHeight ? _newLocation.Z : cameraMaxHeight : _minHeight;

	const float& _projection = FMath::Cos(_pitchInRad) * _distance;
	_newLocation.X = FMath::Cos(_yawInRad) * _projection;
	_newLocation.Y = FMath::Sin(_yawInRad) * _projection;

	return FVector(_targetLocation.X + _newLocation.X, _targetLocation.Y + _newLocation.Y, _newLocation.Z);
}

bool AIU_Camera::CheckGroundCollisions()
{
	if (groundLayer.Num() < 1)
	{
		cameraMinHeight = -100000.0f;
		return false;
	}
	FHitResult _groundResult;
	const FVector& _endLocation = GetActorLocation() + raycastRange * -FVector::UpVector;
	const bool& _groundHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(), GetActorLocation(), _endLocation,
		groundLayer, false, TArray<AActor*>(),
		EDrawDebugTrace::None, _groundResult, true);

	if (_groundHit)
	{
		cameraMinHeight = _groundResult.ImpactPoint.Z + heightFromGround;
		return true;
	}
	cameraMinHeight = -100000.0f;
	return false;
}

bool AIU_Camera::CheckCeilingCollisions()
{
	if (ceilingLayer.Num() < 1)
	{
		cameraMaxHeight = 100000.0f;
		return false;
	}
	FHitResult _ceilingResult;
	const FVector& _endLocation = GetActorLocation() + raycastRange * FVector::UpVector;
	const bool& _ceilingHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(), GetActorLocation(), _endLocation,
		ceilingLayer, false, TArray<AActor*>(),
		EDrawDebugTrace::None, _ceilingResult, true);

	if (_ceilingHit)
	{
		cameraMaxHeight = _ceilingResult.ImpactPoint.Z - distanceFromCeiling;
		return true;
	}
	cameraMaxHeight = 100000.0f;
	return false;
}