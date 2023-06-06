#pragma once
#include "STRUCT_ProjectileSinusoidalSettings.generated.h"

USTRUCT(BlueprintType)
struct FProjectileSinusoidalSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Sinusoidal", meta = (UIMin = "100", UIMax = "10000", ClampMin = "100", ClampMax = "10000"))
		float fPeriodRange = 500.0f;
	
	//Amplitude of the curve (Up & Down)
	UPROPERTY(EditAnywhere, Category = "Projectile | Sinusoidal", meta = (UIMin = "0", UIMax = "10000", ClampMin = "0", ClampMax = "10000"))
		float fAmplitude = 200.0f;
	
	//Begin of the projectile on the Sinusoidal curve
	UPROPERTY(EditAnywhere, Category = "Projectile | Sinusoidal", meta = (UIMin = "0", UIMax = "1", ClampMin = "0", ClampMax = "1"))
		float fStartGap = 0.0f;

	//Frequency of the curve 
	UPROPERTY(EditAnywhere, Category = "Projectile | Sinusoidal")
		FRotator localRotation = FRotator(0.0f);
};