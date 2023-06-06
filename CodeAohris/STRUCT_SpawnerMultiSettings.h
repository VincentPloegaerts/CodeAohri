#pragma once

#include "STRUCT_SpawnerMultiSettings.generated.h"

USTRUCT()
struct FSpawnerMultiSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Multi")
	TArray<unsigned int> quantityBySpawn;
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Multi", meta = (UIMin = "0", ClampMin = "0"))
	float fRange = 1000;
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Multi", meta = (UIMin = "0", ClampMin = "0"))
	float fMinDistance = 100;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Multi")
	bool bSpawnOnPlayer = false;
};