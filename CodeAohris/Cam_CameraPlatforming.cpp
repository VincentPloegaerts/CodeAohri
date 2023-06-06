#include "Cam_CameraPlatforming.h"

#include "IU_PlayerMovementComponent.h"
#include "M_GameMode.h"
#include "STRUCT_GameInput.h"
#include "UC_PlayerAttackComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"



void ACam_CameraPlatforming::BeginPlay()
{
	Super::BeginPlay();
	baseFOV = render->FieldOfView;
	FTimerHandle _handle;
	GetWorld()->GetTimerManager().SetTimer(_handle,this,&ACam_CameraPlatforming::Init,1.0f,false);
}

void ACam_CameraPlatforming::Init()
{
	UInputComponent* _input = GetWorld()->GetFirstPlayerController()->InputComponent;
	if (!_input)return;
	const FGameInput& _gameInput = FGameInput();
	_input->BindAxis(_gameInput.VerticalMovementAxis.AxisName,this,&ACam_CameraPlatforming::ModifyFOV).bConsumeInput = false;
	_input->BindAxis(_gameInput.HorizontalMovementAxis.AxisName,this,&ACam_CameraPlatforming::KeepingFOV).bConsumeInput = false;
	GetWorld()->GetAuthGameMode<AM_GameMode>()->GetPlayer()->MovementComponent()->OnDashed().AddDynamic(this,&ACam_CameraPlatforming::DashModifyFOV);
}

void ACam_CameraPlatforming::Tick(float DeltaSeconds)
{
	if (useCinematicAngles)
	{
		
		yaw = FMath::Lerp(yaw,yawCinematic,DeltaSeconds * lerpToCinematicSpeed);
		pitch = FMath::Lerp(pitch,pitchCinematic,DeltaSeconds * lerpToCinematicSpeed);
		UpdateLocation(DeltaSeconds);
		return;
	}
	CheckValue(pitchCheckTime,angleCheckRate,movingPitch,DeltaSeconds);
	CheckValue(yawCheckTime,angleCheckRate,movingYaw,DeltaSeconds);
	if (keepFOV)
		UpdateTimerFOV(DeltaSeconds);
	
	if (!isActive)return;
	CheckGroundCollisions();
	CheckCeilingCollisions();
	UpdateLocation(DeltaSeconds);	
	
	if (movingCamera || movingPitch || movingYaw)
	{
		passiveBehaviour = false;
		passiveReactivationTime = 0.0f;		
	}
	else
	{
		if (passiveReactivationTime > passiveReactivationDelay)
		{
			passiveBehaviour = true;
			passiveReactivationTime = 0.0f;
			return;
		}
		passiveReactivationTime += DeltaSeconds;
	}
}

void ACam_CameraPlatforming::UpdateLocation(const float& _deltaTime)
{
	const FVector& _targetLocation = ComputeNewPosition(currentDistanceFromTarget /*+ sizeToCatchUp*/, pitch + additionalPitch, yaw,
	                                                    TargetLocationWithCameraOffset(positionOffset),
	                                                    cameraMinHeight + rampHeight);
	const FVector& _playerLocation = TargetLocation();
	const FVector& _currentLocation = GetActorLocation();

	FHitResult _hitResult;
	const bool _wall = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
	                                                                   _playerLocation,
	                                                                   _targetLocation,
	                                                                   cameraWallCollisionLayer,
	                                                                   false,
	                                                                   TArray<AActor*>(),
	                                                                   showLineTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
	                                                                   _hitResult,
	                                                                   true,
	                                                                   FLinearColor::Red,
	                                                                   FLinearColor::Green
	                                                                   ,2.0f);
	
	isWalled = _wall;
	
	FVector _trace = _targetLocation - _playerLocation;
	const float& _wallToPlayerSize = (_hitResult.ImpactPoint - _playerLocation).Size();
	const float& _targetToPlayerSize = _trace.Size();
	_trace.Normalize();
	
	if (!movingPitch && _wall && _hitResult.ImpactNormal.Dot(FVector::UpVector) < 0.8f)
	{
		additionalHeight = FMath::Lerp(additionalHeight,((_targetToPlayerSize - _wallToPlayerSize) / _targetToPlayerSize)* wallLerpFactor,_deltaTime * repositionSpeed);
	}
	else 
	{
		additionalHeight = FMath::Lerp(additionalHeight,0.0f,_deltaTime * wallLerpBackSpeed);
	}
	
	const FVector& _lastPos = _wall ? _hitResult.ImpactPoint - _trace * distanceMinFromWalls : _targetLocation;
	const FVector& _lerpResult = UKismetMathLibrary::VInterpTo(_currentLocation, _lastPos + FVector::UpVector * additionalHeight, _deltaTime, moveSpeed);
	SetActorLocation(_lerpResult);
	SetActorRotation((TargetLocationWithCameraOffset(positionOffset + lookAtOffset) - _targetLocation + FVector::DownVector * additionalPitchLookAt).Rotation());

}

