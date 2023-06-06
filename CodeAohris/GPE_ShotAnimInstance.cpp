// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE_ShotAnimInstance.h"
#include "GPE_Shot.h"

void UGPE_ShotAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	if (AGPE_Shot* _shot = Cast<AGPE_Shot>(GetOwningActor()))
	{
		InitEvent(_shot);
	}
}

void UGPE_ShotAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UGPE_ShotAnimInstance::InitEvent(AGPE_Shot* _shot)
{
	_shot->OnActivate().AddDynamic(this,&UGPE_ShotAnimInstance::UpdateIsActivated);
	_shot->OnShoot().AddDynamic(this,&UGPE_ShotAnimInstance::UpdateIsShooting);
	
	 if(UIU_HealthSystemComponent* _healthComp = _shot->HealthSystemComponent())
	 {
	 	Event_AddUnique(_healthComp->OnTakingHit(), UGPE_ShotAnimInstance::UpdateIsTakingHit);
	 }
}

void UGPE_ShotAnimInstance::UpdateIsTakingHit(const bool _isTakingHit)
{
	isTakingHit = _isTakingHit;
}

void UGPE_ShotAnimInstance::UpdateIsShooting(const bool _isShooting)
{
	isShooting = _isShooting;

	FTimerHandle _timer;

	
	GetWorld()->GetTimerManager().SetTimer(_timer,this,&UGPE_ShotAnimInstance::UpdateIsShootingToFalse,0.1,false,0.1);
}

void UGPE_ShotAnimInstance::UpdateIsActivated(const bool _isActivated)
{
	isActivated = _isActivated;
}

void UGPE_ShotAnimInstance::UpdateIsShootingToFalse()
{
	isShooting = false;
}

