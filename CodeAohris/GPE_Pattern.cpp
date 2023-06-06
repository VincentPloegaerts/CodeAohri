#include "GPE_Pattern.h"

AGPE_Pattern::AGPE_Pattern()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGPE_Pattern::BehaviorTick(const float& _deltaTime)
{
	
}

void AGPE_Pattern::CompleteBehavior()
{
	onCompleteBehavior.Broadcast(this);
	Destroy();
}