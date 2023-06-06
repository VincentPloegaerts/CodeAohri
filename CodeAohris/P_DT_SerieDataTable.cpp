#include "P_DT_SerieDataTable.h"

#include "IU_Macros.h"

#if WITH_EDITOR
FSerieData UP_DT_SerieDataTable::GetSerieData() const
{
	FSerieData _serieData;
	
	const int& _max = serieElements.Num();
	for (int i = 0; i < _max; i++)
	{
		const FSerieElement& _serieElement = serieElements[i];
		_serieData.AddSerieElementData(GetSerieElementData(_serieElement));
	}
	
	return _serieData;
}

FSerieElementData UP_DT_SerieDataTable::GetSerieElementData(const FSerieElement& _rip) const
{
	FSerieElementData _serieElementData;
       
	const int& _max = _rip.patterns.Num();
	for (int i = 0; i < _max; ++i)
	{
		const UP_DT_BaseDataTable* _pattern = _rip.patterns[i];
		if (!_pattern) continue;
           
		FPatternData _patternData;
		_pattern->InitializePatternData(_patternData);
       
		const FPatternElementData _patternElementData(_patternData, _rip.GetSpawnSettings());
       
		_serieElementData.AddData(_patternElementData);
	}

	return _serieElementData;
};

void UP_DT_SerieDataTable::SaveSerie()
{
	FSerieDataTableJSON _json;
	_json.serieElements = serieElements;
	_json.arenaDataTable = arenaDataTable;

	UM_SaveManager::Save(_json, Save_Serie, GetName());
}

void UP_DT_SerieDataTable::LoadSerie()
{
	FSerieDataTableJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Serie, GetName());
	if (!_loadSuccess) return;
	
	serieElements = _json.serieElements;
	arenaDataTable = _json.arenaDataTable;
}

TArray<FString> UP_DT_SerieDataTable::GetPointNames() const
{
	if (arenaDataTable)
		return arenaDataTable->GetArenaSpawnPoint();
	return TArray<FString> {};
}
#endif