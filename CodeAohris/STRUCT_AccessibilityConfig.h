#pragma once

#include "STRUCT_AccessibilityConfig.generated.h"

USTRUCT(BlueprintType)
struct FAccessibilityConfig
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Save | Accessibility")
	EColorVisionDeficiency colorVisionDeficiency = EColorVisionDeficiency::NormalVision;  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Save | Accessibility", meta = (ClampMin = 0, ClampMax = 1))
	float colorVisionDeficiencySeverity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Save | Accessibility")
	bool displayFPS = true;

public:
	FORCEINLINE const EColorVisionDeficiency& GetColorVisionDeficiency() const { return colorVisionDeficiency; }
	FORCEINLINE const float& GetColorVisionDeficiencySeverity() const { return colorVisionDeficiencySeverity; }

	FORCEINLINE const bool& GetDisplayFPS() const { return displayFPS; }
	
	FORCEINLINE void SetColorVisionDeficiency(const EColorVisionDeficiency& _colorVisionDeficiency) { colorVisionDeficiency = _colorVisionDeficiency; }
	FORCEINLINE void SetColorVisionDeficiencySeverity(const float& _severity) { colorVisionDeficiencySeverity = _severity; }

	FORCEINLINE void SetDisplayFPS(const bool& _enable) { displayFPS = _enable; }
};
