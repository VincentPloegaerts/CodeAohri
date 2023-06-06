#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GPE_TempleDoor.generated.h"

class UBoxComponent;
class AM_GameMode;

UCLASS()
class INSIDEUS_API AGPE_TempleDoor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,Category = "Level Loading",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<TSoftObjectPtr<UWorld>> levelsToLoad = TArray<TSoftObjectPtr<UWorld>>();
	UPROPERTY(EditAnywhere,Category = "Level Loading",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<TSoftObjectPtr<UWorld>> levelsToUnload = TArray<TSoftObjectPtr<UWorld>>();
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UBoxComponent* triggerBoxDoor = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		AM_GameMode* gameMode = nullptr;
public:	
	AGPE_TempleDoor();
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable) void LoadAndUnloadLevels();
};
