
#include "GPE_WallShot.h"

bool AGPE_WallShot::IsInRange(const FVector& _target, const FVector& _currentLocation, const bool& _useBox)
{
	if (!RangeBoxActor)return Super::IsInRange(_target, _currentLocation, _useBox);
	return RangeBoxActor->IsInsideBox(_target);
}

bool AGPE_WallShot::ShouldTickIfViewportsOnly() const
{
	return false;
}
