
#include "T_ProjectileGuidedTuto.h"
#include "M_WorldTimeManager.h"
#include "M_GameMode.h"


bool AT_ProjectileGuidedTuto::IsAtDistanceSlowMow()
{
	return (GetActorLocation() - target->GetActorLocation()).Size() < minDistanceSlowMow;
}

void AT_ProjectileGuidedTuto::MovementBehavior(const float& _deltaTime)
{
	Super::MovementBehavior(_deltaTime);
	if (IsAtDistanceSlowMow())
		onReachedDistance.Broadcast();
}
