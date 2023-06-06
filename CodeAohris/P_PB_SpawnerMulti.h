#pragma once

#include "CoreMinimal.h"
#include "P_PB_Spawner.h"

#include "STRUCT_SpawnerMultiSettings.h"

#include "P_PB_SpawnerMulti.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_SpawnerMulti : public UP_PB_Spawner
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FSpawnerMultiSettings spawnerMultiSettings;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> locationTaken;
	
private:
	virtual void InitializeSpawner() override;
	virtual void InitializeSpawner(FPatternData& _patternData) override;
	virtual void SpawnBehavior() override;

	void SpawnMulti(const int& _index);

	//Check if given location is enough away from other takenlocation
	bool IsLocationCorrect(const FVector& _location, const float& _minDistance) const;
	//Recursive function. Get a random location in range and test if is enough away from the other taken lokcation
	bool GetLocation(FVector& _location, const FVector& _fwd, const FVector& _right, const float& _radius, const float& _minDistance, int _currentTry = 0);
};