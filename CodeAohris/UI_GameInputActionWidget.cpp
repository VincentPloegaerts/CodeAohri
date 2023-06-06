#include "UI_GameInputActionWidget.h"

#include "Components/HorizontalBoxSlot.h"

void UUI_GameInputActionWidget::InitializeGameInputActionWidget(UM_InputManager* _inputManager, const FInputActionData& _actionInput)
{
	InputManager = _inputManager;
	if (!InputManager) return;

	InputManager->OnRebindableInputActionReset().AddUniqueDynamic(this, &UUI_GameInputActionWidget::OnResetInputAction);
	InputActionData = _actionInput;
	InputNameText->SetText(FText::FromString(InputActionData.ActionName.ToString() + " : "));

	InitializeActionMappingWidgets();
}

void UUI_GameInputActionWidget::InitializeActionMappingWidgets()
{
	if (ActionMappingWidgetType.Get())
	{
		GameInputGP->ClearChildren();
		int _column = 1;
		for (const FInputActionKeyMapping& _mapping : InputActionData.Mappings)
		{
			UUI_GameInputActionMappingWidget* _widget = CreateWidget<UUI_GameInputActionMappingWidget>(GetWorld(), ActionMappingWidgetType);
			_widget->InitializeActionMappingWidget(InputManager, _mapping);
			_widget->OnRebindKeyFailed().AddUniqueDynamic(this, &UUI_GameInputActionWidget::RebindActionFailed);
			if (_mapping.Key.IsGamepadKey())
				_column = 0;
			GameInputGP->AddChildToGrid(_widget, 0, _column);
			_column++;
		}
	}
}

void UUI_GameInputActionWidget::OnResetInputAction(const TArray<FInputActionData>& _actionInputs)
{
	for (const FInputActionData& _actionInput : _actionInputs)
	{
		if (_actionInput.ActionName == InputActionData.ActionName)
		{
			InputActionData = _actionInput;
			InitializeActionMappingWidgets();
			return;
		}
	}
}

void UUI_GameInputActionWidget::RebindActionFailed(FKey _failedKey, FString _reason)
{
	RebindInputFailed(InputActionData.ActionName, _failedKey, _reason);
}