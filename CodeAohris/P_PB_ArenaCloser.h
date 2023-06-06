#pragma once

#include "CoreMinimal.h"
#include "P_PatternBehavior.h"

#include "STRUCT_ArenaCloserSettings.h"

#include "P_PB_ArenaCloser.generated.h"

class AA_ArenaSlice;

UCLASS()
class INSIDEUS_API UP_PB_ArenaCloser : public UP_PatternBehavior
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FArenaCloserSettings arenaCloserSettings;
	
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Arena Closer | Values")
	TArray<AA_ArenaSlice*> slices;
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Arena Closer | Values")
	int iSliceToClose = 0;	
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Arena Closer | Values")
	int iSliceOpen = 0;
	
public:
	virtual void ActivateBehavior(UP_DT_BaseDataTable* _dataTable) override;
	virtual void ActivateBehavior(FPatternData& _patternData) override;

private:
	void StartArenaSliceClosureProcess();

	UFUNCTION() void AreaSliceClosureProcessEnd(AA_ArenaSlice* _slice);
};