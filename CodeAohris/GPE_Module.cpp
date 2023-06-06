#include "GPE_Module.h"

#include "GPE_SweepVerticalAnimInstance.h"
#include "GPE_ZoneModuActivation.h"
#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "IU_PlayerMovementComponent.h"
#include "M_GameMode.h"
#include "M_WorldTimeManager.h"
#include "STRUCT_CustomMath.h"

AGPE_Module::AGPE_Module()
{
	rotationRoot = CreateDefaultSubobject<USceneComponent>("Rotation Root");
	rotationRoot->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	
	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh");
	skeletalMesh->AttachToComponent(rotationRoot,FAttachmentTransformRules::KeepRelativeTransform);

	damageMesh = CreateDefaultSubobject<UStaticMeshComponent>("Damage Mesh");
	damageMesh->AttachToComponent(skeletalMesh,FAttachmentTransformRules::KeepRelativeTransform);
}

void AGPE_Module::BeginPlay()
{
	Super::BeginPlay();;
	if (!activationBox.Get())
		boxActivated = true;
	else
	{
		activationBox.Get()->OnActorBeginOverlap.AddUniqueDynamic(this,&AGPE_Module::BoxCollided);
	}
	if (skeletalMesh)
	{
		animInstanceRef = Cast<UGPE_SweepVerticalAnimInstance>(skeletalMesh->GetAnimInstance());
		if (animInstanceRef)
		{
			const bool& _useSweep = bUseAnimation && gpeMode == Sweep;
			animInstanceRef->SetPauseDuration(fPauseTime);
			animInstanceRef->SetPlayRateSweep(_useSweep && started ? fPlayRateAnim : 0.0f);
			animInstanceRef->SetPlayRateStopReverse(_useSweep && started ? fPlayRateWarning : 0.0f);
		}
		baseRotation = rotationRoot->GetRelativeRotation();
		const FVector& _destination =  FVector::ForwardVector.RotateAngleAxis(fSweepAngle,FVector::UpVector);
		finalRotation = _destination.Rotation();
	}
	if (damageMesh && gpeMode == Rotation)
	{
		damageMesh->OnComponentBeginOverlap.AddDynamic(this,&AGPE_Module::OnMeshOverlap);
	}
	else if (skeletalMesh && gpeMode == Sweep)
	{
		skeletalMesh->OnComponentBeginOverlap.AddDynamic(this,&AGPE_Module::OnMeshOverlap);
	}
	FTimerHandle _handle;
	GetWorld()->GetTimerManager().SetTimer(_handle,this,&AGPE_Module::Init,0.1f,false);
}

void AGPE_Module::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!boxActivated)
		return;
	if (!started)
	{
		if (animInstanceRef)
			animInstanceRef->SetPlayRateSweep(0.0f);
		fProgress+=DeltaSeconds;
		if (fProgress>delayBeforeActivation)
		{
			started = true;
			fProgress = 0.0f;			
			if (animInstanceRef)
			{
				animInstanceRef->SetPlayRateSweep(bUseAnimation && gpeMode == Sweep ? fPlayRateAnim : 0.0f);
				animInstanceRef->SetPlayRateStopReverse(bUseAnimation && gpeMode == Sweep ? fPlayRateWarning : 0.0f);
			}
			return;
		}
		return;
	}

	if (bHasCollideThisFrame)
		bHasCollideThisFrame = false;
	
	if (bIsPaused)
	{
		UpdatePause(DeltaSeconds);
		return;
	}
	switch (gpeMode)
	{
	case Base:
		UpdateBase(DeltaSeconds * fSlowSpeed);
		break;
	case Sweep:
		UpdateSweep(DeltaSeconds* fSlowSpeed);
		break;
	case Rotation:
		UpdateRotation(DeltaSeconds* fSlowSpeed);
		break;
	default:
		return;
	}
}

void AGPE_Module::UpdateBase(const float& _fDeltaSeconds)
{
	//TODO
}

