#include "P_PB_Movement.h"

#include "P_DT_PatternDataTable.h"

void UP_PB_Movement::ActivateBehavior(UP_DT_BaseDataTable* _dataTable)
{
	Super::ActivateBehavior(_dataTable);
	UP_DT_PatternDataTable* _patternDataTable = Cast<UP_DT_PatternDataTable>(_dataTable);
	if (!_patternDataTable) return;
	
	InitializeMovement(_patternDataTable);
}

void UP_PB_Movement::ActivateBehavior(FPatternData& _patternData)
{
	Super::ActivateBehavior(_patternData);
	InitializeMovement(_patternData);
}

void UP_PB_Movement::TickBehavior(const float& _deltaTime)
{
	Super::TickBehavior(_deltaTime);
	if (!bIsEnable) return;

	MovementBehavior(_deltaTime);
}

void UP_PB_Movement::InitializeMovement(UP_DT_PatternDataTable* _dataTable)
{
	//Declaration in Child class
}

void UP_PB_Movement::InitializeMovement(FPatternData& _patternData)
{
	
}

void UP_PB_Movement::MovementBehavior(const float& _deltaTime)
{
	//Declaration in Child class
}
