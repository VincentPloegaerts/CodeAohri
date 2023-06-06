#include "UI_GameInputAxisMappingWidget.h"

#include "IU_Macros.h"

void UUI_GameInputAxisMappingWidget::InitializeAxisMappingWidget(UM_InputManager* _inputManager, const FInputAxisKeyMapping& _axisMapping)
{
	InputManager = _inputManager;
	if (!InputManager) return;
	
	//InputManager->OnInputReset().AddUniqueDynamic(this, &UUI_GameInputAxisMappingWidget::UpdateKeysOnReset);
	InputAxisMapping = _axisMapping;
	AllowGamepadKey = InputAxisMapping.Key.IsGamepadKey();
	
	MappingIKS->SetSelectedKey(InputAxisMapping.Key);

	UpdateKeyVisual();
}

void UUI_GameInputAxisMappingWidget::RebindKey(FInputChord _key)
{
	if (!InputManager) return;
	
	if (InputAxisMapping.Key == _key.Key)
	{
		UpdateKeyVisual();
		return;
	}
	
	if (!_key.Key.IsValid())													//Avoid Right Click on Input Key Selector (Clear Key)
	{
		MappingIKS->SetSelectedKey(InputAxisMapping.Key);
		return;
	}

	if (AllowGamepadKey != _key.Key.IsGamepadKey())			//Avoid Gamepad Keys in Keyboard Key mappings and Keyboard Keys in Gamepad key mappings
	{
		MappingIKS->SetSelectedKey(InputAxisMapping.Key);
		const FString& _keyTypeText = _key.Key.IsGamepadKey() ? "Gamepad " : "Keyboard ";
		onRebindKeyFailed.Broadcast(_key.Key, _keyTypeText + FailedKeyTypeReasonText);
		UpdateKeyVisual();
		return;
	}

	if (_key.Key.IsGamepadKey())												//Block Gamepad Axis Rebind 
	{
		MappingIKS->SetSelectedKey(InputAxisMapping.Key);
		onRebindKeyFailed.Broadcast(_key.Key, FailedKeyGamepadReasonText);
		UpdateKeyVisual();
		return;
	}
	
	FInputAxisKeyMapping _newMapping = InputAxisMapping;
	_newMapping.Key = _key.Key;

	if (InputManager->RebindAxis(InputAxisMapping.AxisName, InputAxisMapping.Key, _newMapping.Key))
	{
		InputAxisMapping = _newMapping;
	}
	else
	{
		MappingIKS->SetSelectedKey(InputAxisMapping.Key);
		onRebindKeyFailed.Broadcast(_key.Key, FailedKeyUsedReasonText);
	}

	UpdateKeyVisual();
}
