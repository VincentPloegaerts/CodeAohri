#include "P_PB_Random2D.h"

#include "P_DT_ShootDataTable.h"

void UP_PB_Random2D::InitializeRandom(UP_DT_ShootDataTable* _dataTable)
{
	if (!_dataTable) return;
	sRandom2DSettings = _dataTable->GetShootRandom2DSettings();
}

void UP_PB_Random2D::InitializeRandom(FPatternData& _patternData)
{
	Super::InitializeRandom(_patternData);
	sRandom2DSettings = _patternData.GetShootRandom2DSettings();
}

void UP_PB_Random2D::Randomize(FVector& _direction)
{
	if (!bIsEnable || !owner) return;
	
	const FVector& _axis = sRandom2DSettings.ePlan == EPlan::HORIZONTAL ? owner->GetActorUpVector() : owner->GetActorRightVector();
	_direction = _direction.RotateAngleAxis(FMath::RandRange(-sRandom2DSettings.fAngle / 2.0f, sRandom2DSettings.fAngle / 2.0f), _axis);
}
