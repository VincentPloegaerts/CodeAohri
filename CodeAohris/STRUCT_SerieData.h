#pragma once
#include "STRUCT_SerieElementData.h"

#include "STRUCT_SerieData.generated.h"

//Store every data of the serie
USTRUCT()
struct FSerieData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	TArray<FSerieElementData> serieElementDatas;
	
	UPROPERTY()
	unsigned int iIndex = 0;

public:
	void AddSerieElementData(FSerieElementData _elementData)
	{
		serieElementDatas.Add(_elementData);
	}
};