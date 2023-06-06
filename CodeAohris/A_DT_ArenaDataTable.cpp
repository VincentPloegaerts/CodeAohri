#include "A_DT_ArenaDataTable.h"

TArray<FString> UA_DT_ArenaDataTable::GetArenaSpawnPoint() const
{
	TArray<FString> _names;
	
	_names.Append(arenaPointNames);
	_names.Append(arenaSliceNames);
	//_names.Append(arenaMovementPointNames);
	
	return _names;
}

void UA_DT_ArenaDataTable::SetPointNames(const TArray<FString>& _names)
{
	arenaPointNames = _names;
}

void UA_DT_ArenaDataTable::SetSliceNames(const TArray<FString>& _names)
{
	arenaSliceNames = _names;
}

void UA_DT_ArenaDataTable::SetMovementPointNames(const TArray<FString>& _names)
{
	arenaMovementPointNames = _names;
}