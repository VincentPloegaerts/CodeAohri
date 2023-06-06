#include "P_PB_ShootCross.h"

#include "P_DT_ShootDataTable.h"

void UP_PB_ShootCross::InitializeShoot(UP_DT_ShootDataTable* _shootDataTable)
{
	Super::InitializeShoot(_shootDataTable);
	if (!_shootDataTable) return;
	sShootCrossSettings = _shootDataTable->GetShootCrossSettings();
}

void UP_PB_ShootCross::InitializeShoot(FPatternData& _patternData)
{
	Super::InitializeShoot(_patternData);
	sShootCrossSettings = _patternData.GetShootCrossSettings();
}

void UP_PB_ShootCross::Shoot()
{
	if (!owner) return;
	onShoot.Broadcast();
	
	const FVector& _up = owner->GetActorUpVector();
	const FVector& _right = owner->GetActorRightVector();

	ShootArcAngle(sShootCrossSettings.fAngleX, sShootCrossSettings.fRadius, sShootCrossSettings.iProjectileOnX, _up, sShootCrossSettings.bKeepPatternForward, true);
	ShootArcAngle(sShootCrossSettings.fAngleY, sShootCrossSettings.fRadius, sShootCrossSettings.iProjectileOnY, _right, sShootCrossSettings.bKeepPatternForward, true);
}