#pragma once
#include "STRUCT_PatternData.h"
#include "STRUCT_SpawnSettings.h"
#include "STRUCT_PatternElementData.generated.h"

USTRUCT()
struct FPatternElementData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FPatternData patternData;
	
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FSpawnSettings spawnSettings;

	FPatternElementData() {}

	FPatternElementData(const FPatternData& _patternData, const FSpawnSettings& _spawnSettings)
	{
		patternData = _patternData;
		spawnSettings = _spawnSettings;
	}
};