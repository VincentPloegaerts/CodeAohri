#pragma once
#include "ENUM_AxisMode.h"

#include "STRUCT_InstantSettings.generated.h"

/*Struct used to move instantly an object.*/
USTRUCT(BlueprintType)
struct FInstantSettings
{
	GENERATED_BODY()

	/*The angle rotation.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Instant", meta = (UIMin = "-360", UIMax = "360", ClampMin = "-360", ClampMax = "360"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fRotation = 0;

	/*Witch axis the object turn around.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Instant", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TEnumAsByte<EAxisMode> eAxis = EAxisMode::UP_AXIS;
};