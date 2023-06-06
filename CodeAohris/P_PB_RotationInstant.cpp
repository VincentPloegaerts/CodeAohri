#include "P_PB_RotationInstant.h"

#include "P_DT_PatternDataTable.h"

void UP_PB_RotationInstant::InitializeRotation(UP_DT_PatternDataTable* _dataTable)
{
	Super::InitializeRotation(_dataTable);
	if (!_dataTable) return;
	sInstantSettings = _dataTable->GetInstantSettings();
	
	InitializeInstantRotation();
}

void UP_PB_RotationInstant::InitializeRotation(FPatternData& _patternData)
{
	Super::InitializeRotation(_patternData);
	sInstantSettings = _patternData.GetInstantSettings();
	
	InitializeInstantRotation();
}

void UP_PB_RotationInstant::RotationBehavior(const float& _deltaTime)
{
	Super::RotationBehavior(_deltaTime);
	if (!bIsEnable || !owner) return;

	owner->AddActorWorldRotation(rRotation);
	
	bIsEnable = false;
}

void UP_PB_RotationInstant::InitializeInstantRotation()
{
	const EAxisMode& _axis = sInstantSettings.eAxis;
	if (_axis == EAxisMode::UP_AXIS)
		rRotation.Yaw = sInstantSettings.fRotation;
	else if (_axis == EAxisMode::RIGHT_AXIS)
		rRotation.Pitch = sInstantSettings.fRotation;
	else
		rRotation.Roll = sInstantSettings.fRotation;
}
