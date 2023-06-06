// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ThornPowerOrb.h"
#include "IU_Player.h"
#include "UC_PlayerAttackComponent.h"

void AC_ThornPowerOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	AIU_Player* _player = Cast<AIU_Player>(OtherActor);

	if(!_player)return;

	

	if(UC_PlayerAttackComponent* _attackChara = _player->GetCharacterAttackComponent())
	{
		_attackChara->SetOrbUsed(true);
		_attackChara->SetThornRainState(true);
		_attackChara->SetHarpoonShootState(false);
		_attackChara->SetCurrentMaxShootTime();
		_attackChara->ResetThornTimer();
		_attackChara->FOnPowerOrbCollected().Broadcast();
	}
	

	if(twin)
		twin->Destroy();

	Destroy();
}

void AC_ThornPowerOrb::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &AC_ThornPowerOrb::SetPowerOrbLifeSpan,
											   0.5f, false);
}

void AC_ThornPowerOrb::SetPowerOrbLifeSpan()
{
	if(haveALifeSpan)
		SetLifeSpan(fLifeTime);
}
