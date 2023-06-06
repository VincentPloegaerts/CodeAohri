#pragma once
#include "STRUCT_ShootArcSettings.generated.h"

USTRUCT(BlueprintType)
struct FShootArcSettings
{
	GENERATED_BODY()

	/*Bool to set the rotation of projectile in arc rotation or pattern forward*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Arc", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bKeepPatternForward = true;
	/*Global Radius of the Arc.*/
	UPROPERTY(EditAnywhere,  Category = "Behavior | Shoot | Settings | Arc", meta = (UIMin = "0", UIMax = "50000", ClampMin = "0", ClampMax = "50000"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fRadius = 0;

	/*Angle of the Arc, projectile repartition depends on iSpawnAtOnce.*/
	UPROPERTY(EditAnywhere,  Category = "Behavior | Shoot | Settings | Arc", meta = (UIMin = "0", UIMax = "360", ClampMin = "0", ClampMax = "360"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fAngle = 0;
};