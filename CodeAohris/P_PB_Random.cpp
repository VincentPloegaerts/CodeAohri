#include "P_PB_Random.h"

#include "P_DT_ShootDataTable.h"

void UP_PB_Random::InitializeRandom(UP_DT_ShootDataTable* _dataTable)
{
	//Declaration in Child class
}

void UP_PB_Random::InitializeRandom(FPatternData& _patternData)
{
	
}

void UP_PB_Random::ActivateBehavior(UP_DT_BaseDataTable* _dataTable)
{
	Super::ActivateBehavior(_dataTable);
	if (!bIsEnable) return;
	
	UP_DT_ShootDataTable* _data = Cast<UP_DT_ShootDataTable>(_dataTable);
	
	InitializeRandom(_data);
}

void UP_PB_Random::ActivateBehavior(FPatternData& _patternData)
{
	Super::ActivateBehavior(_patternData);
	if (!bIsEnable) return;
	
	InitializeRandom(_patternData);
}

void UP_PB_Random::Randomize(FVector& _direction)
{
	//Declaration in Child class
}
