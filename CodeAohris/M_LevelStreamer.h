#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "M_LevelStreamer.generated.h"

UCLASS()
class INSIDEUS_API AM_LevelStreamer : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		UBoxComponent* triggerBox;

	UPROPERTY(EditAnywhere)
		UArrowComponent* arrowComponent;
	
	UPROPERTY(EditAnywhere, Category = "Levels | Array")
		TArray<TSoftObjectPtr<UWorld>> levelsToLoad = TArray<TSoftObjectPtr<UWorld>>();
	
	UPROPERTY(EditAnywhere, Category = "Levels | Array")
		TArray<TSoftObjectPtr<UWorld>> levelsToUnload = TArray<TSoftObjectPtr<UWorld>>();
	
	UPROPERTY()
		TArray<TSoftObjectPtr<UWorld>> currentLevelsToLoad = TArray<TSoftObjectPtr<UWorld>>();
	
	UPROPERTY()
		TArray<TSoftObjectPtr<UWorld>> currentLevelsToUnload = TArray<TSoftObjectPtr<UWorld>>();
	
	UPROPERTY()
		bool bCanLoad = false;
	
	UPROPERTY()
		bool bCanUnload = false;

	UPROPERTY()
		int iLoadIndex = 0;

	UPROPERTY()
		int iUnloadIndex = 0;
	
	
public:	
	AM_LevelStreamer();

	UFUNCTION(BlueprintCallable) void Rollback();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void StreamLevel(int _uuidOffset, FName _callbackFunction, TArray<TSoftObjectPtr<UWorld>> _levels, int& _index, bool& _bool, bool _load = true);
	void DetectOverlapLoad(FVector _location);
	void DetectOverlapUnload(FVector _location);
	
	UFUNCTION(BlueprintCallable) void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION(BlueprintCallable) void OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable) void EndLoad();
	UFUNCTION(BlueprintCallable) void EndUnload();

public:
	UFUNCTION() void UnloadLevelsBehindBoss();
};
