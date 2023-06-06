#pragma once

#include "STRUCT_ArenaCloserSettings.generated.h"

USTRUCT()
struct FSliceSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Arena Slice | Setting")
	float fDamage = 1; 
	UPROPERTY(EditAnywhere, Category = "Arena Slice | Setting")
	float fTimeBeforeClose = 1;
	UPROPERTY(EditAnywhere, Category = "Arena Slice | Setting")
	float fCloseTime = 2;
};

USTRUCT()
struct FArenaCloserSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Behavior | Arena Closer | Setting")
	bool bClosePlayerSlice = false;	
	UPROPERTY(EditAnywhere, Category = "Behavior | Arena Closer | Setting", meta = (GetOptions = "GetSliceNames"))
	TArray<FString> arenaSlices;
	UPROPERTY(EditAnywhere, Category = "Behavior | Arena Closer | Setting")
	FSliceSettings sliceSettings;
};
