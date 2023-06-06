#include "HUD_Menu.h"

#include "M_GameInstance.h"
#include "S_PlayerSave.h"

AHUD_Menu::AHUD_Menu()
{
	widgetTypes =
	{
		{ WIDGET_MAIN_MENU, nullptr },
		{ WIDGET_NEW_GAME, nullptr },
		{ WIDGET_LOAD_GAME, nullptr },
		{ WIDGET_DIFFICULTY, nullptr },
		{ WIDGET_OPTIONS, nullptr },
		{ WIDGET_CALIBRATION, nullptr },
		{ WIDGET_RUSH_MODE, nullptr },
		{ WIDGET_EXTRA, nullptr },
		{ WIDGET_CONFIRM, nullptr },
		{ WIDGET_LOADING, nullptr },
		{ WIDGET_POPUP, nullptr },
		{ WIDGET_CREDIT, nullptr}
	};
}

void AHUD_Menu::InitializeHUD()
{
	if (const UM_GameInstance* _gi = GetGameInstance<UM_GameInstance>())
	{
		if (US_PlayerSave* _save = _gi->GetPlayerSave())
		{
			if (!_save->GetGraphicConfig().IsBrightnessInitialized())
			{
				_save->EditGraphicConfig().SetBrightnessInitialized();
				ShowInitialWidget();
				ShowMenuWidget(WIDGET_CALIBRATION);
				return;
			}
		}
	}
	
	ShowInitialWidget();	
}

void AHUD_Menu::ShowInitialWidget()
{
	ShowMenuWidget(WIDGET_MAIN_MENU, true);
	FInputModeGameAndUI _inputMode = FInputModeGameAndUI();
	_inputMode.SetHideCursorDuringCapture(false);
	playerController->SetInputMode(_inputMode);
	playerController->SetShowMouseCursor(true);
}
