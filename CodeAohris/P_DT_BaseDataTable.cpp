#include "P_DT_BaseDataTable.h"

#include "IU_Macros.h"
#include "M_SaveManager.h"
#include "STRUCT_PatternData.h"

#if WITH_EDITOR

void UP_DT_BaseDataTable::SavePattern()
{
	//Child Class
}
void UP_DT_BaseDataTable::LoadPattern()
{
	//Child Class
}

FBaseDataTableJSON UP_DT_BaseDataTable::GetBaseDataTableJSON() const
{
	FBaseDataTableJSON _json;
	_json.patternType = patternType;
	_json.tripleFeedBackData = tripleFeedBackData;
	_json.fDelay = fDelay;
	_json.fRangeRandom = fRangeRandom;
	_json.bDuplicateFX = bDuplicateFX;
	_json.duplication = duplication;
	_json.centerDuplication = centerDuplication;
	_json.duplicationOffset = duplicationOffset;
	return _json;
}

void UP_DT_BaseDataTable::LoadBaseDataTableJSON(const FBaseDataTableJSON& _json)
{
	patternType = _json.patternType;
	tripleFeedBackData = _json.tripleFeedBackData;
	fDelay = _json.fDelay;
	fRangeRandom = _json.fRangeRandom;
	bDuplicateFX = _json.bDuplicateFX;
	duplication = _json.duplication;
	centerDuplication = _json.centerDuplication;
	duplicationOffset = _json.duplicationOffset;
}

void UP_DT_BaseDataTable::InitializePatternData(FPatternData& _data) const
{
	_data.SetPatternName(GetName());
	_data.SetPatternType(patternType);
	_data.SetTripleFeedBackData(tripleFeedBackData);
	_data.SetDelay(fDelay);
	_data.SetRangeRandom(fRangeRandom);
	_data.SetDuplicationFX(bDuplicateFX);
	_data.SetDuplication(duplication);
	_data.SetCenterDuplication(centerDuplication);
	_data.SetDuplicationOffset(duplicationOffset);	
}
#endif