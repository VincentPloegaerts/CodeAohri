#include "P_PB_SpawnerDefault.h"

#include "IU_Macros.h"
#include "IU_Player.h"
#include "M_GameMode.h"
#include "P_DT_ZoneDataTable.h"

void UP_PB_SpawnerDefault::InitializeSpawner()
{
	Super::InitializeSpawner();
	if (!zoneDataTable) return;

	spawnerDefaultSettings = zoneDataTable->GetSpawnerDefaultSettings();
	iSpawned = 0;
	iToSpawn = spawnerDefaultSettings.iSpawnQty;
}

void UP_PB_SpawnerDefault::InitializeSpawner(FPatternData& _patternData)
{
	Super::InitializeSpawner(_patternData);
	spawnerDefaultSettings = _patternData.GetSpawnerDefaultSettings();
	iSpawned = 0;
	iToSpawn = spawnerDefaultSettings.iSpawnQty;
}

void UP_PB_SpawnerDefault::SpawnBehavior()
{
	if (!bIsEnable) return;
	FVector _location (owner->GetActorLocation());

	if (spawnerDefaultSettings.bSpawnOnPlayer)
	{
		if (const AM_GameMode* _gameMode = world->GetAuthGameMode<AM_GameMode>())
		{
			if (const AIU_Player* _player = _gameMode->GetPlayer())
			{
				const FVector& _playerLocation = _player->GetActorLocation();
				_location.X = _playerLocation.X;
				_location.Y = _playerLocation.Y;
			}
			else
				LOG_ERROR("UP_PB_SpawnerDefault::SpawnBehavior -> Invalid Player");
		}
		else
			LOG_ERROR("UP_PB_SpawnerDefault::SpawnBehavior -> Invalid Gamemode");
	}
	
	SpawnZoneGPE(spawnerSettings.zoneType, _location, owner->GetActorRotation());
}