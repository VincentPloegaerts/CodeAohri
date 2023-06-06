#pragma once

#include "CoreMinimal.h"
#include "P_PB_Spawner.h"

#include "STRUCT_SpawnerDefaultSettings.h"

#include "P_PB_SpawnerDefault.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_SpawnerDefault : public UP_PB_Spawner
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FSpawnerDefaultSettings spawnerDefaultSettings;

private:
	virtual void InitializeSpawner() override;
	virtual void InitializeSpawner(FPatternData& _patternData) override;
	virtual void SpawnBehavior() override;
};