#include "UI_NewSaveMenuWidget.h"

#include "M_GameInstance.h"

#include "HUD_Main.h"
#include "UI_ConfirmWidget.h"
#include "UI_GameSaveWidget.h"
#include "UI_DifficultyMenuWidget.h"

#include "S_GameSave.h"

#include "IU_Macros.h"

void UUI_NewSaveMenuWidget::SelectGame(const int& _gameIndex)
{
	if (!gameInstance) return;

	iGameIndex = _gameIndex;
	bSaveExist = gameInstance->DoesGameSaveExist(iGameIndex);

	if (bSaveExist)
		AskSelectGame(crushGameSaveTitle, crushGameSaveText);
	else
		AskSelectGame(createNewGameTitle, createNewGameText);
}

void UUI_NewSaveMenuWidget::AskSelectGame(const FString& _title, const FString& _text)
{
	if (UUI_ConfirmWidget* _widget = hud->GetConfirmWidget())
	{
		_widget->InitializeConfirmBox(_title, _text);
		_widget->OnConfirm().AddUniqueDynamic(this, &UUI_NewSaveMenuWidget::ConfirmSelectGame);
	}
	else
		ConfirmSelectGame();
}

void UUI_NewSaveMenuWidget::ConfirmSelectGame()
{
	gameInstance->CreateGameSave(iGameIndex);
	
	if (UUI_DifficultyMenuWidget* _widget = hud->GetDifficultyWidget())
		_widget->OnDifficultySelected().AddUniqueDynamic(this, &UUI_NewSaveMenuWidget::InitializeNewSave);
}

void UUI_NewSaveMenuWidget::InitializeNewSave(const EDifficultyType& _difficulty)
{
	if (!gameInstance) return;

	gameInstance->ChangeLevel(GAME_LEVEL);
}

void UUI_NewSaveMenuWidget::InitializeGameSaveWidget(UUI_GameSaveWidget* _widget, const int& _index, const US_GameSave* _gameSave)
{
	_widget->InitializeGameSaveWidget(_index, _gameSave);
}

void UUI_NewSaveMenuWidget::Back()
{
	if (gameInstance->DoesGameSaveExist(iGameIndex) && !bSaveExist)	//If A Game Save have been created on an Empty Game Slot -> Clear Game Save Slot | Avoid GameSave deletion if a GameSave Already exist 
		gameInstance->DeleteGameSave(iGameIndex);

	if (hud)
		if (UUI_DifficultyMenuWidget* _widget = hud->GetWidget<UUI_DifficultyMenuWidget>(WIDGET_DIFFICULTY))
			_widget->OnDifficultySelected().RemoveDynamic(this, &UUI_NewSaveMenuWidget::InitializeNewSave);

	iGameIndex = -1;
	Super::Back();
}