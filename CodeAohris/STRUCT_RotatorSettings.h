#pragma once
#include "STRUCT_RotatorSettings.generated.h"

/*Struct used for rotating around an axis.*/
USTRUCT(BlueprintType)
struct FRotatorSettings
{
	GENERATED_BODY()

	/*Inverse the rotation.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Rotator", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bClockWise = false;

	/*Rotate around the Yaw (UP) axis.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Rotator", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bYawRotation = true;

	/*Rotate around the Pitch (FORWARD) axis.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Rotator", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bPitchRotation = false;

	/*Rotate around the Roll (RIGHT) axis.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Rotator", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bRollRotation = false;
};