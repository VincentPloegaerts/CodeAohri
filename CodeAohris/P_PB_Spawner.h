#pragma once

#include "CoreMinimal.h"
#include "P_PatternBehavior.h"
#include "STRUCT_GPE_GlobalSettings.h"

#include "STRUCT_SpawnerSettings.h"

#include "P_PB_Spawner.generated.h"

class AM_FeedbackManager;
class AM_GPE_PatternTickManager;
class UP_DT_ZoneDataTable;
class AGPE_Pattern;

UCLASS(Abstract)
class INSIDEUS_API UP_PB_Spawner : public UP_PatternBehavior
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FSpawnerSettings spawnerSettings;

	UPROPERTY(VisibleAnywhere, Category = "Behavior | Spawner | Values")
	UP_DT_ZoneDataTable* zoneDataTable = nullptr;

	UPROPERTY()
	bool bUseGPEGlobalSettings = false;
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Spawner | Values")
	FGPE_GlobalSettings gpe_GlobalSettings;
	
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Spawner | Values")
	float fElapsedTime = 0;
	//Total amount of GPE Spawned
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Spawner | Values")
	int iTotalSpawned = 0;
	//Amount of GPE Complete
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Spawner | Values")
	int iDone = 0;
	//Amount of GPE to spawn
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Spawner | Values")
	int iToSpawn = 0;
	//Amount GPE cycle spawned (incrementation on spawn depends on SpawnerType)
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Spawner | Values")
	int iSpawned = 0;

	UPROPERTY(VisibleAnywhere, Category = "Behavior | Spawner | Values")
	AM_GPE_PatternTickManager* tickManager = nullptr;

	UPROPERTY()
	AM_FeedbackManager* feedbackManager = nullptr;
	
	UPROPERTY()
	UWorld* world = nullptr;

public:
	FORCEINLINE bool IsSpawnComplete() const { return iSpawned >= iToSpawn;	}
	
public:
	virtual void ActivateBehavior(UP_DT_BaseDataTable* _dataTable) override;
	virtual void ActivateBehavior(FPatternData& _patternData) override;
	virtual void TickBehavior(const float& _deltaTime) override;

protected:
	virtual void InitializeSpawner();
	virtual void InitializeSpawner(FPatternData& _patternData);
	virtual void SpawnBehavior();
	
	void SpawnComplete();
	
	UFUNCTION() void SpawnZoneGPE(const TSubclassOf<AGPE_Zone>& _type, const FVector& _location, const FRotator& _rotation);
	UFUNCTION() void EndGPE(AGPE_Pattern* _zoneGPE);
};