#pragma once
#include "ENUM_SpawnSettings.h"

#include "STRUCT_MovementToTargetSettings.generated.h"

USTRUCT()
struct FMovementToTargetSettings
{
	GENERATED_BODY()

	//Amount of Movement between initial position and target position
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings | To Target", meta = (UIMin = "1", UIMax = "50", ClampMin = "1", ClampMax = "50"))
	unsigned int iRepeat = 1;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings | To Target", meta = (UIMin = "1", UIMax = "10000", ClampMin = "1", ClampMax = "10000"))
	float fMovementSpeed = 200;

	//Target to move to
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings | To Target")
	TEnumAsByte<ESpawnSettings> target = ESpawnSettings::BOSS;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings | To Target", meta  = (EditCondition = "target == ESpawnSettings::ARENA_POINT", EditConditionHides, GetOptions = "GetPointNames"))
	FString arenaPointName;
};