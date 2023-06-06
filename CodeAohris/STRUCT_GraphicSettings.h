#pragma once
#include "STRUCT_GraphicSettings.generated.h"

USTRUCT()
struct FResolutions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Resolutions values")
	FString resolution = FString();

	UPROPERTY(EditAnywhere, Category = "Resolutions values")
	FString ratio = FString();

	FResolutions() { }
	FResolutions(const FString& _resolution, const FString& _ratio)
	{
		resolution = _resolution;
		ratio = _ratio;
	}
	FResolutions(const FString& _resolution)
	{
		FResolutions(_resolution, "");
	}
};