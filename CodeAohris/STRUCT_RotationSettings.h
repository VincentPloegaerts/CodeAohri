#pragma once
#include "STRUCT_RotationSettings.generated.h"

/*Struct used for the rotation speed.*/
USTRUCT(BlueprintType)
struct FRotationSettings
{
	GENERATED_BODY()
	
	/*Rotation speed of the object.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings | Global", meta = (UIMin = "0", UIMax = "5000", ClampMin = "0", ClampMax = "5000"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fRotationSpeed = 0;
};