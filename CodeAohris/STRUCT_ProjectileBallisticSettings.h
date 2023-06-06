#pragma once
#include "STRUCT_ProjectileBallisticSettings.generated.h"

USTRUCT(BlueprintType)
struct FProjectileBallisticSettings
{
	GENERATED_BODY()
	
	//This offset allow the projectile to go up (the higher the value is, the higher the projectile will go)
	UPROPERTY(EditAnywhere, Category = "Projectile | Ballistic", meta = (UIMin = "0.0", UIMax = "10000.0", ClampMin = "0.0", ClampMax = "10000.0"))
		float fOffsetZ = 200.0f;
	
	//Max range of the projectile
	UPROPERTY(EditAnywhere, Category = "Projectile | Ballistic", meta = (UIMin = "0.0", UIMax = "10000.0", ClampMin = "0.0", ClampMax = "10000.0"), DisplayName = "Range")
		float fRangeBallistic = 700.0f;
};