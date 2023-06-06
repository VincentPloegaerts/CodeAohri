#pragma once
#include "ENUM_AxisMode.h"

#include "STRUCT_AngleSettings.generated.h"

/*Struct used for rotating an object on an axis.*/
USTRUCT(BlueprintType)
struct FAngleSettings
{
	GENERATED_BODY()

	/*Reverse the start angle.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Angle", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bReverse = false;

	/*The iteration number of swing.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Angle", meta = (UIMin = "1", UIMax = "1000", ClampMin = "1", ClampMax = "1000"))
	unsigned int iCount = 1;

	/*The angle in degree.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Angle", meta = (UIMin = "0", UIMax = "360", ClampMin = "0", ClampMax = "360"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fAngle = 45.0f;

	/*Witch axis is rotating around.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Angle", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TEnumAsByte<EAxisMode> eAxis = EAxisMode::UP_AXIS;
};