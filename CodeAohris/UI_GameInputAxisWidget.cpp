#include "UI_GameInputAxisWidget.h"

#include "Components/HorizontalBoxSlot.h"

void UUI_GameInputAxisWidget::InitializeGameInputAxisWidget(UM_InputManager* _inputManager, const FInputAxisData& _axisInput)
{
	InputManager = _inputManager;
	if (!InputManager) return;

	InputManager->OnRebindableInputAxisReset().AddUniqueDynamic(this, &UUI_GameInputAxisWidget::OnResetInputAxis);
	InputAxisData = _axisInput;
	InputNameText->SetText(FText::FromString(InputAxisData.AxisName.ToString() + " : "));

	InitializeAxisMappingWidgets();
}

void UUI_GameInputAxisWidget::InitializeAxisMappingWidgets()
{
	if (AxisMappingWidgetType.Get())
	{
		GameInputGP->ClearChildren();
		int _column = 1;
		for (const FInputAxisKeyMapping& _mapping : InputAxisData.Mappings)
		{
			UUI_GameInputAxisMappingWidget* _widget = CreateWidget<UUI_GameInputAxisMappingWidget>(GetWorld(), AxisMappingWidgetType);
			_widget->InitializeAxisMappingWidget(InputManager, _mapping);
			_widget->OnRebindKeyFailed().AddUniqueDynamic(this, &UUI_GameInputAxisWidget::RebindAxisFailed);
			if (_mapping.Key.IsGamepadKey())
				_column = 0;
			GameInputGP->AddChildToGrid(_widget, 0, _column);
			_column++;
		}
	}
}

void UUI_GameInputAxisWidget::OnResetInputAxis(const TArray<FInputAxisData>& _axisInputs)
{
	for (const FInputAxisData& _axisInput : _axisInputs)
	{
		if (_axisInput.AxisName == InputAxisData.AxisName)
		{
			InputAxisData = _axisInput;
			InitializeAxisMappingWidgets();
			return;
		}
	}
}

void UUI_GameInputAxisWidget::RebindAxisFailed(FKey _failedKey, FString _reason)
{
	RebindInputFailed(InputAxisData.AxisName, _failedKey, _reason);
}
