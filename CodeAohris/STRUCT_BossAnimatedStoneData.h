#pragma once
#include "STRUCT_BossAnimatedStoneData.generated.h"

USTRUCT()
struct FBossAnimatedStoneData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector vLocationOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	bool bIsRelativeLocation = false;

	UPROPERTY(EditAnywhere)
	FRotator vRotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere)
	bool bAttachToPattern = false;
	UPROPERTY()
	AActor* requestingActor = nullptr;
};