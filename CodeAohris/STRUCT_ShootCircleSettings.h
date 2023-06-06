#pragma once
#include "STRUCT_ShootCircleSettings.generated.h"

USTRUCT(BlueprintType)
struct FShootCircleSettings
{
	GENERATED_BODY()

	/*Radius of the Circle, repartition of projectile depends on iSpawnAtOnce.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Circle", meta = (UIMin = "0", UIMax = "50000", ClampMin = "0", ClampMax = "50000"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fRadius = 100;
};