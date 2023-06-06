#pragma once
#include "P_DT_BaseDataTable.h"
#include "STRUCT_SpawnSettings.h"
#include "STRUCT_SerieElement.generated.h"

class UP_DT_BaseDataTable;

//Struct editable in the behavior tree
USTRUCT()
struct FSerieElement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<UP_DT_BaseDataTable*> patterns;

	//Delay after spawning patterns or waiting end of patterns
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.01"))
	float fDelay = 1.0f;

	UPROPERTY(EditAnywhere)
	bool bWaitEndOfPatterns = false;

	UPROPERTY(EditAnywhere)
	FSpawnSettings spawnSettings;

public:
	const TArray<UP_DT_BaseDataTable*>& GetPatterns() const { return patterns; }
	const FSpawnSettings& GetSpawnSettings() const { return spawnSettings; }
};
