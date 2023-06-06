#pragma once
#include "STRUCT_TargetSettings.generated.h"

/*Struct used to rotate looking to a target.*/
USTRUCT(BlueprintType)
struct FTargetSettings
{
	GENERATED_BODY()

	/*Used smooth rotation.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Target", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bLerp = false;
	//Look exact player or only Yaw rotation to player
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Target", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bLookPitchPlayer = true;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Target", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bSpeedUpWhenTargetFar = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Target", BlueprintReadWrite, meta = (AllowPrivateAccess = True, ClampMin = "0"))
	float fBonusSpeed = 5.f;	
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Target", BlueprintReadWrite, meta = (AllowPrivateAccess = True, ClampMin = "0", ClampMax = "180"))
	float fRangeAngleMaxSpeed = 90.f;
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Target", BlueprintReadWrite, meta = (AllowPrivateAccess = True, ClampMin = "0", ClampMax = "180"))
	float fRangeAngleMinSpeed = 10.f;
};