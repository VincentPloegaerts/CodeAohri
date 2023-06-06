#pragma once
#include "STRUCT_FeedBackData.h"
#include "STRUCT_FeedBackActor.generated.h"

USTRUCT()
struct FFeedBackActor
{
	GENERATED_BODY()

	UPROPERTY()
	const AActor* requestingActor = nullptr;

	UPROPERTY()
	FFeedBackData feedBack = FFeedBackData();
};