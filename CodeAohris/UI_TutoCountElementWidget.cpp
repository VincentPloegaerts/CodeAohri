#include "UI_TutoCountElementWidget.h"

#include "M_GameInstance.h"
#include "M_GameMode.h"

#include "M_InputManager.h"

#include "HUD_Main.h"

void UUI_TutoCountElementWidget::InitializeWidget()
{
	if (AHUD_Main* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>())
	{
		_hud->OnGamepadKey().AddUniqueDynamic(this, &UUI_TutoElementWidget::InitializeGamepadVisual);
		_hud->OnKeyboardKey().AddUniqueDynamic(this, &UUI_TutoElementWidget::InitializeKeyboardVisual);
	}
	
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
		if (const UM_InputManager* _inputManager = _gi->GetInputManager())
			OnGameInput(_inputManager->GetGameInput());
}

void UUI_TutoCountElementWidget::InitializeKeyboardVisual()
{
	Super::InitializeKeyboardVisual();
}

void UUI_TutoCountElementWidget::InitializeGamepadVisual()
{
	Super::InitializeGamepadVisual();
}


void UUI_TutoCountElementWidget::UpdateWidgetsVisual()
{
	if (const APlayerController* _controller = GetWorld()->GetFirstPlayerController())
	{
		if (const AHUD_Main* _hud = _controller->GetHUD<AHUD_Main>())
		{
			if (_hud->IsGamepadVisual())
				InitializeGamepadVisual();
			else
				InitializeKeyboardVisual();
		}
	}
}

void UUI_TutoCountElementWidget::UpdateCurrentCount(float _newCount)
{
	onCountUpdate.Broadcast();
	currentCount = _newCount;
	//LOG("Update Current Count in %s with %d", *GetName(), currentCount)
}

void UUI_TutoCountElementWidget::ResetCurrentCount()
{
	//LOG("Reset in %s", *GetName())
	currentCount = 0;
}

