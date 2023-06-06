#include "B_DT_CritPointDataTable.h"

#include "M_SaveManager.h"

#if WITH_EDITOR
void UB_DT_CritPointDataTable::Save()
{
	FCritPointDataTableJSON _json;
	_json.critPointData = critPointData;

	UM_SaveManager::Save(_json, Save_CritPoint, GetName());
}
void UB_DT_CritPointDataTable::Load()
{
	FCritPointDataTableJSON _json;
	
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_CritPoint, GetName());
	if (!_loadSuccess) return;

	critPointData = _json.critPointData;
}
#endif
