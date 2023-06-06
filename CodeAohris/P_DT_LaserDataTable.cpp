#include "P_DT_LaserDataTable.h"

#include "STRUCT_PatternData.h"

#if WITH_EDITOR
void UP_DT_LaserDataTable::SavePattern()
{
	FLaserDataTableJSON _json = GetLaserDataTableJSON();
	
	UM_SaveManager::Save(_json, Save_Pattern, GetName());
	
}
void UP_DT_LaserDataTable::LoadPattern()
{
	FLaserDataTableJSON _json;;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Pattern, GetName());
	if (!_loadSuccess) return;
	
	LoadLaserDataTableJSON(_json);
}

FLaserDataTableJSON UP_DT_LaserDataTable::GetLaserDataTableJSON() const
{
	FLaserDataTableJSON _json;
	_json.InitBaseJSON(GetBaseDataTableJSON());
	_json.InitPatternJSON(GetPatternDataTableJSON());
	
	_json.bEditLaser = bEditLaser;
	_json.laserSettings = laserSettings;
	return _json;
}

void UP_DT_LaserDataTable::LoadLaserDataTableJSON(const FLaserDataTableJSON& _json)
{
	LoadBaseDataTableJSON(_json);
	LoadPatternDataTableJSON(_json);

	bEditLaser = _json.bEditLaser;
	laserSettings = _json.laserSettings;
}

void UP_DT_LaserDataTable::InitializePatternData(FPatternData& _data) const
{
	Super::InitializePatternData(_data);
	_data.SetPatternDataTableType(EPatternDataTableType::PATTERN_LASER);
	_data.SetLaserState(bEditLaser);
	_data.SetLaserSettings(laserSettings);
}
#endif