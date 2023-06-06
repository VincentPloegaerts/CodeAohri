#include "UI_OptionCalibrationWidget.h"

#include "M_GameInstance.h"
#include "S_PlayerSave.h"

void UUI_OptionCalibrationWidget::SetVisibility(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
		ResetBrightness();
	
	Super::SetVisibility(InVisibility);
}

void UUI_OptionCalibrationWidget::InitializeWidget()
{
	Super::InitializeWidget();
	brightnessSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionCalibrationWidget::OnBrightnessSliderChanged);
	
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
		playerSave = _gi->GetPlayerSave();
	
	ResetBrightness();
}

void UUI_OptionCalibrationWidget::InitializeFocus()
{
	brightnessSlider->SetKeyboardFocus();
}

void UUI_OptionCalibrationWidget::OnBrightnessSliderChanged(float _value)
{
	_value = FMath::Clamp(_value, 0.01f, 1.0f);
	const float& _brightness = FMath::Lerp(fMinBrightness, fMaxBrightness, _value);
	BrightnessChanged(_brightness);

	ApplyBrightness();
}
void UUI_OptionCalibrationWidget::ApplyBrightness()
{
	onBrightnessUpdated.Broadcast(FMath::Lerp(fMinBrightness, fMaxBrightness, brightnessSlider->GetValue()));
}

void UUI_OptionCalibrationWidget::ResetBrightness()
{
	if (playerSave)
	{
		const float& _brightness = playerSave->GetGraphicConfig().GetExposureValue();
		const float& _value = (_brightness - fMinBrightness) / (fMaxBrightness - fMinBrightness);
		brightnessSlider->SetValue(_value);
		BrightnessChanged(_brightness);
	}
}

void UUI_OptionCalibrationWidget::BrightnessChanged(float _brightness)
{
	//...	
}
