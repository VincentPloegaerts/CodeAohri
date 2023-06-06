#include "M_PatternsTickManager.h"

#include "M_GameMode.h"

AM_PatternsTickManager::AM_PatternsTickManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AM_PatternsTickManager::Tick(float DeltaTime)
{
	const float& _deltaTime = fWorldSpeed * DeltaTime;
	Super::Tick(_deltaTime);
	onTick.Broadcast(_deltaTime);
}