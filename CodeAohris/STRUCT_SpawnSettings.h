#pragma once
#include "ENUM_SpawnSettings.h"
#include "ENUM_SpawnDirectionMode.h"

#include "STRUCT_SpawnSettings.generated.h"

USTRUCT(BlueprintType)
struct FSpawnSettings
{
	GENERATED_BODY()

	/*Witch entity is used for spawning.*/
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TEnumAsByte<ESpawnSettings> eSpawnPosition;

	/*The name of the arena socket to spawn on.*/
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (EditCondition = "eSpawnPosition == ESpawnSettings::ARENA_POINT", EditConditionHides, GetOptions = "GetPointNames"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bRandom = false;
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (EditCondition = "eSpawnPosition == ESpawnSettings::ARENA_POINT && bRandom == false", EditConditionHides, GetOptions = "GetPointNames"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	FString spawnArenaPointName;
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (EditCondition = "eSpawnPosition == ESpawnSettings::ARENA_POINT && bRandom == true", EditConditionHides, GetOptions = "GetPointNames"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TArray<FString> randomArenaPoint;
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (EditCondition = "eSpawnPosition == ESpawnSettings::SOCKET", EditConditionHides, GetOptions = "GetSocketNames"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	FName spawnSocketName;

	/*random range around initial location (2D).*/
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite, meta = (UIMin = "0", ClampMin = "0", AllowPrivateAccess = True))
	float fRandomRange = 0;
	/*Vector added to the spawn location (after random range)*/
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	FVector vSpawnPositionOffset;
	
	/*If look a target or a direction.*/
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TEnumAsByte<ESpawnDirectionMode> eSpawnDirectionMode;

	/*Witch entity to look at.*/
	UPROPERTY(EditAnywhere,	Category = "Spawn Settings", meta = (EditCondition = "eSpawnDirectionMode != ESpawnDirectionMode::DIRECTION", EditConditionHides), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TEnumAsByte<ESpawnSettings> eSpawnDirection;
	UPROPERTY(EditAnywhere,	Category = "Spawn Settings", meta = (EditCondition = "eSpawnDirectionMode != ESpawnDirectionMode::DIRECTION", EditConditionHides), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bRotateOnlyZ = false;

	UPROPERTY(EditAnywhere,	Category = "Spawn Settings")
	bool bRotateToSpawnDirection = false;

	/*The name of the arena socket to look at.*/
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (EditCondition = "eSpawnDirection == ESpawnSettings::ARENA_POINT && eSpawnDirectionMode != ESpawnDirectionMode::DIRECTION", EditConditionHides, GetOptions = "GetPointNames"),BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	FString directionArenaPointName;
	/*The name of the arena socket to look at.*/
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (EditCondition = "eSpawnDirection == ESpawnSettings::SOCKET && eSpawnDirectionMode != ESpawnDirectionMode::DIRECTION", EditConditionHides, GetOptions = "GetSocketNames"),BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	FName directionSocketName;
	
	//LookTargetOffset
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True, EditCondition = "eSpawnDirectionMode == ESpawnDirectionMode::TARGET", EditConditionHides))
	FVector vLookTargetOffset;

	/*If target unused, look in the direction of SpawnPosition and add this offset.*/
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (EditCondition = "eSpawnDirectionMode == ESpawnDirectionMode::DIRECTION", EditConditionHides), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	FRotator rSpawnLocalDirectionOffset;

	FString GetRandomPoint() const
	{
		const int& _max = randomArenaPoint.Num();
		if (_max == 0)
			return FString("NONE");
		
		return randomArenaPoint[FMath::RandRange(0, _max - 1)];
	}

	FString GetSpawnPointName() const
	{
		if (eSpawnPosition == ESpawnSettings::ARENA_POINT)
			return bRandom ? GetRandomPoint() : spawnArenaPointName;
		else
			return spawnSocketName.ToString();
	}
	
	FString GetDirectionPointName() const
	{
		if (eSpawnDirection == ESpawnSettings::ARENA_POINT)
			return directionArenaPointName;
		else
			return directionSocketName.ToString();
	}
};
