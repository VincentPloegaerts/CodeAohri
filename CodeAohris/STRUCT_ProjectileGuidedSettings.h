#pragma once
#include "STRUCT_ProjectileGuidedSettings.generated.h"

USTRUCT(BlueprintType)
struct FProjectileGuidedSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Guided", meta = (UIMin = "0.0", UIMax = "10000.0", ClampMin = "0.0", ClampMax = "10000.0"))
		float fProjectileGuidedSpeed = 2500.0f;
	
	//Rotation speed of the Guided Projectile to look at the target
	UPROPERTY(EditAnywhere, Category = "Projectile | Guided", meta = (UIMin = "0.0", UIMax = "5000.0", ClampMin = "0.0", ClampMax = "5000.0"))
		float fRotationSpeed = 50.0f;
	
	//Range before the Guided will look at target, once at range will go forward
	UPROPERTY(EditAnywhere, Category = "Projectile | Guided", meta = (UIMin = "0.0", UIMax = "2000.0", ClampMin = "0.0", ClampMax = "2000.0"), DisplayName = "Range")
		float fRangeGuided = 150.0f;
	
	//Time before look at target
	UPROPERTY(EditAnywhere, Category = "Projectile | Guided", meta = (UIMin = "0.0", UIMax = "25.0", ClampMin = "0.0", ClampMax = "25.0"))
		float fWaitTime = 0.1f;
};