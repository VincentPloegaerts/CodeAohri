#pragma once
#include "STRUCT_SpawnerFeedBack.h"
#include "STRUCT_ProjectileSettings.h"
#include "STRUCT_SpawnerSettings.generated.h"

class AGPE_Zone;

USTRUCT()
struct FSpawnerSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Global")
	bool bWaitEndOfGPE = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Global")
	TSubclassOf<AGPE_Zone> zoneType;

	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Global")
	FVector spawnOffset = FVector(0);

	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Global")
	bool bUseGroundRaycast = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Global", meta = (EditCondition = "bUseGroundRaycast"))
	TArray<TEnumAsByte<EObjectTypeQuery>> groundLayers;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Global", meta = (UIMin = "0.05", UIMax = "10", ClampMin = "0.05", ClampMax = "10"))
	float fSpawnRate = 1;
	
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Global", meta = (ClampMin = "0"))
	bool bActivateRandom2D = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Global", meta = (ClampMin = "0", EditCondition = "bActivateRandom2D"))
	float randomRange2D = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Global")
	FSpawnerFeedBack zoneFeedBack = FSpawnerFeedBack();
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings | Global")
	FKnockbackSettings knockBackSettings = FKnockbackSettings();
};