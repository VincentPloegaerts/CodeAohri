#pragma once
#include "FMODBlueprintStatics.h"
#include "STRUCT_SongFeedBackIndex.generated.h"

class UFMODAudioComponent;

USTRUCT()
struct FSongFeedBackIndex
{
	GENERATED_BODY()

	UPROPERTY()
	UFMODAudioComponent* audioComponent = nullptr;

	UPROPERTY()
	FFMODEventInstance eventInstance = FFMODEventInstance();

	UPROPERTY()
	unsigned iPatternIndex = 0;
};