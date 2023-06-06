#pragma once

#include "STRUCT_MovementTeleportSettings.generated.h"

USTRUCT()
struct FMovementTeleportSettings
{
	GENERATED_BODY()

	//Offset between target and teleportation point
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings | Teleport")
	FVector vOffset = FVector(0);
	
	//Amount of Teleportation
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings | Teleport", meta = (UIMin = "1", UIMax = "50", ClampMin = "1", ClampMax = "50"))
	unsigned int iRepeat = 1;
	
	//Time before Teleportation
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings | Teleport", meta = (UIMin = "0.1", UIMax = "10", ClampMin = "0.1", ClampMax = "10"))
	float fRepeatRate = 1;	
};