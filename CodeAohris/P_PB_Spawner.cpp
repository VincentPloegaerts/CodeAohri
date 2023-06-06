#include "P_PB_Spawner.h"

#include "IU_Macros.h"
#include "GPE_Zone.h"
#include "M_GameMode.h"
#include "M_GPE_PatternTickManager.h"
#include "P_DT_ZoneDataTable.h"
#include "Kismet/KismetSystemLibrary.h"

void UP_PB_Spawner::ActivateBehavior(UP_DT_BaseDataTable* _dataTable)
{
	Super::ActivateBehavior(_dataTable);
	zoneDataTable = Cast<UP_DT_ZoneDataTable>(_dataTable);
	if (!zoneDataTable)
	{
		LOG("UP_PB_Spawner::ActivateBehavior : Data Table is not a Zone Data Table type");
		return;
	}

	world = GetWorld();
	if (const AM_GameMode* _gm = world->GetAuthGameMode<AM_GameMode>())
	{
		tickManager = _gm->GetGPEPatternTickManager();
		if (!tickManager) LOG("UP_PB_Spawner::ActivateBehavior : No GPE tick manager found");
	}
	bUseGPEGlobalSettings = false;
	InitializeSpawner();
}

void UP_PB_Spawner::ActivateBehavior(FPatternData& _patternData)
{
	Super::ActivateBehavior(_patternData);

	world = GetWorld();
	if (const AM_GameMode* _gm = world->GetAuthGameMode<AM_GameMode>())
	{
		tickManager = _gm->GetGPEPatternTickManager();
		if (!tickManager)
			LOG("UP_PB_Spawner::ActivateBehavior : No GPE tick manager found");
	}

	bUseGPEGlobalSettings = true;
	InitializeSpawner(_patternData);
}

void UP_PB_Spawner::TickBehavior(const float& _deltaTime)
{
	Super::TickBehavior(_deltaTime);
	if (!bIsEnable) return;
	
	fElapsedTime += _deltaTime;
	if (fElapsedTime > spawnerSettings.fSpawnRate)
	{
		fElapsedTime = 0;
		
		SpawnBehavior();
		iSpawned++;
		
		if (IsSpawnComplete())
			SpawnComplete();
	}
}

void UP_PB_Spawner::InitializeSpawner()
{
	if (!zoneDataTable) return;
	
	spawnerSettings = zoneDataTable->GetSpawnerSettings();
	fElapsedTime += spawnerSettings.fSpawnRate + 1;

	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		feedbackManager = _gameMode->GetFeedbackManager();
	}
}

void UP_PB_Spawner::InitializeSpawner(FPatternData& _patternData)
{
	spawnerSettings = _patternData.GetSpawnerSettings();
	gpe_GlobalSettings.bScaling = _patternData.GetScalingState();
	gpe_GlobalSettings.scalingSettings = _patternData.GetScalingSettings();
	
	gpe_GlobalSettings.bEditProjectileGPE = _patternData.GetEditProjectileState();
	gpe_GlobalSettings.projectileSettings = _patternData.GetGPEProjectileSettings();
	
	gpe_GlobalSettings.bEditFloorGPE = _patternData.GetEditFloorState();
	gpe_GlobalSettings.floorDangerSettings = _patternData.GetGPEFloorDangerSettings();
	
	gpe_GlobalSettings.bEditHuntRiftGPE = _patternData.GetEditHuntRiftState();
	gpe_GlobalSettings.huntRiftSettings = _patternData.GetGPEHuntRiftSettings();

	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		feedbackManager = _gameMode->GetFeedbackManager();
	}
}

void UP_PB_Spawner::SpawnBehavior()
{
	//Declaration in Child class
}
void UP_PB_Spawner::SpawnComplete()
{
	bIsEnable = false;
	if (!spawnerSettings.bWaitEndOfGPE)
	{
#if WITH_LOG_PB_SPAWNER
		LOG("Spawner Spawn Complete");
#endif
		onBehaviorComplete.Broadcast();
	}
}

void UP_PB_Spawner::SpawnZoneGPE(const TSubclassOf<AGPE_Zone>& _type, const FVector& _location, const FRotator& _rotation)
{
	FVector _randomLocation(0.f);
	
	if (spawnerSettings.bActivateRandom2D)
	{
		const float& _randomRange = FMath::RandRange(0.f, spawnerSettings.randomRange2D);
		const float& _randomRad = FMath::DegreesToRadians(FMath::RandRange(0.f, 360.f));

		_randomLocation = FVector(cos(_randomRad) * _randomRange, sin(_randomRad) * _randomRange, 0);
	}

	FVector _finalLocation = _location + _randomLocation;
		
	if (spawnerSettings.bUseGroundRaycast)
	{
		const TArray<TEnumAsByte<EObjectTypeQuery>>& _layers = spawnerSettings.groundLayers;
		if (_layers.Num() < 1)
			LOG_ERROR("UP_PB_Spawner::SpawnZoneGPE -> There is no specified layer in spawner settings");

		FHitResult _result;
		FVector _ZOffset(0.f, 0.f, 10000.f);

		if (Get_LineTrace(world, _finalLocation + _ZOffset, _finalLocation - _ZOffset, _layers, _result))
		{
			_finalLocation = _result.Location;
		}
		else
			LOG_ERROR("UP_PB_Spawner::SpawnZoneGPE -> Probably wrong arena layer type");
	}
	
	if (AGPE_Zone* _zoneGPE = world->SpawnActor<AGPE_Zone>(_type, _finalLocation + spawnerSettings.spawnOffset, _rotation))
	{
		++iTotalSpawned;
		if (spawnerSettings.bWaitEndOfGPE)
			_zoneGPE->OnCompleteBehavior().AddDynamic(this, &UP_PB_Spawner::EndGPE);

		if (bUseGPEGlobalSettings)
			_zoneGPE->InitializeGPEBehavior(gpe_GlobalSettings);
		else
			_zoneGPE->InitializeGPEBehavior(zoneDataTable);

		_zoneGPE->InitFeedback(feedbackManager, spawnerSettings.zoneFeedBack);
		_zoneGPE->InitKnockBackSettings(spawnerSettings.knockBackSettings);
		
		if (tickManager)
			tickManager->AddGPEPattern(_zoneGPE);
	}
}
void UP_PB_Spawner::EndGPE(AGPE_Pattern* _zoneGPE)
{
	++iDone;
	
	_zoneGPE->OnCompleteBehavior().RemoveDynamic(this, &UP_PB_Spawner::EndGPE);
	if (tickManager)
		tickManager->DestroyGPEPattern(_zoneGPE);
	
	if (!bIsEnable && iDone >= iTotalSpawned)
	{
#if WITH_LOG_PB_SPAWNER
		LOG("Spawner All Zone GPE End");
#endif
		onBehaviorComplete.Broadcast();
	}
}