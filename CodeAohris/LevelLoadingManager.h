#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelLoadingManager.generated.h"

class ATriggerBoxLevelLoading;
UCLASS()
class INSIDEUS_API ALevelLoadingManager : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedLoading);

	//THIS WILL NOT LOAD IF 'ShouldLoadStartLevelFromManager' is not overrided
	//You can call 'AddLevels' on BeginPlau with the array if you're doing this from BP
	//Use 'TriggerBoxLevelLoading' or AddLevels for other load
	UPROPERTY(EditAnywhere,Category = "Level Loading",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		TArray<TSoftObjectPtr<UWorld>> levelsToLoadOnStart= TArray<TSoftObjectPtr<UWorld>>();
	//Delay between each load, loads are always unsynched meaning this delay just adds a duration after loading a level before the next one
	UPROPERTY(EditAnywhere,Category = "Level Loading",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float delayBetweenLoads = 0.0f;
	
	UPROPERTY(VisibleAnywhere,Category = "Level Loading|Debug",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		TArray<TSoftObjectPtr<UWorld>> levelsToLoadLeft = TArray<TSoftObjectPtr<UWorld>>();
	UPROPERTY(VisibleAnywhere,Category = "Level Loading|Debug",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		TArray<TSoftObjectPtr<UWorld>> levelsToUnloadLeft = TArray<TSoftObjectPtr<UWorld>>();
	UPROPERTY()
		FOnFinishedLoading onFinishedLoading;
	UPROPERTY()
		FTimerHandle handle;
public:	
	ALevelLoadingManager();
	//Add levels to the Load and Unload Array and start loading if there's not another one going on 
	UFUNCTION(BlueprintCallable) void AddLevels(TArray<TSoftObjectPtr<UWorld>> _toLoad,TArray<TSoftObjectPtr<UWorld>> _toUnload);
	//Called when everything have been loaded/unloaded
	FORCEINLINE FOnFinishedLoading& OnFinishedLoading() {return onFinishedLoading;}
	FORCEINLINE TArray<TSoftObjectPtr<UWorld>> LevelsToLoadOnStart() {return levelsToLoadOnStart;}
private:	
	virtual void BeginPlay() override;
	UFUNCTION() void Init();
	UFUNCTION() void InitTriggerBoxes(TArray<ATriggerBoxLevelLoading*> _triggerArray);
	UFUNCTION() void LoadLevel(const FString& _levelName);
	UFUNCTION() void UnloadLevel(const FString& _levelName);
	UFUNCTION() void LevelLoadStreamFinished();
	UFUNCTION() void LevelUnloadStreamFinished();
	UFUNCTION() void CallLoadingWithDelay(const FName& _functionName,const FString& _parameter,const float& _delay);
public:
	//override this if you want to load 'levelsToLoadOnStart' from this manager
	UFUNCTION() virtual bool ShouldLoadStartLevelFromManager();
};
