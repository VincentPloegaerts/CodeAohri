#pragma once
#include "STRUCT_PatternElementData.h"

#include "STRUCT_SerieElementData.generated.h"

USTRUCT()
struct FSerieElementData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	TArray<FPatternElementData> patternsElementData;
	
	UPROPERTY(EditAnywhere)
	float fDelay = 0.0f;

	UPROPERTY(EditAnywhere)
	bool bWaitEndOfPatterns = false;

	void AddData(const FPatternElementData& _patternElementData)
	{
		patternsElementData.Add(_patternElementData);
	}
	void SetDelay(const float& _delay)
	{
		fDelay = _delay;
	}
	void SetIsWaiting(const bool& _isWaiting)
	{
		bWaitEndOfPatterns = _isWaiting;
	}
};