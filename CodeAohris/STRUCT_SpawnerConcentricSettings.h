#pragma once

#include "STRUCT_SpawnerConcentricSettings.generated.h"

USTRUCT()
struct FConcentricCircleSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Concentric Circle Settigns")
	unsigned int quantity = 0;
	UPROPERTY(EditAnywhere, Category = "Concentric Circle Settigns")
	float spacing = 0;
};

USTRUCT()
struct FSpawnerConcentricSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Concentric")
	TArray<FConcentricCircleSettings> circleSettings;
};
