#include "GPE_Laser.h"

#include "IU_HealthSystemComponent.h"
#include "IU_Player.h"
#include "M_GameMode.h"
#include "Components/BillboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"


AGPE_Laser::AGPE_Laser()
{
#if WITH_EDITOR
	laserBillboard = CreateDefaultSubobject<UBillboardComponent>("Laser Billboard");
	laserBillboard->SetupAttachment(RootComponent);
#endif
	
	niagaraLaser = CreateDefaultSubobject<UNiagaraComponent>("NiagaraLaser");
	niagaraLaser->AttachToComponent(staticMesh, FAttachmentTransformRules::KeepRelativeTransform);
	
	damageBox = CreateDefaultSubobject<UStaticMeshComponent>("TriggerBox");
	damageBox->AttachToComponent(niagaraLaser, FAttachmentTransformRules::KeepRelativeTransform);
}

void AGPE_Laser::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void AGPE_Laser::Init()
{
	baseRotation = niagaraLaser->GetRelativeRotation();
	const FVector& _destination =  FVector::ForwardVector.RotateAngleAxis(angleBalayage,FVector::UpVector);
	finalRotation = _destination.Rotation();
	OnActorBeginOverlap.AddDynamic(this, &AGPE_Laser::OnHit);
	beamEnd	= damageBox->GetComponentScale();
	beamEnd.Y *=10.0f;
	beamEnd.Z *= 10.0f;
	niagaraLaser->SetVectorParameter(nameSizeBeamProperty,beamEnd);
}

void AGPE_Laser::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!started)
	{
		niagaraLaser->Deactivate();
		timePattern+=DeltaSeconds;
		if (timePattern>delayBeforeActivation)
		{
			niagaraLaser->Activate(true);

			started = true;
			timePattern = 0;
			return;
		}
		return;
	}
	
	switch (laserMode)
	{
	case Standard:
		UpdateStandard(DeltaSeconds);
		break;
	case Balayage:
		UpdateBalayage(DeltaSeconds);
		break;
	case Rotate:
		UpdateRotate(DeltaSeconds);
		break;
	case Infinity:
		break;
	default:
		return;
	}
}

void AGPE_Laser::UpdateStandard(float _deltaSeconds)
{
	timePattern += _deltaSeconds;
	if (timePattern >= duration)
	{
		isActive = !isActive;
		timePattern = 0;
		niagaraLaser->SetActive(isActive,true);
		damageBox->SetActive(isActive,true);		
		isActive ? onLaserActivate.Broadcast() : onLaserDeactivate.Broadcast();
	}
}

void AGPE_Laser::UpdateBalayage(float _deltaSeconds)
{
	if (timePattern / duration  *100.0f > 100 - halfSlowPortionPercent || timePattern / duration *100.0f < halfSlowPortionPercent)
	{
		actualSpeedPercent = FMath::Lerp(actualSpeedPercent,speedPercentSlowPortion,_deltaSeconds * speedChangeLerpSpeed);		
	}
	else
		actualSpeedPercent = FMath::Lerp(actualSpeedPercent,100.0f,_deltaSeconds * speedChangeLerpSpeed);
	timePattern += _deltaSeconds * actualSpeedPercent /100.0f;
	const FRotator& _finalRotation = FMath::Lerp( isActive ? baseRotation : finalRotation, isActive ? finalRotation : baseRotation, timePattern / duration);
	niagaraLaser->SetRelativeRotation(_finalRotation);
	if (timePattern >= duration)
	{
		isActive = !isActive;
		timePattern = 0;
	}
}

void AGPE_Laser::UpdateRotate(float _deltaSeconds)
{
	timePattern += _deltaSeconds;
	if (timePattern >= duration)
	{
		timePattern -= duration;
	}
	const float& _angle = (timePattern / duration) * 360.0f * (rotateCounterClockWise ?  -1.0f : 1.0f);
	const float& _radAngle = FMath::DegreesToRadians(_angle);

	const FRotator _rotation = FVector(FMath::Cos(_radAngle), FMath::Sin(_radAngle), 0).Rotation();
	niagaraLaser->SetRelativeRotation(_rotation);
}

void AGPE_Laser::OnHit(AActor* _self, AActor* _other)
{
	if (!isActive && laserMode == Standard)return;
	if (const AIU_Player* _player = Cast<AIU_Player>(_other))
	{
		_player->HealthComponent()->TakeDamageFromZone(damages);
	}
}


