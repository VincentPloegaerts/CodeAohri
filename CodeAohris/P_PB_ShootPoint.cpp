#include "P_PB_ShootPoint.h"

#include "IU_Macros.h"
#include "P_DT_ShootDataTable.h"

void UP_PB_ShootPoint::InitializeShoot(UP_DT_ShootDataTable* _shootDataTable)
{
	Super::InitializeShoot(_shootDataTable);
	if (!_shootDataTable) return;
	sShootPointSettings = _shootDataTable->GetShootPointSettings();
}

void UP_PB_ShootPoint::InitializeShoot(FPatternData& _patternData)
{
	Super::InitializeShoot(_patternData);
	sShootPointSettings = _patternData.GetShootPointSettings();
}

void UP_PB_ShootPoint::Shoot()
{
	if (!owner) return;
	Super::Shoot();
	
	const float& _radius = sShootPointSettings.fRadius;
	const FVector& _center = owner->GetActorLocation();
	const FVector& _right = owner->GetActorRightVector();
	const FVector& _up = owner->GetActorUpVector();
	FVector _direction = owner->GetActorForwardVector();

	const FVector& _offset = _right + FMath::RandRange(-_radius, _radius) + _up * FMath::RandRange(-_radius, _radius);
	
	const int& _max = shootSettings.iSpawnAtOnce;
	
	for (int _count = 0; _count < _max; ++_count)
		ShootProjectile(_center + _offset, _direction);
}