#pragma once

#include "CoreMinimal.h"
#include "UI_OptionCalibrationWidget.h"

#include "Components/Image.h"

#include "UI_OptionCalibrationVisualWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_OptionCalibrationVisualWidget : public UUI_OptionCalibrationWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* CalibrationImage1 = nullptr;
	UPROPERTY(meta = (BindWidget))
	UImage* CalibrationImage2 = nullptr;
	UPROPERTY(meta = (BindWidget))
	UImage* CalibrationImage3 = nullptr;

private:
	virtual void BrightnessChanged(float _brightness) override;
};