void ACam_CameraPlatforming::MovePitchSuper(const float& _pitchValue,const float& _sens,const bool& _ignoreAcceptance,const bool& _ignoreInvert)
{
	if (FMath::Abs(_pitchValue) < axisAcceptance) //avoid unwanted inputs from a drifting controller or such
	{
		movingCamera = false;
		return;
	}
	float _result = _pitchValue * pitchSpeed * GetWorld()->DeltaTimeSeconds * (invertYAxis ? -1 : 1) *	aimAssistSpeed * _sens *
		(target->GetCharacterAttackComponent()->IsPlayerChargingShoot() ? aimRotateFactor : 1.0f) /*- aimAssistVector.Z*/;
	//UE_LOG(LogTemp,Warning,TEXT("%f"),_result);
	if (isWalled) //if we're against a wall do not move on the Z axis and just rotate on the camera pitch axis
	{
		additionalPitchLookAt += _result * speedAdditionalPitchLookAt;
		const float _absValue = FMath::Abs(additionalPitchLookAt);
		additionalPitchLookAt = _absValue > maxAdditionalPitchLookAt ? maxAdditionalPitchLookAt * additionalPitchLookAt / _absValue : additionalPitchLookAt;
		return;
	}
	
	additionalPitchLookAt = FMath::Lerp(additionalPitchLookAt,0.0f,GetWorld()->DeltaTimeSeconds * repositionSpeed); //if we're not on the wall come back to destination
	
	if (_result <0 && - _result > anglePassiveAcceptance) //check if we're moving on the pitch axis willingly or just not doing a perfect line with mouse/joystick
	{
		movingPitch = true;
		pitchCheckTime = 0.0f;
	}
	if (_result > 0 && additionalHeight !=0.0f) //reduce additional Height to 0 while moving the camera up
	{
		additionalHeight -= 2*_result;
		additionalHeight = additionalHeight <=0.0f ? 0.0f : additionalHeight;
	}
	else if (additionalHeight !=0.0f) //reduce additional Height before changing pitch value
	{
		additionalHeight += _result;
		if (additionalHeight <0) //don't skip some inputs if there's more input on this frame than additional Height left
		{
			_result = additionalHeight;
			additionalHeight = 0.0f;
		}
		else
		{			
			movingCamera = true;
			return;
		}
	}
	
	const float& _pitchInRad = FMath::DegreesToRadians(pitch + _result);
	const FVector& _targetLocationWithOffset = TargetLocationWithCameraOffset(positionOffset);
	const float& _newLocationZ = FMath::Sin(_pitchInRad) * currentDistanceFromTarget + _targetLocationWithOffset.Z;
	
	if (_newLocationZ < cameraMinHeight + rampHeight  && _result < 0.0f)
	{
		if (rampHeight > 0.0f)
		{
			rampHeight += _result * rampUpFactor;				
		}
		else
		{
			additionalPitch += _result;
			additionalPitch =  FMath::Clamp<float>(pitch+additionalPitch, minPitch, maxPitch) - pitch;
		}
	}
	else
	{
		if (additionalPitch <0 && _result > 0.0f)
		{
			rampHeight += _result*rampUpFactor;
			if (additionalPitch + _result >0.0f)
			{				
				_result -= additionalPitch;
				additionalPitch = 0.0f;
				pitch += _result * rampUpFactor;
				pitch = FMath::ClampAngle(pitch, minPitch, maxPitch);
			}
			else
			{
				additionalPitch += _result * rampUpFactor / 1.5f;
				additionalPitch = FMath::ClampAngle(additionalPitch + pitch,minPitch,maxPitch) - pitch;			
			}
			//LOG("%f",currentDistanceFromTarget);
		}
		else
		{
			if (rampHeight > 0.0f && _result >0.0f)
			{
				pitch += _result;
				pitch = FMath::ClampAngle(pitch, minPitch, maxPitch);
			}
			else
			{
				pitch +=  _result;
				pitch = FMath::ClampAngle(pitch, minPitch, maxPitch);
				rampHeight = 0.0f;
			}
		}
	}
}

