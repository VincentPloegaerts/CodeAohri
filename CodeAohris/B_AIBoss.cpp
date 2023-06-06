#include "B_AIBoss.h"

#include "BehaviorTree/BehaviorTree.h"
#include "B_Boss.h"
#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"
#include "M_BlackBoardManager.h"
#include "M_GameMode.h"

void AB_AIBoss::BeginPlay()
{
	Super::BeginPlay();
	boss = Cast<AB_Boss>(GetPawn());
	if (boss)
	{
		patternCaster = boss->GetPatternCaster();

		Event_Add(boss->OnBossActivated(), AB_AIBoss::ActivateAIBaseController);
		Event_Add(boss->OnBossReset(), AB_AIBoss::ResetAIBaseController);
		
		Event_Add(boss->OnBossStunned(), AB_AIBoss::StunBehaviorTree);
		Event_Add(boss->OnBossStunnedEnd(), AB_AIBoss::EndStun);

		Event_Add(boss->OnBossMidLifeAnimStart(), AB_AIBoss::StunBehaviorTree);
		//Event_Add(boss->OnBossMidLifeAnimEnd(), AB_AIBoss::EndStun);
		
		if (UIU_HealthSystemComponent* _healthComponent = boss->GetHealthComponent())
		{
			_healthComponent->OnDeath()->AddDynamic(this, &AB_AIBoss::DeactivateBehaviorTree);
		}
		else
			LOG("AB_AIBoss::BeginPlay -> boss healthComponent is invalid");
	}
	else
		LOG("AB_AIBoss::BeginPlay -> invalid boss");
}

void AB_AIBoss::InitAIBossPhase()
{
	Super::InitAIBossPhase();
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if (boss == _gameMode->GetBoss())
			blackBoardManager->Set_OnBossMidLifeAnimEnd_AIBoss(this);
	}
}