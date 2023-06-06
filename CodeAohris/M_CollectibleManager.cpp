#include "M_CollectibleManager.h"

AM_CollectibleManager::AM_CollectibleManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AM_CollectibleManager::RegisterToManager(AC_Collectible* _collectible)
{
	collectibles.Add(_collectible);
}

