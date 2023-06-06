#pragma once

#include "CoreMinimal.h"
#include "P_Pattern.h"
#include "P_PatternZone.generated.h"

class UP_PB_Scaling;
class UP_PB_SpawnerDefault;
class UP_PB_SpawnerConcentric;
class UP_PB_SpawnerMulti;
class UP_PB_ArenaCloser;

UCLASS()
class INSIDEUS_API AP_PatternZone : public AP_Pattern
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Pattern Zone")
	UP_PB_Scaling* scaling;
	UPROPERTY(EditAnywhere, Category = "Pattern Zone")
	UP_PB_SpawnerDefault* spawnerDefault;
	UPROPERTY(EditAnywhere, Category = "Pattern Zone")
	UP_PB_SpawnerConcentric* spawnerConcentric;
	UPROPERTY(EditAnywhere, Category = "Pattern Zone")
	UP_PB_SpawnerMulti* spawnerMulti;


public:
	FORCEINLINE UP_PB_Scaling* GetScaling() const { return scaling; }
	FORCEINLINE UP_PB_SpawnerDefault* GetSpawnerDefault() const { return spawnerDefault; }
	FORCEINLINE UP_PB_SpawnerConcentric* GetSpawnerConcentric() const { return spawnerConcentric; }
	FORCEINLINE UP_PB_SpawnerMulti* GetSpawnerMulti() const { return spawnerMulti; }

public:
	AP_PatternZone();
	
	//Called by TaskNode from boss BehaviorTree
	virtual void InitializePattern(UP_DT_BaseDataTable* _dataTable) override;
	virtual void InitializePatternData(FPatternData& _patternData) override;
	virtual void DeActivatePatternBehavior() override;
};