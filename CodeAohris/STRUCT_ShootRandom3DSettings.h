#pragma once
#include "STRUCT_ShootRandom3DSettings.generated.h"

USTRUCT(BlueprintType)
struct FShootRandom3DSettings
{
	GENERATED_BODY()

	/*Total angle range for direction randomization (randomization in cone - forward the owner).*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Random 3D", meta = (UIMin = "0", UIMax = "360", ClampMin = "0", ClampMax = "360"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fAngle = 45.0f;
};