void AGPE_Module::UpdateSweep(const float& _fDeltaSeconds)
{
	if (bUseAnimation)return;
	fProgress += _fDeltaSeconds * (bGoingForward ? fSpeedForward : - fSpeedBackward);
	fProgress = fProgress >= fSweepAngle ? fSweepAngle :
				fProgress <= 0.0f ? 0.0f : fProgress;
	const FRotator& _finalRotation = FMath::Lerp( baseRotation, finalRotation, fProgress / fSweepAngle);
	rotationRoot->SetRelativeRotation(_finalRotation);
	if (fProgress == 0.0f || fProgress == fSweepAngle)
	{
		bGoingForward = !bGoingForward;
		bIsPaused = true;
		onPauseStart.Broadcast();
	}
	
}

void AGPE_Module::UpdateRotation(const float& _fDeltaSeconds)
{
	fProgress += _fDeltaSeconds * fRotateSpeed * (bCounterClockwise ? -1.0f : 1.0f);
	fProgress = fProgress >= 360.0f ? fProgress - 360.0f :
				fProgress <= -360.0f ? fProgress +360.0f : fProgress;
	const float& _radAngle = FMath::DegreesToRadians(fProgress);

	const FRotator& _rotation = FVector(FMath::Cos(_radAngle), FMath::Sin(_radAngle), 0).Rotation();
	rotationRoot->SetRelativeRotation(_rotation);
}

void AGPE_Module::UpdatePause(const float& _fDeltaSeconds)
{
	fPauseProgress += _fDeltaSeconds;
	if (fPauseProgress >= fPauseTime)
	{
		bIsPaused = false;
		fPauseProgress = 0.0f;
		onPauseEnd.Broadcast();
	}
}

void AGPE_Module::SetSlowSpeed(const float& _speed)
{
	fSlowSpeed = _speed;
}

void AGPE_Module::Init()
{
	const AM_GameMode* _gm =  GetWorld()->GetAuthGameMode<AM_GameMode>();
	if (!_gm)
	{
		FTimerHandle _handle;
		GetWorld()->GetTimerManager().SetTimer(_handle,this,&AGPE_Module::Init,0.1f,false);
		return;
	}
	AM_WorldTimeManager* _worldTimer = _gm->GetWorldTimeManager();
	playerRef = _gm->GetPlayer();
	if (_worldTimer)
		_worldTimer->OnWorldSpeedChange().AddDynamic(this,&AGPE_Module::SetSlowSpeed);
}

void AGPE_Module::OnMeshOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep,
	const FHitResult& _SweepResult)
{
	if (bHasCollideThisFrame)return;
	
	if (GetWorld()->GetTimerManager().IsTimerActive(timerHandle))return;
	if (playerRef == _otherActor)
	{
		playerRef->HealthComponent()->TakeDamageFromZone(damages);
		
		knockDirection = gpeMode == Rotation ? GetActorUpVector() : - GetActorRightVector();
		
		if (_SweepResult.ImpactPoint.Size() > 0.1f)
		{
			const float& _angleHit = FMath::RadiansToDegrees(FCustomMath::AngleBetweenVectors(playerRef->GetActorForwardVector(), knockDirection));
			knockDirection *= knockBackStrength * (_angleHit < 90.0f ? -1.0f : 1.0f);
		}
		else
		{
			FVector _location = GetActorLocation();
			_location.Z = 0;
			FVector _playerLocation = playerRef->GetActorLocation();
			_playerLocation.Z = 0;
			
			const float& _angle = FMath::RadiansToDegrees(FCustomMath::AngleBetweenVectors(_playerLocation - _location, GetActorRightVector()));

			knockDirection *= knockBackStrength * (_angle > 90.f ? 1.f : -1.f);
		}
		
		bHasCollideThisFrame = true;
		KnockPlayer(playerRef);
	}
}

void AGPE_Module::KnockPlayer(AIU_Player* _player)
{
	UIU_PlayerMovementComponent* _playerMove = _player->MovementComponent();
	_playerMove->SetMomentum(FVector(0, 0, -50));
	_playerMove->StartKnockBack(knockDirection, knockBackDuration);
}

void AGPE_Module::CheckPlayerDistance()
{
}

void AGPE_Module::BoxCollided(AActor* _box, AActor* _other)
{
	if (_other == playerRef)
	{
		boxActivated = true;
	}
}
