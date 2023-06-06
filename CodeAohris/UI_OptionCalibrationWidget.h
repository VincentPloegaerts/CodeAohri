#pragma once

#include "CoreMinimal.h"
#include "UI_BackWidget.h"

#include "Components/Slider.h"

#include "UI_OptionCalibrationWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_OptionCalibrationWidget : public UUI_BackWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBrightnessUpdated, float, BrightnessChanged);
	
protected:
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ClampMin = "-14.9", ClampMax = "15"))
	float fMinBrightness = -14.9f;
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ClampMin = "-14.9", ClampMax = "15"))
	float fMaxBrightness = 15.0f;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* brightnessSlider = nullptr;

	UPROPERTY()
	FOnBrightnessUpdated onBrightnessUpdated;

	UPROPERTY()
	class US_PlayerSave* playerSave = nullptr;
	
public:
	FORCEINLINE FOnBrightnessUpdated& OnBrightnessUpdated() { return onBrightnessUpdated; }

	virtual void SetVisibility(ESlateVisibility InVisibility) override;

protected:
	virtual void InitializeWidget() override;
	virtual void InitializeFocus() override;

private:
	UFUNCTION() void OnBrightnessSliderChanged(float _value);	
	UFUNCTION() void ApplyBrightness();
	void ResetBrightness();
	
protected:
	virtual void BrightnessChanged(float _brightness);
};
