#include "P_PB_RotationRotator.h"

#include "P_DT_PatternDataTable.h"

void UP_PB_RotationRotator::InitializeRotation(UP_DT_PatternDataTable* _dataTable)
{
	Super::InitializeRotation(_dataTable);
	if (!_dataTable) return;
	rotatorSettings = _dataTable->GetRotatorSettings();
	rotation = FRotator(rotatorSettings.bPitchRotation, rotatorSettings.bYawRotation, rotatorSettings.bRollRotation);
}

void UP_PB_RotationRotator::InitializeRotation(FPatternData& _patternData)
{
	Super::InitializeRotation(_patternData);
	rotatorSettings = _patternData.GetRotatorSettings();
	rotation = FRotator(rotatorSettings.bPitchRotation, rotatorSettings.bYawRotation, rotatorSettings.bRollRotation);
}

void UP_PB_RotationRotator::RotationBehavior(const float& _deltaTime)
{
	Super::RotationBehavior(_deltaTime);
	if (!bIsEnable || !owner) return;

	const float& _scale = _deltaTime * sRotationSettings.fRotationSpeed;
	owner->AddActorLocalRotation(rotation * _scale);
}
