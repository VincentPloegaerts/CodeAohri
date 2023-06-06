#pragma once

#include "CoreMinimal.h"
#include "P_PB_Spawner.h"

#include "STRUCT_SpawnerConcentricSettings.h"

#include "P_PB_SpawnerConcentric.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_SpawnerConcentric : public UP_PB_Spawner
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FSpawnerConcentricSettings spawnerConcentricSettings;

private:
	virtual void InitializeSpawner() override;
	virtual void InitializeSpawner(FPatternData& _patternData) override;
	virtual void SpawnBehavior() override;

	void SpawnCircle(const int& _index);
	
	float GetTotalSpacing(const int& _index) const;
	int GetSpawnAmount(const int& _index) const;

};