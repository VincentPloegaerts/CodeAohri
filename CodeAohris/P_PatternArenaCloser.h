#pragma once

#include "CoreMinimal.h"
#include "P_PatternBase.h"
#include "P_PatternArenaCloser.generated.h"

class UP_PB_ArenaCloser;

UCLASS()
class INSIDEUS_API AP_PatternArenaCloser : public AP_PatternBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Pattern Zone")
	UP_PB_ArenaCloser* arenaCloser = nullptr;

public:
	FORCEINLINE UP_PB_ArenaCloser* GetArenaCloser() const { return arenaCloser; }

public:
	AP_PatternArenaCloser();

	//Called by TaskNode from boss BehaviorTree
	virtual void InitializePattern(UP_DT_BaseDataTable* _dataTable) override;
	virtual void InitializePatternData(FPatternData& _patternData) override;
	virtual void DeActivatePatternBehavior() override;
};