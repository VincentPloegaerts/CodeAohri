#include "P_PatternZone.h"

#include "IU_Macros.h"

#include "P_DT_ZoneDataTable.h"

#include "P_PB_Scaling.h"

#include "P_PB_SpawnerConcentric.h"
#include "P_PB_SpawnerDefault.h"
#include "P_PB_SpawnerMulti.h"

#include "P_PB_ArenaCloser.h"

AP_PatternZone::AP_PatternZone()
{
	PrimaryActorTick.bCanEverTick = false;

	scaling = CreateDefaultSubobject<UP_PB_Scaling>(TEXT("Scaling"));
	AddOwnedComponent(scaling);
	
	spawnerDefault = CreateDefaultSubobject<UP_PB_SpawnerDefault>(TEXT("Spawner Default"));
	AddOwnedComponent(spawnerDefault);
	
	spawnerConcentric = CreateDefaultSubobject<UP_PB_SpawnerConcentric>(TEXT("Spawner Concentric"));
	AddOwnedComponent(spawnerDefault);
	
	spawnerMulti = CreateDefaultSubobject<UP_PB_SpawnerMulti>(TEXT("Spawner Multi"));
	AddOwnedComponent(spawnerDefault);
}

void AP_PatternZone::InitializePattern(UP_DT_BaseDataTable* _dataTable)
{
	Super::InitializePattern(_dataTable);

	UP_DT_ZoneDataTable* _zoneDataTable = Cast<UP_DT_ZoneDataTable>(_dataTable);

	if (!_zoneDataTable)
	{
		LOG("AP_PatternZone::InitializePattern -> Invalid data table");
		return;
	}
	AM_PatternsTickManager::FOnTick& _onTick = patternsTickManager->GetOnTick();

	InitializeBehavior(_zoneDataTable->GetScalingState(), scaling, _zoneDataTable, _onTick);
	
	InitializeBehavior(_zoneDataTable->GetSpawnerDefaultState(), spawnerDefault, _zoneDataTable, _onTick);
	InitializeBehavior(_zoneDataTable->GetSpawnerConcentricState(), spawnerConcentric, _zoneDataTable, _onTick);
	InitializeBehavior(_zoneDataTable->GetSpawnerMultiState(), spawnerMulti, _zoneDataTable, _onTick);
}

void AP_PatternZone::InitializePatternData(FPatternData& _patternData)
{
	Super::InitializePatternData(_patternData);
	
	AM_PatternsTickManager::FOnTick& _onTick = patternsTickManager->GetOnTick();

	InitializeBehavior(_patternData.GetScalingState(), scaling, _patternData, _onTick);
	
	InitializeBehavior(_patternData.GetSpawnerDefaultState(), spawnerDefault, _patternData, _onTick);
	InitializeBehavior(_patternData.GetSpawnerConcentricState(), spawnerConcentric, _patternData, _onTick);
	InitializeBehavior(_patternData.GetSpawnerMultiState(), spawnerMulti, _patternData, _onTick);
}

void AP_PatternZone::DeActivatePatternBehavior()
{
	Super::DeActivatePatternBehavior();

	DeActivateBehavior(scaling, patternsTickManager->GetOnTick());
}
