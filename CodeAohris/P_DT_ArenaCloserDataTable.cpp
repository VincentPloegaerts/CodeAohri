#include "P_DT_ArenaCloserDataTable.h"

#include "STRUCT_PatternData.h"

#if WITH_EDITOR
void UP_DT_ArenaCloserDataTable::SavePattern()
{
	FArenaCloserDataTableJSON _json = GetArenaCloserDataTableJSON();

	UM_SaveManager::Save(_json, Save_Pattern, GetName());

}
void UP_DT_ArenaCloserDataTable::LoadPattern()
{
	FArenaCloserDataTableJSON _json;
	UM_SaveManager::Load(_json, Save_Pattern, GetName());

	LoadArenaCloserDataTableJSON(_json);
}

FArenaCloserDataTableJSON UP_DT_ArenaCloserDataTable::GetArenaCloserDataTableJSON() const
{
	FArenaCloserDataTableJSON _json;
	_json.InitBaseJSON(GetBaseDataTableJSON());
	
	_json.arenaCloserSettings = arenaCloserSettings;
	return _json;
}

void UP_DT_ArenaCloserDataTable::LoadArenaCloserDataTableJSON(const FArenaCloserDataTableJSON& _json)
{
	LoadBaseDataTableJSON(_json);

	arenaCloserSettings = _json.arenaCloserSettings;
}

void UP_DT_ArenaCloserDataTable::InitializePatternData(FPatternData& _data) const
{
	Super::InitializePatternData(_data);
	_data.SetPatternDataTableType(EPatternDataTableType::PATTERN_ARENA_CLOSER);
	_data.SetArenaCloserSettings(arenaCloserSettings);
}
#endif