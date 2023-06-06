#include "P_PB_ShootArc.h"

#include "P_DT_ShootDataTable.h"

void UP_PB_ShootArc::InitializeShoot(UP_DT_ShootDataTable* _shootDataTable)
{
	Super::InitializeShoot(_shootDataTable);
	if (!_shootDataTable) return;
	sShootArcSettings = _shootDataTable->GetShootArcSettings();
}

void UP_PB_ShootArc::InitializeShoot(FPatternData& _patternData)
{
	Super::InitializeShoot(_patternData);
	sShootArcSettings = _patternData.GetShootArcSettings();
}

void UP_PB_ShootArc::Shoot()
{
	if (!owner) return;
	onShoot.Broadcast();
	
	ShootArcAngle(sShootArcSettings.fAngle, sShootArcSettings.fRadius, shootSettings.iSpawnAtOnce, owner->GetActorUpVector(), sShootArcSettings.bKeepPatternForward, true);
}

void UP_PB_ShootArc::ShootArcAngle(const float& _angleRange, const float& _radius, const int& _max, const FVector& _axe, const bool& _keepPatternForward, const bool& _moveBackPoints) const
{
	if (!owner) return;
	
	const float& _halfRange = _angleRange / 2.0f;

	const FVector& _center = owner->GetActorLocation();
	const FVector& _fwd = owner->GetActorForwardVector();

	const float& _parse = _angleRange / _max;
		
	for	(int _count = 0; _count < _max; _count++)
	{
		const float& _angle = FMath::Lerp(-_halfRange, _halfRange, _count / float(_max));
		
		const FVector& _location = _center + _fwd.RotateAngleAxis(_angle + _parse / 2.0f, _axe) * _radius;
		FVector _direction = _keepPatternForward ? _fwd : (_location - _center).GetSafeNormal();
		
		ShootProjectile(_location - (_moveBackPoints ? _fwd * _radius : FVector(0)), _direction);
	}
}