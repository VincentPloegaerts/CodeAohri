#include "HUD_Game.h"

#include "STRUCT_GameInput.h"

#include "Kismet/GameplayStatics.h"

#include "LevelLoadingManager.h"
#include "M_CinematicManager.h"
#include "M_GameInstance.h"
#include "M_GameMode.h"
#include "UC_PlayerAttackComponent.h"
#include "UI_ConfirmWidget.h"
#include "UI_CreditWidget.h"

#include "IU_Macros.h"

bool AHUD_Game::CanPause() const
{
	const bool& _cinematicVisible = IsWidgetVisible(WIDGET_CINEMATIC);
	const bool& _loadVisible = IsWidgetVisible(WIDGET_LOADING);
	const bool& _creditVisible = IsWidgetVisible(WIDGET_CREDIT);
	const bool& _confirmVisible = IsWidgetVisible(WIDGET_CONFIRM);	//for Boss Rush 
	return !(_cinematicVisible || _loadVisible || _creditVisible || _confirmVisible);
}

bool AHUD_Game::IsWidgetVisible(const EWidgetEnum& _widget) const
{
	if (!widgets.Contains(_widget)) return false;
	return widgets[_widget]->GetVisibility() == ESlateVisibility::Visible;
}

AHUD_Game::AHUD_Game()
{
	widgetTypes =
	{
		{ WIDGET_CINEMATIC, nullptr},
		{ WIDGET_PLAYER, nullptr },
		{ WIDGET_BOSS, nullptr },
		{ WIDGET_PAUSE, nullptr },
		{ WIDGET_CHEST_SPAM,nullptr},
		{ WIDGET_CHEST_QTE,nullptr},
		{ WIDGET_TUTO,nullptr},
		{ WIDGET_OPTIONS, nullptr },
		{ WIDGET_CALIBRATION, nullptr },
		{ WIDGET_DIFFICULTY, nullptr },
		{ WIDGET_EXTRA, nullptr },
		{ WIDGET_CONFIRM, nullptr },
		{ WIDGET_LOADING, nullptr },
		{ WIDGET_ELEVATOR, nullptr },
		{ WIDGET_WARNINGS_ZONE, nullptr },
		{ WIDGET_POPUP, nullptr },
		{ WIDGET_BOSS_RUSH, nullptr},
		{ WIDGET_CREDIT, nullptr}
	};
}

void AHUD_Game::InitializeHUD()
{
	Super::InitializeHUD();
#if !WITH_EDITOR
	if (const UWorld* _world = GetWorld())
	{
		gameMode = _world->GetAuthGameMode<AM_GameMode>();
		if (gameMode)
		{
			if (ALevelLoadingManager* _llm = gameMode->GetLevelLoadingManager())
			{
				if (APlayerController* _player = _world->GetFirstPlayerController())
				{
					_player->SetInputMode(FInputModeUIOnly());
					ShowLoadScreen();
					_llm->OnFinishedLoading().AddUniqueDynamic(this,&AHUD_Game::HideLoadScreen);
					ON_SCREEN_LOG_TIME(15, FColor::Green, "HUD -> Sub to On Finished Loading");	
				}
			}
		}
	}
#endif
#if WITH_EDITOR
	gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>();
#endif
}

void AHUD_Game::InitializeInput()
{
	Super::InitializeInput();
	if (!InputComponent) return; 
	const FGameInput& _input = FGameInput();
	InputComponent->BindAction(_input.PauseGameAction.ActionName, IE_Pressed, this, &AHUD_Game::MenuPause);
}

void AHUD_Game::ShowInitialWidget()
{
	ShowGameWidget(WIDGET_PLAYER);
	playerController->SetInputMode(FInputModeGameOnly());
}

void AHUD_Game::MenuPause()
{
	if (!CanPause()) return;
	
	if (bMenuPause)
	{
		onGameUnPause.Broadcast();
		HideMenuPause();
	}
	else
	{
		OnPause();
		onGamePause.Broadcast();
		ShowMenuPause();
		if (gameMode)
			gameMode->GetCinematicManager()->EndHold();
	}
}

void AHUD_Game::ShowBossWidget()
{
	ShowGameWidget(WIDGET_BOSS);
}
void AHUD_Game::HideBossWidget()
{
	HideGameWidget(WIDGET_BOSS);		
}

