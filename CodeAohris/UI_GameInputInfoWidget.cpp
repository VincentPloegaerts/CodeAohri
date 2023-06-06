#include "UI_GameInputInfoWidget.h"

void UUI_GameInputInfoWidget::InitializeInputInfoWidget(const FKey& _key)
{
	if (InputInfoDataTable)
	{
		const FInputInfo& _inputInfo = InputInfoDataTable->GetInputInfoForKey(_key);
		const bool _validInputTexture = _inputInfo.InputTexture != nullptr;

		UTexture2D* _backgroundTexture = _key.IsGamepadKey() ? GamepadBackgroundTexture : KeyboardBackgroundTexture; 
		const bool _validBackgroundTexture = _backgroundTexture != nullptr;
		
		InputBackgroundImage->SetBrushFromTexture(_backgroundTexture);
		InputBackgroundImage->SetOpacity(_validBackgroundTexture);
		
		InputImage->SetBrushFromTexture(_inputInfo.InputTexture);
		InputImage->SetOpacity(_validInputTexture);
		
		InputText->SetText(_inputInfo.InputDisplayName);
		InputText->SetOpacity(!_validInputTexture);
	}

	Key = _key;
}

void UUI_GameInputInfoWidget::ClearInputInfoWidget(const FString& _text) const
{
	InputImage->SetOpacity(0);
	InputText->SetText(FText::FromString(_text));
}
