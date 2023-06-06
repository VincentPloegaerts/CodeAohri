#pragma once
#include "STRUCT_SpawnSettings.h"
#include "STRUCT_FeedBackData.h"
#include "STRUCT_PatternMovementData.generated.h"

class UP_DT_BaseDataTable;

//Used in boss movement
USTRUCT()
struct FPatternMovementData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Data")
	UP_DT_BaseDataTable* pattern = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movement Data")
	bool bWaitForEndOfPattern = false;

	UPROPERTY(EditAnywhere, Category = "Movement Data")
	FSpawnSettings spawnSettings;
};