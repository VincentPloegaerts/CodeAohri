#include "P_PB_Random3D.h"

#include "P_DT_ShootDataTable.h"

#include "Kismet/KismetMathLibrary.h"

void UP_PB_Random3D::InitializeRandom(UP_DT_ShootDataTable* _dataTable)
{
	if (!_dataTable) return;
	sRandom3DSettings = _dataTable->GetShootRandom3DSettings();
}

void UP_PB_Random3D::InitializeRandom(FPatternData& _patternData)
{
	Super::InitializeRandom(_patternData);
	sRandom3DSettings = _patternData.GetShootRandom3DSettings();
}

void UP_PB_Random3D::Randomize(FVector& _direction)
{
	if (!bIsEnable) return;
	_direction =  UKismetMathLibrary::RandomUnitVectorInConeInDegrees(_direction, sRandom3DSettings.fAngle / 2.0f);
}
