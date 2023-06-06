#pragma once
#include "STRUCT_ShootCrossSettings.generated.h"

USTRUCT(BlueprintType)
struct FShootCrossSettings
{
	GENERATED_BODY()

	/*Bool to set the rotation of projectile in cross rotation or pattern forward*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Cross", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bKeepPatternForward = true;
	/*Global Radius of the Cross Arc.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Cross", meta = (UIMin = "0", UIMax = "50000", ClampMin = "0", ClampMax = "50000"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fRadius = 0;

	/*Angle of projectile repartition on the Arc X of the Cross.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Cross", meta = (UIMin = "0", UIMax = "360", ClampMin = "0", ClampMax = "360"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fAngleX = 0;
	/*Angle of projectile repartition on the Arc Y of the Cross.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Cross", meta = (UIMin = "0", UIMax = "360", ClampMin = "0", ClampMax = "360"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fAngleY = 0;
	/*Amount of projectile on the Arc X of the Cross.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Cross", meta = (UIMin = "0", UIMax = "500", ClampMin = "0", ClampMax = "500"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	int iProjectileOnX = 0;
	/*Amount of projectile on the Arc X of the Cross.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Cross", meta = (UIMin = "0", UIMax = "500", ClampMin = "0", ClampMax = "500"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	int iProjectileOnY = 0;
};
