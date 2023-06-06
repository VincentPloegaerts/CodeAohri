#include "P_DT_ZoneDataTable.h"

#include "STRUCT_PatternData.h"

#if WITH_EDITOR
void UP_DT_ZoneDataTable::SavePattern()
{
	FZoneDataTableJSON _json = GetZoneDataTableJSON();

	UM_SaveManager::Save(_json, Save_Pattern, GetName());
}

void UP_DT_ZoneDataTable::LoadPattern()
{
	FZoneDataTableJSON _json;
	
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Pattern, GetName());
	if (!_loadSuccess) return;

	LoadZoneDataTableJSON(_json);
}

FZoneDataTableJSON UP_DT_ZoneDataTable::GetZoneDataTableJSON() const
{
	FZoneDataTableJSON _json;
	_json.InitBaseJSON(GetBaseDataTableJSON());
	_json.InitPatternJSON(GetPatternDataTableJSON());

	_json.bScaling = bScaling;
	_json.scalingSettings = scalingSettings;
	_json.spawnerSettings = spawnerSettings;
	_json.bSpawnerDefault = bSpawnerDefault;
	_json.spawnerDefaultSettings = spawnerDefaultSettings;
	_json.bSpawnerConcentric = bSpawnerConcentric;
	_json.spawnerConcentricSettings = spawnerConcentricSettings;
	_json.bSpawnerMulti = bSpawnerMulti;
	_json.spawnerMultiSettings = spawnerMultiSettings;
	_json.bEditProjectileGPE = bEditProjectileGPE;
	_json.projectileSettings = projectileSettings;
	_json.bEditFloorGPE = bEditFloorGPE;
	_json.floorDangerSettings = floorDangerSettings;
	_json.bEditHuntRiftGPE = bEditHuntRiftGPE;
	_json.huntRiftSettings = huntRiftSettings;
	return _json;
}

void UP_DT_ZoneDataTable::LoadZoneDataTableJSON(const FZoneDataTableJSON& _json)
{
	LoadBaseDataTableJSON(_json);
	LoadPatternDataTableJSON(_json);

	bScaling = _json.bScaling;
	scalingSettings = _json.scalingSettings;
	spawnerSettings = _json.spawnerSettings;
	bSpawnerDefault = _json.bSpawnerDefault;
	spawnerDefaultSettings = _json.spawnerDefaultSettings;
	bSpawnerConcentric = _json.bSpawnerConcentric;
	spawnerConcentricSettings = _json.spawnerConcentricSettings;
	bSpawnerMulti = _json.bSpawnerMulti;
	spawnerMultiSettings = _json.spawnerMultiSettings;
	bEditProjectileGPE = _json.bEditProjectileGPE;
	projectileSettings = _json.projectileSettings;
	bEditFloorGPE = _json.bEditFloorGPE;
	floorDangerSettings = _json.floorDangerSettings;
	bEditHuntRiftGPE = _json.bEditHuntRiftGPE;
	huntRiftSettings = _json.huntRiftSettings;
}

void UP_DT_ZoneDataTable::InitializePatternData(FPatternData& _data) const
{
	Super::InitializePatternData(_data);
	_data.SetPatternDataTableType(EPatternDataTableType::PATTERN_ZONE);

	_data.SetScalingState(bScaling);
	_data.SetScalingSettings(scalingSettings);
	
	_data.SetSpawnerDefaultState(bSpawnerDefault);
	_data.SetSpawnerConcentricState(bSpawnerConcentric);
	_data.SetSpawnerMultiState(bSpawnerMulti);
	
	_data.SetSpawnerSettings(spawnerSettings);
	_data.SetSpawnerDefaultSettings(spawnerDefaultSettings);
	_data.SetSpawnerConcentricSettings(spawnerConcentricSettings);
	_data.SetSpawnerMultiSettings(spawnerMultiSettings);

	_data.SetEditProjectileState(bEditProjectileGPE);
	_data.SetEditFloorState(bEditFloorGPE);
	_data.SetEditHuntRiftState(bEditHuntRiftGPE);
	
	_data.SetGPEProjectileSettings(projectileSettings);
	_data.SetGPEFloorDangerSettings(floorDangerSettings);
	_data.SetGPEHuntRiftSettings(huntRiftSettings);
}
#endif