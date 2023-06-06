#include "P_DT_WaveDataTable.h"

#include "STRUCT_PatternData.h"

#if WITH_EDITOR
void UP_DT_WaveDataTable::SavePattern()
{
	FWaveDataTableJSON _json = GetWaveDataTableJSON();

	UM_SaveManager::Save(_json, Save_Pattern, GetName());
}
void UP_DT_WaveDataTable::LoadPattern()
{
	FWaveDataTableJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Pattern, GetName());
	if (!_loadSuccess) return;

	LoadWaveDataTableJSON(_json);
}

FWaveDataTableJSON UP_DT_WaveDataTable::GetWaveDataTableJSON() const
{
	FWaveDataTableJSON _json;
	_json.InitBaseJSON(GetBaseDataTableJSON());

	_json.bOverrideWave = bOverrideWave;
	_json.waveSettings = waveSettings;
	return _json;
}
void UP_DT_WaveDataTable::LoadWaveDataTableJSON(const FWaveDataTableJSON& _json)
{
	LoadBaseDataTableJSON(_json);
	
	bOverrideWave = _json.bOverrideWave;
	waveSettings = _json.waveSettings;
}

void UP_DT_WaveDataTable::InitializePatternData(FPatternData& _data) const
{
	Super::InitializePatternData(_data);
	_data.SetPatternDataTableType(EPatternDataTableType::PATTERN_WAVE);

	_data.SetOverrideWave(bOverrideWave);
	_data.SetWaveSettings(waveSettings);
}
#endif