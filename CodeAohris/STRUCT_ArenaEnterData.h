#pragma once
#include "STRUCT_ArenaEnterData.generated.h"

USTRUCT(BlueprintType)
struct FArenaEnterData
{
	GENERATED_BODY()
	
	// Arena enter location
	UPROPERTY(EditAnywhere, Category = "Arena enter values")
		FVector enterOfArenaLocation = FVector(0.0f);

	// Arena enter rotation
	UPROPERTY(EditAnywhere, Category = "Arena enter values")
		FRotator enterOfArenaRotation = FRotator(0.0f);
	
	FArenaEnterData() {}
};