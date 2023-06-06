#pragma once

#include "CoreMinimal.h"
#include "TriggerBoxLevelLoading.generated.h"

class UBoxComponent;
class ALevelLoadingManager;
UCLASS()
class INSIDEUS_API ATriggerBoxLevelLoading : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UBoxComponent* triggerBoxForward = nullptr;

	UPROPERTY(EditAnywhere,Category = "Level Loading",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<TSoftObjectPtr<UWorld>> levelsToLoad = TArray<TSoftObjectPtr<UWorld>>();
	UPROPERTY(EditAnywhere,Category = "Level Loading",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<TSoftObjectPtr<UWorld>> levelsToUnload = TArray<TSoftObjectPtr<UWorld>>();
	UPROPERTY()
		ALevelLoadingManager* levelLoadingManagerRef = nullptr;
	UPROPERTY()
		bool bufferLoadLevels = false;
	
public:
	ATriggerBoxLevelLoading();
	UFUNCTION() void Init(ALevelLoadingManager* _levelLoadingManager);
	UFUNCTION() void AddLevels();
private:
	UFUNCTION() void OnCollisionForward(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* otherComp,
										int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);
	UFUNCTION() void OnCollisionBackward(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* otherComp,
										int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);
};
