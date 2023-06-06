#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GPE_PlatformDebugger.generated.h"

class AGPE_BreakablePlatform;

UCLASS()
class INSIDEUS_API AGPE_PlatformDebugger : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere) bool bHasFail = false;
	UPROPERTY(EditAnywhere)	float fCheckSpeed = 5.f;
	UPROPERTY(EditAnywhere)	TArray<TEnumAsByte<EObjectTypeQuery>> platformLayer;
	UPROPERTY(EditAnywhere)	TSubclassOf<AGPE_BreakablePlatform> platformToSpawn;

	UPROPERTY(EditAnywhere)	FVector vTargetLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)	FRotator rTargetRotation = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere)	FVector vTargetScale = FVector::OneVector;
	UPROPERTY(EditAnywhere)	float fPlayerDetectionRange = 8000.f;
	
public:
	AGPE_PlatformDebugger();

protected:
	virtual void BeginPlay() override;
	
	
	UFUNCTION() void CheckPlatform();
	UFUNCTION() void SpawnPlatform();
	
};