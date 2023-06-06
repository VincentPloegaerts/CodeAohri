#pragma once
#include "STRUCT_ShootPointSettings.generated.h"

USTRUCT(BlueprintType)
struct FShootPointSettings
{
	GENERATED_BODY()

	/*Radius around the original spawn point to create an offset.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Point", meta = (UIMin = "0", UIMax = "10000", ClampMin = "0", ClampMax = "10000"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fRadius = 0;
};