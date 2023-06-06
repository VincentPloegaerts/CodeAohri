#include "UI_TutoElementWidget.h"

#include "M_GameInstance.h"
#include "M_GameMode.h"

#include "M_InputManager.h"

#include "HUD_Main.h"
#include "IU_Macros.h"

void UUI_TutoElementWidget::InitializeWidget()
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

TArray<UUI_GameInputInfoWidget*> UUI_TutoElementWidget::CreateWidgetsFromAction(const FInputActionData& _inputAction)
{
	TArray<UUI_GameInputInfoWidget*> _widgets = { };
	if (GameInputInfoWidgetType.Get())
	{ 
		for (const FInputActionKeyMapping& _mapping : _inputAction.Mappings)
		{
			UUI_GameInputInfoWidget* _widget = CreateWidget<UUI_GameInputInfoWidget>(GetWorld(), GameInputInfoWidgetType);
			_widget->InitializeInputInfoWidget(_mapping.Key);
			_widgets.Add(_widget);
		}
	}
	InputInfoWidgets.Append(_widgets);
	UpdateWidgetsVisual();

	return _widgets;
}

TArray<UUI_GameInputInfoWidget*> UUI_TutoElementWidget::CreateWidgetsFromAxis(const FInputAxisData& _inputAxis)
{
	TArray<UUI_GameInputInfoWidget*> _widgets = { };
	if (GameInputInfoWidgetType.Get())
	{
		for (const FInputAxisKeyMapping& _mapping : _inputAxis.Mappings)
		{
			UUI_GameInputInfoWidget* _widget = CreateWidget<UUI_GameInputInfoWidget>(GetWorld(), GameInputInfoWidgetType);
			_widget->InitializeInputInfoWidget(_mapping.Key);
			_widgets.Add(_widget);
		}
	}
	InputInfoWidgets.Append(_widgets);
	UpdateWidgetsVisual();
	
	return _widgets;
}

void UUI_TutoElementWidget::ClearWidgets()
{
	for (UUI_GameInputInfoWidget* _widget : InputInfoWidgets)
		_widget->RemoveFromParent();
	
	InputInfoWidgets.Empty();
}

void UUI_TutoElementWidget::InitializeKeyboardVisual()
{
	for (UUI_GameInputInfoWidget* _widget : InputInfoWidgets)
		_widget->SetVisibility(_widget->GetKey().IsGamepadKey() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UUI_TutoElementWidget::InitializeGamepadVisual()
{
	for (UUI_GameInputInfoWidget* _widget : InputInfoWidgets)
		_widget->SetVisibility(_widget->GetKey().IsGamepadKey() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UUI_TutoElementWidget::UpdateWidgetsVisual()
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
