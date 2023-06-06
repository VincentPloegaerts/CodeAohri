#pragma once

#include "STRUCT_PlayerAccessibility.generated.h"

USTRUCT(BlueprintType)
struct FPlayerAccessibility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Save | Accessibility")
	EColorVisionDeficiency colorblindDeficiency = EColorVisionDeficiency::NormalVision;  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Save | Accessibility", meta = (ClampMin = 0, ClampMax = 1))
	float colorblindSeverity = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Save | Accessibility")
	bool motionBlurEnable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Save | Accessibility", meta = (ClampMin = 0, ClampMax = 1))
	float motionBlurValue = 0.5f;
};
