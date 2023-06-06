#include "UI_GameInputActionMappingWidget.h"

#include "IU_Macros.h"

void UUI_GameInputActionMappingWidget::InitializeActionMappingWidget(UM_InputManager* _inputManager, const FInputActionKeyMapping& _actionMapping)
{
	InputManager = _inputManager;
	if (!InputManager) return;
	
	InputActionKeyMapping = _actionMapping;
	AllowGamepadKey = InputActionKeyMapping.Key.IsGamepadKey();

	MappingIKS->SetSelectedKey(InputActionKeyMapping.Key);

	UpdateKeyVisual();
}

void UUI_GameInputActionMappingWidget::RebindKey(FInputChord _key)
{
	if (!InputManager) return;
	
	if (InputActionKeyMapping.Key == _key.Key)
	{
		UpdateKeyVisual();
		return;
	}
	
	if (!_key.Key.IsValid())													//Avoid Right Click on Input Key Selector (Clear Key)
	{
		MappingIKS->SetSelectedKey(InputActionKeyMapping.Key);
		UpdateKeyVisual();
		return;
	}
	
	if (AllowGamepadKey != _key.Key.IsGamepadKey())								//Avoid Gamepad Keys in Keyboard Key mappings and Keyboard Keys in Gamepad key mappings
	{
		MappingIKS->SetSelectedKey(InputActionKeyMapping.Key);
		const FString& _keyTypeText = _key.Key.IsGamepadKey() ? "Gamepad " : "Keyboard ";
		onRebindKeyFailed.Broadcast(_key.Key, _keyTypeText + FailedKeyTypeReasonText);
		UpdateKeyVisual();
		return;
	}
	
	FInputActionKeyMapping _newMapping = InputActionKeyMapping;
	_newMapping.Key = _key.Key;
	
	if (InputManager->RebindAction(InputActionKeyMapping.ActionName, InputActionKeyMapping.Key, _newMapping.Key))
	{
		InputActionKeyMapping = _newMapping;
	}
	else
	{
		MappingIKS->SetSelectedKey(InputActionKeyMapping.Key);
		onRebindKeyFailed.Broadcast(_key.Key, FailedKeyUsedReasonText);
	}

	UpdateKeyVisual();
}