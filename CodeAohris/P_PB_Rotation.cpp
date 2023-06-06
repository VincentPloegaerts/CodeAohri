#include "P_PB_Rotation.h"

#include "P_DT_PatternDataTable.h"

void UP_PB_Rotation::ActivateBehavior(UP_DT_BaseDataTable* _dataTable)
{
	Super::ActivateBehavior(_dataTable);
	UP_DT_PatternDataTable* _patternDataTable = Cast<UP_DT_PatternDataTable>(_dataTable);
	if (!_patternDataTable) return;
	
	InitializeRotation(_patternDataTable);	
}

void UP_PB_Rotation::ActivateBehavior(FPatternData& _patternData)
{
	Super::ActivateBehavior(_patternData);
	InitializeRotation(_patternData);
}

void UP_PB_Rotation::TickBehavior(const float& _deltaTime)
{
	if (!bIsEnable) return;
	
	RotationBehavior(_deltaTime);
}

void UP_PB_Rotation::InitializeRotation(UP_DT_PatternDataTable* _dataTable)
{
	if (!_dataTable) return;
	sRotationSettings = _dataTable->GetRotationSettings();
}

void UP_PB_Rotation::InitializeRotation(FPatternData& _patternData)
{
	sRotationSettings = _patternData.GetRotationSettings();
}

void UP_PB_Rotation::RotationBehavior(const float& _deltaTime)
{
	//Declaration in Child class
}