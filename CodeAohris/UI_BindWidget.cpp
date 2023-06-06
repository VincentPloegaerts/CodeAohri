#include "UI_BindWidget.h"

void UUI_BindWidget::InitializeWidget()
{
	keyboardIK->OnKeySelected.AddUniqueDynamic(this, &UUI_BindWidget::KeyboardInputKeyChanged);
	gamepadIK->OnKeySelected.AddUniqueDynamic(this, &UUI_BindWidget::GamepadInputKeyChanged);
}

void UUI_BindWidget::KeyboardInputKeyChanged(FInputChord _input)
{
	const FKey& _key = _input.Key;
	if (!IsValidKey(_key, true)) return;
	onBindKeyChanged.Execute(_key);
}
void UUI_BindWidget::GamepadInputKeyChanged(FInputChord _input)
{
	const FKey& _key = _input.Key;
	if (IsValidKey(_key, true)) return;
	onBindKeyChanged.Execute(_key);
}