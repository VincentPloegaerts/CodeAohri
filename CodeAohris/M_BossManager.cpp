#include "M_BossManager.h"

#include "M_GameInstance.h"
#include "M_GameMode.h"

#include "S_GameSave.h"

#include "M_ArenaManager.h"

#include "B_Boss.h"
#include "A_Arena.h"
#include "IU_HealthSystemComponent.h"

#include "IU_Macros.h"

AM_BossManager::AM_BossManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AM_BossManager::BeginPlay()
{
	Super::BeginPlay();
	gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>();
	ROG_ERROR(!gameMode, "AM_BossManager::BeginPlay -> Invalid Gamemode");
	arenaManager = gameMode->GetArenaManager(); //arenaManager created before bossManager
	ROG_ERROR(!arenaManager, "AM_BossManager::BeginPlay -> Invalid arenaManager");
	gameSave = GetGameInstance<UM_GameInstance>()->GetCurrentGameSave();
	if (gameSave)
	{
		currentBossPhase = gameSave->GetBossPhase();
	}
	
}

void AM_BossManager::SetArena(AA_Arena* _arena, const EBossPhase& _bossPhase)
{
#if WITH_LOG_PHASE_BOSS
	LOG("AM_BossManager::SetArena -> Arena %s", *_arena->GetName());
#endif
	
	ROG_ERROR(!arenaManager, "AM_BossManager::SetArena -> Invalid Arena manager");
	ROG_ERROR(!bossPhaseSettings.Contains(_bossPhase), "AM_BossManager::SetArena -> Invalid Phase");
	
	FBossPhase& _bossSettings = bossPhaseSettings.FindChecked(_bossPhase);

	AA_Arena*& _currentArena = _bossSettings.arena;
	ROG_ERROR(_currentArena, "AM_BossManager::SetArena -> There is already an arena of phase %s", *UEnum::GetValueAsString(_bossPhase));

	_currentArena = _arena;
	
	if (currentBossPhase == _bossPhase)
	{
		currentArena = _arena;
		arenaManager->SetArena(_arena);
		onArenaChanged.Broadcast(_arena);
	}
}

void AM_BossManager::SetBoss(AB_Boss* _boss, const EBossPhase& _bossPhase)
{
#if WITH_LOG_PHASE_BOSS
	LOG("AM_BossManager::SetBoss -> Boss %s", *_boss->GetName());
#endif
	
	ROG_ERROR(!gameMode, "AM_BossManager::SetBoss -> Invalid Gamemode");
	ROG_ERROR(!bossPhaseSettings.Contains(_bossPhase), "AM_BossManager::SetBoss -> Invalid Phase");
	
	FBossPhase& _bossSettings = bossPhaseSettings.FindChecked(_bossPhase);
	
	AB_Boss*& _currentBoss = _bossSettings.boss;
	ROG_ERROR(_currentBoss, "AM_BossManager::SetBoss -> There is already a boss of phase %s", *UEnum::GetValueAsString(_bossPhase));

	_currentBoss = _boss;
	
	if (currentBossPhase == _bossPhase)
	{
		gameMode->SetBoss(_boss);
		_boss->Init();
		currentBoss = _boss;
		onBossChanged.Broadcast(_boss);
		if (UIU_HealthSystemComponent* _health = _boss->GetHealthComponent())
		{
			_health->OnDeath()->AddDynamic(this, &AM_BossManager::IncrementBossPhase);
		}
	}
}

void AM_BossManager::IncrementBossPhase()
{
#if WITH_LOG_PHASE_BOSS
	LOG("AM_BossManager::IncrementBossPhase -> Phase Incremented from Phase %s", *UEnum::GetValueAsString(currentBossPhase));
#endif
	
	if (currentBossPhase == UN)
		currentBossPhase = DEUX;
	else if (currentBossPhase == DEUX)
		currentBossPhase = TROIS;
	else
		return;

	if (gameSave)
		gameSave->SetBossPhase(currentBossPhase);

	ROG_ERROR(!gameMode || !arenaManager, "AM_BossManager::IncrementBossPhase -> Invalid Managers");
	ROG_ERROR(!bossPhaseSettings.Contains(currentBossPhase), "AM_BossManager::IncrementBossPhase -> Invalid Phase");
	
	const FBossPhase& _bossSettings = bossPhaseSettings.FindChecked(currentBossPhase);
	
	if (AA_Arena* _arena = _bossSettings.arena)
	{
		arenaManager->SetArena(_arena);
		currentArena = _arena;
		onArenaChanged.Broadcast(_arena);
	}
	else
		LOG_ERROR("AM_BossManager::IncrementBossPhase -> Invalid next arena");
	
	if (AB_Boss* _boss = _bossSettings.boss)
	{
		gameMode->SetBoss(_boss);
		_boss->Init();
		currentBoss = _boss;
		if (UIU_HealthSystemComponent* _health = _boss->GetHealthComponent())
		{
			_health->OnDeath()->AddDynamic(this, &AM_BossManager::IncrementBossPhase);
		}
		onBossChanged.Broadcast(_boss);
	}
	else
		LOG_ERROR("AM_BossManager::IncrementBossPhase -> Invalid next boss");
}