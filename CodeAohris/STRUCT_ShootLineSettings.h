#pragma once
#include "ENUM_Plan.h"

#include "STRUCT_ShootLineSettings.generated.h"

USTRUCT(BlueprintType)
struct FShootLineSettings
{
	GENERATED_BODY()

	/*Length of the projectile line (repartition of projectile depends on iSpawnAtOnce).*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Line", meta = (UIMin = "100", UIMax = "50000", ClampMin = "100", ClampMax = "50000"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fLength = 1000;

	/*If the line is created on Vertical or Horizontal axis.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Line", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		TEnumAsByte<EPlan> ePlan = EPlan::VERTICAL;
};