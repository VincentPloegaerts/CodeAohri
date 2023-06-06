#include "B_AI_AFKKiller.h"

#include "B_AIBoss.h"
#include "B_Boss.h"
#include "IU_Macros.h"
#include "M_GameMode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "B_AFKKiller.h"
#include "IU_HealthSystemComponent.h"
#include "IU_Player.h"
#include "M_BlackBoardManager.h"
#include "M_BossManager.h"

AB_AI_AFKKiller::AB_AI_AFKKiller()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AB_AI_AFKKiller::BeginPlay()
{
	Super::BeginPlay();
	SubToBossAndBossManager();
}

void AB_AI_AFKKiller::InitAIBossPhase()
{
	Super::InitAIBossPhase();
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if (boss == _gameMode->GetBoss())
			blackBoardManager->Set_OnBossMidLifeAnimEnd_AIAFKKIller(this);
	}
}

void AB_AI_AFKKiller::SubToBossAndBossManager()
{
	if (const AM_GameMode* _gameMode = world->GetAuthGameMode<AM_GameMode>())
	{
		SubToBoss(_gameMode->GetBoss());
		bossManager = _gameMode->GetBossManager();
		if (bossManager)
			Event_Add(bossManager->OnBossChanged(), AB_AI_AFKKiller::SubToBoss);
		else
			LOG("AB_AI_AFKKiller::SubToBossEvent -> Invalid Boss Manager");
	}
	else
		LOG("AB_AI_AFKKiller::SubToBossEvent -> Invalid GameMode");
}

void AB_AI_AFKKiller::SubToBoss(AB_Boss* _boss)
{
	boss = _boss;
	if (!boss)return;

	if (boss->GetBossPhase() != bossPhase)return;
	
	Event_AddUnique(boss->OnBossActivated(), AB_AI_AFKKiller::ActivateAIBaseController);
	Event_AddUnique(boss->OnBossReset(), AB_AI_AFKKiller::ResetAIBaseController);
	
	Event_AddUnique(boss->OnBossStunned(), AB_AI_AFKKiller::StunBehaviorTree);
	Event_AddUnique(boss->OnBossStunnedEnd(), AB_AI_AFKKiller::EndStun);
	
	Event_AddUnique(boss->OnBossMidLifeAnimStart(), AB_AI_AFKKiller::StunBehaviorTree);
	Event_AddUnique(boss->OnBossMidLifeAnimEnd(), AB_AI_AFKKiller::EndStun);

	bossHealth = boss->GetHealthComponent();
	if (bossHealth)
		bossHealth->OnDeath()->AddUniqueDynamic(this, &AB_AI_AFKKiller::DestroyAFKKillerOnBossDeath);
	else
		LOG("AB_AI_AFKKiller::SubToBoss -> Invalid boss health component");

	if (const AB_AFKKiller* _afkKiller = Cast<AB_AFKKiller>(GetPawn()))
	{
		patternCaster = _afkKiller->GetPatternCaster();
	}
	else
		LOG("AB_AI_AFKKiller::SubToBoss -> Invalid afk killer Pawn owner");
}

void AB_AI_AFKKiller::DestroyAFKKillerOnBossDeath()
{
	if (boss)
	{
		Event_Remove(boss->OnBossActivated(), AB_AI_AFKKiller::ActivateAIBaseController);
		Event_Remove(boss->OnBossReset(), AB_AI_AFKKiller::ResetAIBaseController);
		
		Event_Remove(boss->OnBossStunned(), AB_AI_AFKKiller::StunBehaviorTree);
		Event_Remove(boss->OnBossStunnedEnd(), AB_AI_AFKKiller::EndStun);
		
		Event_Remove(boss->OnBossMidLifeAnimStart(), AB_AI_AFKKiller::StunBehaviorTree);
		Event_Remove(boss->OnBossMidLifeAnimEnd(), AB_AI_AFKKiller::EndStun);
	}
	if (bossHealth)
		bossHealth->OnDeath()->RemoveDynamic(this, &AB_AI_AFKKiller::DestroyAFKKillerOnBossDeath);
	if (bossManager)
		Event_Remove(bossManager->OnBossChanged(), AB_AI_AFKKiller::SubToBoss);

	DestroyGPEAndProjectile();
	if (APawn* _pawn = GetPawn())
		_pawn->Destroy();
	Destroy();
}