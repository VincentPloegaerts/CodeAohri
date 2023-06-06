#include "UI_LoadSaveMenuWidget.h"

#include "ENUM_LevelType.h"
#include "HUD_Main.h"
#include "M_GameInstance.h"
#include "UI_GameSaveWidget.h"
#include "UI_PopUpWidgetWidget.h"

#include "IU_Macros.h"
#include "UI_ConfirmWidget.h"

void UUI_LoadSaveMenuWidget::SelectGame(const int& _gameIndex)
{
	if (!gameInstance) return;

	iGameSave = _gameIndex;
	
	if (gameInstance->DoesGameSaveExist(iGameSave))
	{
		if (UUI_ConfirmWidget* _widget = hud->GetConfirmWidget())
		{
			_widget->OnConfirm().AddUniqueDynamic(this, &UUI_LoadSaveMenuWidget::ConfirmSelectGame);
			_widget->InitializeConfirmBox(FString::Printf(TEXT("%s : %d"), *confirmTitle, iGameSave), confirmText);
		}
	}
	else
	{
		if (const UUI_PopUpWidgetWidget* _widget = hud->GetPopUpWidgetWidget())
		{
			_widget->InitializePopUpWidget(popupTitle, popupText);
		}
	}
}

void UUI_LoadSaveMenuWidget::ConfirmSelectGame()
{
	if (!gameInstance) return;

	if (gameInstance->DoesGameSaveExist(iGameSave))
	{
		gameInstance->LoadGameSave(iGameSave);
	
		gameInstance->ChangeLevel(GAME_LEVEL);
	}
}

void UUI_LoadSaveMenuWidget::InitializeGameSaveWidget(UUI_GameSaveWidget* _widget, const int& _index, const US_GameSave* _gameSave)
{
	_widget->InitializeGameSaveWidget(_index, _gameSave, true);
}
