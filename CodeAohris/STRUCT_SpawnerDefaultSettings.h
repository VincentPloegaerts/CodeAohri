#pragma once
#include "STRUCT_SpawnerDefaultSettings.generated.h"

USTRUCT()
struct FSpawnerDefaultSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Global", meta = (UIMin = "1", UIMax = "40", ClampMin = "1", ClampMax = "40"))
	unsigned int iSpawnQty = 1;

	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Global")
	bool bSpawnOnPlayer = false;
};