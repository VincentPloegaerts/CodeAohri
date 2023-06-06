#pragma once
#include "STRUCT_ProjectileSimulatedSettings.generated.h"

USTRUCT()
struct FProjectileSimulatedSettings
{
	GENERATED_BODY()
	
	//GravityScale is the force that will attract the projectile down, used random value between those two Min & Max
	UPROPERTY(EditAnywhere, Category = "Projectile | Simulated", meta = (UIMin = "0", UIMax = "1000", ClampMin = "0", ClampMax = "1000"))
		float fGravityScaleMin = 25.0f;
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Simulated", meta = (UIMin = "0", UIMax = "1000", ClampMin = "0", ClampMax = "1000"))
		float fGravityScaleMax = 50.0f;
};