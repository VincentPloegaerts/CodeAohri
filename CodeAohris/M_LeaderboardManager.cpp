#include "M_LeaderboardManager.h"

#include "B_Boss.h"
#include "IU_Macros.h"
#include "M_BossManager.h"
#include "M_GameInstance.h"
#include "M_GameMode.h"

AM_LeaderboardManager::AM_LeaderboardManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AM_LeaderboardManager::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &AM_LeaderboardManager::InitializeLeaderboardManager, 0.5f, false);
}
void AM_LeaderboardManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	elapsedTime += DeltaSeconds;
}

void AM_LeaderboardManager::InitializeLeaderboardManager()
{
	if (const UM_GameInstance* _gi = GetGameInstance<UM_GameInstance>())
		steamIntegration = _gi->GetSteamIntegration();


	StopTimer();

	if (const AM_GameMode* _gm = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if (AM_BossManager* _bossManager = _gm->GetBossManager())
		{
			_bossManager->OnBossChanged().AddUniqueDynamic(this, &AM_LeaderboardManager::InitializeManagerFromBoss);
			InitializeManagerFromBoss(_bossManager->GetCurrentBoss());
		}
	}
}

void AM_LeaderboardManager::InitializeManagerFromBoss(AB_Boss* _boss)
{
	if (!_boss) return;
	
	if (boss)
	{
		LOG("RemoveDynamic on old boss")
		boss->OnBossActivated().RemoveDynamic(this, &AM_LeaderboardManager::StartTimer);
		boss->OnBossReset().RemoveDynamic(this, &AM_LeaderboardManager::StopTimer);
		boss->OnBossDead().RemoveDynamic(this, &AM_LeaderboardManager::RegisterTimer);	
	}
	
	boss = _boss;

	boss->OnBossActivated().AddUniqueDynamic(this, &AM_LeaderboardManager::StartTimer);
	boss->OnBossReset().AddUniqueDynamic(this, &AM_LeaderboardManager::StopTimer);
	boss->OnBossDead().AddUniqueDynamic(this, &AM_LeaderboardManager::RegisterTimer);
}

void AM_LeaderboardManager::StartTimer()
{
	SetActorTickEnabled(true);
	elapsedTime = 0;

	ON_SCREEN_LOG_TIME_(10, "Start Timer...")
}
void AM_LeaderboardManager::StopTimer()
{
	SetActorTickEnabled(false);
	elapsedTime = 0;	
	ON_SCREEN_LOG_TIME_(10, "Stop Timer")
}
void AM_LeaderboardManager::RegisterTimer()
{
	if (!steamIntegration || !boss) return;
	
	const float _time = elapsedTime;
	StopTimer();

#if WITH_EDITOR
	const FString& _msg = FString::Printf(TEXT("Register score for Boss : %s"), *boss->GetActorLabel());
	ON_SCREEN_LOG_TIME(10, FColor::Yellow, *_msg);
#endif
	
	const EBossPhase& _phase = boss->GetBossPhase();

	steamIntegration->UpdateLeaderboardBoss(_phase, _time);
}