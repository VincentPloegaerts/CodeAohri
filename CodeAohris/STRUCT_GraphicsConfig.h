#pragma once

#include "STRUCT_GraphicsConfig.generated.h"

USTRUCT(BlueprintType)
struct FGraphicsConfig
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics Config", meta = (UIMin = "0", UIMax = "4", ClampMin = "0", ClampMax = "4"))
	int32 defaultScalability = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics Config")
	bool motionBlurEnable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics Config", meta = (ClampMin = 0, ClampMax = 1))
	float motionBlurValue = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics Config")
	float exposureValue = 10.5f;

	UPROPERTY()
	bool brightnessInitialized = false;
	
public:
	FORCEINLINE const int32& GetDefaultScalability() const { return defaultScalability; }

	FORCEINLINE const bool& GetMotionBlurEnable() const { return motionBlurEnable; }
	FORCEINLINE const float& GetMotionBlurValue() const { return motionBlurValue; }
	FORCEINLINE const float& GetExposureValue() const { return exposureValue; }

	FORCEINLINE void SetMotionBlurEnable(const bool& _enable) { motionBlurEnable = _enable; }
	FORCEINLINE void SetMotionBlurValue(const float& _value) { motionBlurValue = _value; }
	FORCEINLINE void SetExposureValue(const float& _value) { exposureValue = _value; }

	FORCEINLINE const bool& IsBrightnessInitialized() const { return brightnessInitialized; } 
	FORCEINLINE void SetBrightnessInitialized() { brightnessInitialized = true; }
};