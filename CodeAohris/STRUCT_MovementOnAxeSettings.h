#pragma once

#include "ENUM_AxisMode.h"

#include "STRUCT_MovementOnAxeSettings.generated.h"

USTRUCT()
struct FMovementOnAxeSettings
{
	GENERATED_BODY()

	//Amount of Movement between initial position and target position
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings | On Axe", meta = (UIMin = "1", UIMax = "50", ClampMin = "1", ClampMax = "50"))
	unsigned int iRepeat = 50;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings | On Axe", meta = (UIMin = "1", UIMax = "10000", ClampMin = "1", ClampMax = "10000"))
	float fMovementSpeed = 200;
	
	//Distance of movement on the given axe
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings | On Axe", meta = (UIMin = "-20000", UIMax = "20000", ClampMin = "-20000", ClampMax = "20000"))
	float fDistance = 500;

	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings | On Axe")
	bool bSpawnIsNotCenter = false;
	
	//Axe of movement (local to pattern actor)
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings | On Axe")
	TEnumAsByte<EAxisMode> axis = EAxisMode::FORWARD_AXIS;
};