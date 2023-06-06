#include "M_GPE_PatternTickManager.h"

#include "B_Boss.h"
#include "M_GameMode.h"
#include "GPE_Zone.h"
#include "HUD_Game.h"
#include "IU_CameraManager.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "M_BossManager.h"
#include "UI_ZoneWarningWidget.h"

AM_GPE_PatternTickManager::AM_GPE_PatternTickManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AM_GPE_PatternTickManager::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle _handle;
	GetWorld()->GetTimerManager().SetTimer(_handle,this,&AM_GPE_PatternTickManager::Init,1.75f,false);
}

void AM_GPE_PatternTickManager::Tick(float DeltaTime)
{
	const float& _deltaTime = fWorldSpeed * DeltaTime;
	Super::Tick(_deltaTime);
	onTick.Broadcast(_deltaTime);
}

void AM_GPE_PatternTickManager::AddGPEPattern(AGPE_Pattern* _gpePattern)
{
	if (!_gpePattern)return;
	allGPEPattern.Add(_gpePattern);
	onTick.AddDynamic(_gpePattern, &AGPE_Pattern::BehaviorTick);
	if (!zoneWarning)return;
	zoneWarning->AddWarning(_gpePattern);
}

void AM_GPE_PatternTickManager::DestroyGPEPattern(AGPE_Pattern* _gpePattern)
{
	allGPEPattern.Remove(_gpePattern);
	onTick.RemoveDynamic(_gpePattern, &AGPE_Pattern::BehaviorTick);
	_gpePattern->Destroy();
}

void AM_GPE_PatternTickManager::DestroyEveryGPEPattern()
{
	uint _gpeCount = allGPEPattern.Num();
	while (_gpeCount)
	{
		if (allGPEPattern.Num() < 1)break;
		
		AGPE_Pattern* _gpePattern = allGPEPattern[0];
		allGPEPattern.Remove(_gpePattern);
		if (IsValid(_gpePattern))
		{
			onTick.RemoveDynamic(_gpePattern, &AGPE_Pattern::BehaviorTick);
			_gpePattern->Destroy();
		}
		--_gpeCount;
	}

	allGPEPattern.Empty();
}

void AM_GPE_PatternTickManager::CheckZonePosition()
{
	
}

void AM_GPE_PatternTickManager::Init()
{
	const AM_GameMode* _gm = GetWorld()->GetAuthGameMode<AM_GameMode>();
	camManager = _gm->GetCameraManager();
	player = _gm->GetPlayer();
	BossManager = _gm->GetBossManager();
	BossManager->OnBossChanged().AddUniqueDynamic(this, &AM_GPE_PatternTickManager::SwitchCurrentBoss);
	currentBoss  = BossManager->GetCurrentBoss();
	if (currentBoss)
	{
		currentBoss->OnBossActivated().AddUniqueDynamic(this, &AM_GPE_PatternTickManager::CurrentBossActivated);
		currentBoss->OnBossDead().AddUniqueDynamic(this, &AM_GPE_PatternTickManager::CurrentBossDeath);
		currentBoss->OnBossDead().AddUniqueDynamic(this, &AM_GPE_PatternTickManager::DestroyEveryGPEPattern);
	}
	hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Game>();
}

void AM_GPE_PatternTickManager::SwitchCurrentBoss(AB_Boss* _boss)
{
	if (!_boss)return;
	if (currentBoss)
	{
		currentBoss->OnBossActivated().RemoveDynamic(this, &AM_GPE_PatternTickManager::CurrentBossActivated);
		currentBoss->OnBossDead().RemoveDynamic(this, &AM_GPE_PatternTickManager::DestroyEveryGPEPattern);
	}
	
	currentBoss = _boss;
	
	currentBoss->OnBossActivated().AddUniqueDynamic(this, &AM_GPE_PatternTickManager::CurrentBossActivated);
	currentBoss->OnBossDead().AddUniqueDynamic(this, &AM_GPE_PatternTickManager::CurrentBossDeath);
	currentBoss->OnBossDead().AddUniqueDynamic(this, &AM_GPE_PatternTickManager::DestroyEveryGPEPattern);
}

void AM_GPE_PatternTickManager::CurrentBossActivated()
{
	bossStarted = true;
	currentCamera = camManager->GetCamera();
	zoneWarning = Cast<UUI_ZoneWarningWidget>(hud->ShowGameWidget(WIDGET_WARNINGS_ZONE));
	if (zoneWarning)
		zoneWarning->Init(player,currentCamera);
}

void AM_GPE_PatternTickManager::CurrentBossDeath()
{
	bossStarted = false;
	hud->HideGameWidget(WIDGET_WARNINGS_ZONE);
}