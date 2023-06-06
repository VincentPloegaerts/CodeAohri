#pragma once
#include "ENUM_Plan.h"
#include "STRUCT_ShootRandom2DSettings.generated.h"

USTRUCT(BlueprintType)
struct FShootRandom2DSettings
{
	GENERATED_BODY()

	/*Plan of the randomization (refer to forward of owner).*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Random 2D", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TEnumAsByte<EPlan> ePlan = EPlan::HORIZONTAL;
	
	/*Total angle range for the direction randomization.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings | Random 2D", meta = (UIMin = "0", UIMax = "360", ClampMin = "0", ClampMax = "360"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fAngle = 45.0f;
};