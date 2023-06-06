#include "P_PatternArenaCloser.h"

#include "P_DT_ArenaCloserDataTable.h"
#include "P_PB_ArenaCloser.h"

AP_PatternArenaCloser::AP_PatternArenaCloser()
{
	arenaCloser = CreateDefaultSubobject<UP_PB_ArenaCloser>("Arena Closer");
	AddOwnedComponent(arenaCloser);
}

void AP_PatternArenaCloser::InitializePattern(UP_DT_BaseDataTable* _dataTable)
{
	Super::InitializePattern(_dataTable);
	
	arenaCloser->ActivateBehavior(_dataTable);
	arenaCloser->OnBehaviorComplete().AddDynamic(this, &AP_PatternBase::PatternCompletion);
}

void AP_PatternArenaCloser::InitializePatternData(FPatternData& _patternData)
{
	Super::InitializePatternData(_patternData);
	
	arenaCloser->ActivateBehavior(_patternData);
	arenaCloser->OnBehaviorComplete().AddDynamic(this, &AP_PatternBase::PatternCompletion);
}

void AP_PatternArenaCloser::DeActivatePatternBehavior()
{
	Super::DeActivatePatternBehavior();

	arenaCloser->OnBehaviorComplete().RemoveDynamic(this, &AP_PatternBase::PatternCompletion);
	arenaCloser->DeActivateBehavior();
}