UUI_Widget* AHUD_Game::ShowGameWidget(const EWidgetEnum& _widgetEnum)
{
	if (!widgets.Contains(_widgetEnum)) return nullptr;

	if (gameWidgetMap.Contains(_widgetEnum))
		return gameWidgetMap[_widgetEnum];
	
	UUI_Widget* _widget = widgets[_widgetEnum];
	_widget->SetVisibility(ESlateVisibility::Visible);
	gameWidgetMap.Add(_widgetEnum, _widget);
	return _widget;
}
void AHUD_Game::HideGameWidget(const EWidgetEnum& _widgetEnum)
{
	if (!gameWidgetMap.Contains(_widgetEnum)) return;
	UUI_Widget* _widget = gameWidgetMap.FindAndRemoveChecked(_widgetEnum);
	_widget->SetVisibility(ESlateVisibility::Hidden);
}

void AHUD_Game::ShowCredits()
{
	HideGameWidgets();
	ShowMenuWidget(WIDGET_CREDIT);
	if (UUI_CreditWidget* _credit = GetWidget<UUI_CreditWidget>(WIDGET_CREDIT))
	{
		_credit->OnBack.AddUniqueDynamic(this, &AHUD_Game::ReturnToMenu);		
		_credit->OnCreditFinished().AddUniqueDynamic(this, &AHUD_Game::ReturnToMenu);		
	}

	FInputModeGameAndUI _inputMode = FInputModeGameAndUI();
	_inputMode.SetHideCursorDuringCapture(false);
	playerController->SetInputMode(_inputMode);
	playerController->SetShowMouseCursor(true);
	LockPlayer();
}

void AHUD_Game::ShowMenuPause()
{
	bMenuPause = true;
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
	HideGameWidgets();
	ShowMenuWidget(WIDGET_PAUSE);

	FInputModeGameAndUI _inputMode = FInputModeGameAndUI();
	_inputMode.SetHideCursorDuringCapture(false);
	playerController->SetInputMode(_inputMode);
	playerController->SetShowMouseCursor(true);
}
void AHUD_Game::HideMenuPause()
{
	if (!widgets.Contains(WIDGET_PAUSE)) return;
	
	if (visibleMenuWidget == widgets[WIDGET_PAUSE])
	{
		bMenuPause = false;
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		GoBackMenu();
		ShowGameWidgets();

		playerController->SetInputMode(FInputModeGameOnly());
		playerController->SetShowMouseCursor(false);
	}
}

void AHUD_Game::LockPlayer() const
{
	if (const AM_GameMode* _gm = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if (AIU_Player* _player = _gm->GetPlayer())
		{
			_player->DisableInput(playerController);
			_player->GetCharacterAttackComponent()->CheckHoldingTime();
			playerController->FlushPressedKeys();
		}
	}
}

void AHUD_Game::ShowLoadScreen()
{
	if (!widgets.Contains(WIDGET_LOADING)) return;

	HideGameWidgets();
	widgets[WIDGET_LOADING]->SetVisibility(ESlateVisibility::Visible);
}
void AHUD_Game::HideLoadScreen()
{
	if (!widgets.Contains(WIDGET_LOADING)) return;

	widgets[WIDGET_LOADING]->SetVisibility(ESlateVisibility::Hidden);
	ShowGameWidgets();
	if (const UWorld* _world = GetWorld())
	{
		if (const AM_GameMode* _gm = _world->GetAuthGameMode<AM_GameMode>())
		{
			if (ALevelLoadingManager* _llm = _gm->GetLevelLoadingManager())
			{
				if (APlayerController* _player = _world->GetFirstPlayerController())
				{
					_player->SetInputMode(FInputModeGameOnly());
					_llm->OnFinishedLoading().RemoveDynamic(this,&AHUD_Game::HideLoadScreen);
				}
			}
		}
	}
}

void AHUD_Game::ShowCinematicWidget()
{
	if (!widgets.Contains(WIDGET_CINEMATIC)) return;
	
	HideGameWidgets();
	widgets[WIDGET_CINEMATIC]->SetVisibility(ESlateVisibility::Visible);
}
void AHUD_Game::HideCinematicWidget()
{
	if (!widgets.Contains(WIDGET_CINEMATIC)) return;

	widgets[WIDGET_CINEMATIC]->SetVisibility(ESlateVisibility::Hidden);
	ShowGameWidgets();
}

void AHUD_Game::ReturnToMenu()
{
	if (UM_GameInstance* _gi = GetGameInstance<UM_GameInstance>())
		_gi->ReturnToMainMenu();
}

void AHUD_Game::ShowGameWidgets() const
{
	for (const auto& current : gameWidgetMap)
		current.Value->SetVisibility(ESlateVisibility::Visible);
}
void AHUD_Game::HideGameWidgets() const
{
	for (const auto& current : gameWidgetMap)
		current.Value->SetVisibility(ESlateVisibility::Hidden);
}

void AHUD_Game::GoBackAllMenuWidget()
{
	while (menuWidgetHeap.Num() > 1)
	{
		GoBackMenu();
	}

	MenuPause();
}