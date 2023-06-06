#include "M_BossRushLogic.h"

#include "A_ArenaBlock.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "M_BossManager.h"
#include "M_GameMode.h"
#include "HUD_Game.h"
#include "IU_HealthSystemComponent.h"
#include "M_CinematicManager.h"
#include "M_GameInstance.h"
#include "UI_ConfirmWidget.h"
#include "UI_PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"

class UUI_ConfirmWidget;

AM_BossRushLogic::AM_BossRushLogic()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AM_BossRushLogic::BeginPlay()
{
	Super::BeginPlay();
	for (const TPair<TSoftObjectPtr<AB_Boss>, TSoftObjectPtr<AA_ArenaBlock>>& pair : positionToGoForEachBoss)
	{
		pair.Value->SetCloseDelay(delayBeforeBossStart);
	}
	FTimerHandle _handle;
	GetWorld()->GetTimerManager().SetTimer(_handle, this, &AM_BossRushLogic::Init, 0.2f);
}

void AM_BossRushLogic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AM_BossRushLogic::Init()
{
	gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>();
	cinematicManager = gameMode->GetCinematicManager();
	bossManager = gameMode->GetBossManager();
	bossManager->OnBossChanged().AddUniqueDynamic(this, &AM_BossRushLogic::BossFinished);
	player = gameMode->GetPlayer();
	playerController = GetWorld()->GetFirstPlayerController();
	hud = playerController->GetHUD<AHUD_Game>();
	if (hud)
		if (UUI_PauseMenuWidget* _widget = hud->GetWidget<UUI_PauseMenuWidget>(WIDGET_PAUSE))
			_widget->LockButtons();
	
	gameInstance = GetGameInstance<UM_GameInstance>();
	for (const TPair<TSoftObjectPtr<AB_Boss>, TSoftObjectPtr<AA_ArenaBlock>>& pair : positionToGoForEachBoss)
	{
		if (pair.Key.Get() == gameInstance->GetCurrentStepBossRush().Get())
		{
			nextTP = positionToGoForEachBoss[pair.Key].Get();
			break;
		}
	}
	cinematicManager->OnCinematicFinishedEvent().AddUniqueDynamic(this, &AM_BossRushLogic::BossStarting);
	if (nextTP)
	{
		nextTP->Interacted();
	}
}

void AM_BossRushLogic::BossFinished(AB_Boss* _boss)
{
	if (lastBoss.Get() == _boss)
	{
		//ended boss rush
	}
	for (const TPair<TSoftObjectPtr<AB_Boss>, TSoftObjectPtr<AA_ArenaBlock>>& pair : positionToGoForEachBoss)
	{
		if (pair.Key.Get() == _boss)
		{
			// cinematicManager->SetCanSkipCinematic(false);
			cinematicManager->OnCinematicFinishedEvent().AddUniqueDynamic(this, &AM_BossRushLogic::TeleportToNewBoss);
			nextTP = positionToGoForEachBoss[pair.Key].Get();
			break;
		}
	}
}

void AM_BossRushLogic::TeleportToNewBoss()
{
	cinematicManager->OnCinematicFinishedEvent().RemoveDynamic(this, &AM_BossRushLogic::TeleportToNewBoss);
	//UGameplayStatics::SetGamePaused(GetWorld(), true); //sinon y'a pas les timers
	hud->HideGameWidgets();
	FInputModeGameAndUI _inputMode = FInputModeGameAndUI();
	_inputMode.SetHideCursorDuringCapture(false);
	playerController->SetInputMode(_inputMode);
	playerController->SetShowMouseCursor(true);
	player->DisableInput(playerController);
	
	AskGoToNextBoss();
}

void AM_BossRushLogic::TeleportPlayer()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	player->EnableInput(playerController);
	hud->ShowGameWidgets();
	playerController->SetInputMode(FInputModeGameOnly());
	playerController->SetShowMouseCursor(false);
	cinematicManager->OnCinematicFinishedEvent().AddUniqueDynamic(this, &AM_BossRushLogic::BossStarting);
	nextTP->Interacted();
}

void AM_BossRushLogic::AskGoToNextBossDelay()
{
	FTimerHandle _handle;
	GetWorld()->GetTimerManager().SetTimer(_handle, this, &AM_BossRushLogic::AskGoToNextBoss, 0.01f);
}

void AM_BossRushLogic::AskGoToNextBoss()
{
	if (UUI_ConfirmWidget* _widget = hud->GetConfirmWidget())
	{
		_widget->InitializeConfirmBox(changeBossConfirmBoxTitle, changeBossConfirmBoxText);
		_widget->OnConfirm().AddUniqueDynamic(this, &AM_BossRushLogic::TeleportPlayer);
		_widget->OnCancel().AddUniqueDynamic(this, &AM_BossRushLogic::AskGoBackToMenuDelay);
	}
}

void AM_BossRushLogic::AskGoBackToMenuDelay()
{
	FTimerHandle _handle;
	GetWorld()->GetTimerManager().SetTimer(_handle, this, &AM_BossRushLogic::AskGoBackToMenu, 0.01f);
}

void AM_BossRushLogic::AskGoBackToMenu()
{
	if (UUI_ConfirmWidget* _widget = hud->GetConfirmWidget())
	{
		_widget->InitializeConfirmBox(goBackToMenuConfirmBoxTitle, goBackToMenuConfirmBoxText);
		_widget->OnConfirm().AddUniqueDynamic(this, &AM_BossRushLogic::GoBackToMainMenu);
		_widget->OnCancel().AddUniqueDynamic(this, &AM_BossRushLogic::AskGoToNextBossDelay);
	}
}

void AM_BossRushLogic::GoBackToMainMenu()
{
	if (UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
		_gi->ReturnToMainMenu();
}

void AM_BossRushLogic::BossStarting()
{
	cinematicManager->OnCinematicFinishedEvent().RemoveDynamic(this, &AM_BossRushLogic::BossStarting);
	hud->ShowGameWidget(WIDGET_BOSS_RUSH);
	hud->ShowInitialWidget();
}
