#include "UI_OptionCalibrationVisualWidget.h"

void UUI_OptionCalibrationVisualWidget::BrightnessChanged(float _brightness)
{
	const float& _value = (_brightness - fMinBrightness) / (fMaxBrightness - fMinBrightness);

	const FLinearColor& _color = FLinearColor(_value, _value, _value, 1);
	CalibrationImage1->SetBrushTintColor(_color);
	CalibrationImage2->SetBrushTintColor(_color);
	CalibrationImage3->SetBrushTintColor(_color);
}