void ACam_CameraPlatforming::MoveYawSuper(const float& _yawValue,const float& _sens,const bool& _ignoreAcceptance,const bool& _ignoreInvert)
{
	if (FMath::Abs(_yawValue) < axisAcceptance) //controller drift
	{
		movingCamera = false;
		return;
	}
	//CheckSurroundings(_yawValue);
	const float _result = _yawValue * yawSpeed * GetWorld()->DeltaTimeSeconds * aimAssistSpeed * _sens *
		(target->GetCharacterAttackComponent()->IsPlayerChargingShoot() ? aimRotateFactor : 1.0f);
	if (_result <0 && -_result > anglePassiveAcceptance) //CHeck if moving on the yaw axis willingly or miss inputing
	{
		movingYaw = true;
		movingCamera = true;
		yawCheckTime = 0.0f;
	}
	Super::MoveYawSuper(_yawValue,_sens);
}

void ACam_CameraPlatforming::CheckValue(float& _currentValue,const float& _max,bool& _toChange, const float& _toAdd)
{
	_currentValue += _toAdd;
	if (_currentValue > _max)
	{
		_toChange = false;
		_currentValue = 0.0f;
	}
}

void ACam_CameraPlatforming::ModifyFOV(float _input)
{
	if (!bCanMove)return;
	const float& _deltaTime = GetWorld()->DeltaTimeSeconds;
	if (_input >0.1f)
	{
		keepFOV = true;
		skipProgress = 0.0f;
		const float& _additionalFOV = _input * FOVLerpSpeed * _deltaTime;
		UpdateFOV(_additionalFOV);
	}
	else if (!keepFOV)
	{
		currentAdditionalFOV = FMath::Lerp(currentAdditionalFOV,0.0f,FOVLerpBackSpeed * _deltaTime);
		render->SetFieldOfView(baseFOV +  currentAdditionalFOV);
	}
}

void ACam_CameraPlatforming::UpdateFOV(float _additionalFOV)
{
	currentAdditionalFOV = currentAdditionalFOV + _additionalFOV > maxAdditionalFOV ? maxAdditionalFOV :
							currentAdditionalFOV + _additionalFOV;
	render->SetFieldOfView(baseFOV + currentAdditionalFOV);
}

void ACam_CameraPlatforming::KeepingFOV(float _input)
{
	if (!bCanMove)return;
	if (FMath::Abs(_input) > 0.05f)
	{
		keepFOV = true;
		skipProgress = 0.0f;
	}
	else if (keepFOV && skipProgress < skipTime)
		return;
	else
		keepFOV = false;
}

void ACam_CameraPlatforming::DashModifyFOV()
{
	UpdateFOV(additionalFOVOnDash);
}

void ACam_CameraPlatforming::UpdateTimerFOV(const float& _deltaTime)
{
	skipProgress += _deltaTime;
}

