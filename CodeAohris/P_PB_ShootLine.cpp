#include "P_PB_ShootLine.h"

#include "P_DT_ShootDataTable.h"

void UP_PB_ShootLine::InitializeShoot(UP_DT_ShootDataTable* _shootDataTable)
{
	Super::InitializeShoot(_shootDataTable);
	if (!_shootDataTable) return;
	sShootLineSettings = _shootDataTable->GetShootLineSettings();
}

void UP_PB_ShootLine::InitializeShoot(FPatternData& _patternData)
{
	Super::InitializeShoot(_patternData);
	sShootLineSettings = _patternData.GetShootLineSettings();
}

void UP_PB_ShootLine::Shoot()
{
	if (!owner) return;
	Super::Shoot();
	
	const FVector& _center = owner->GetActorLocation();
	FVector _direction = owner->GetActorForwardVector();
	const FVector& _axe = sShootLineSettings.ePlan == EPlan::VERTICAL ? owner->GetActorUpVector() : owner->GetActorRightVector();

	const float& _fHalfLength = sShootLineSettings.fLength / 2.0f;
	const int& _max = shootSettings.iSpawnAtOnce;

	for (int _count = 0; _count < _max; _count++)
	{
		const float& _progress = FMath::Lerp(-_fHalfLength, _fHalfLength, _count / (float)_max);
		const FVector& _location = _center + _axe * _progress;

		ShootProjectile(_location, _direction);
	}
}