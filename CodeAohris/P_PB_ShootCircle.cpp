#include "P_PB_ShootCircle.h"

#include "P_DT_ShootDataTable.h"

void UP_PB_ShootCircle::InitializeShoot(UP_DT_ShootDataTable* _shootDataTable)
{
	Super::InitializeShoot(_shootDataTable);
	if (!_shootDataTable) return;
	sShootCircleSettings = _shootDataTable->GetShootCircleSettings();
}

void UP_PB_ShootCircle::InitializeShoot(FPatternData& _patternData)
{
	Super::InitializeShoot(_patternData);
	sShootCircleSettings = _patternData.GetShootCircleSettings();
}

void UP_PB_ShootCircle::Shoot()
{
	if (!owner) return;
	onShoot.Broadcast();
	
	ShootArcAngle(359.0f, sShootCircleSettings.fRadius, shootSettings.iSpawnAtOnce, owner->GetActorUpVector());
}