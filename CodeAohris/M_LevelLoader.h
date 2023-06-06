#pragma once

#include "CoreMinimal.h"
#include "M_CinematicManager.h"
#include "GameFramework/Actor.h"
#include "M_LevelLoader.generated.h"


UCLASS()
class INSIDEUS_API AM_LevelLoader : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Level Loader")
		bool bUseLoader = true;
	
	UPROPERTY(EditAnywhere, Category = "Level Loader | Load")
		TArray<TSoftObjectPtr<UWorld>> levelsToLoad = TArray<TSoftObjectPtr<UWorld>>();

	UPROPERTY(VisibleAnywhere, Category = "Level Loader | Load")
		int iLoadIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = "Level Loader | Load")
		bool bCanLoad = true;
	
	UPROPERTY(EditAnywhere, Category = "Level Loader | Unload")
		TArray<TSoftObjectPtr<UWorld>> levelsToUnload = TArray<TSoftObjectPtr<UWorld>>();

	UPROPERTY(VisibleAnywhere, Category = "Level Loader | Unload")
		int iUnloadIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = "Level Loader | Unload")
		bool bCanUnload = false;
	
public:	
	AM_LevelLoader();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	AM_CinematicManager* GetCinematicManager();
	void StreamLevel(int _uuidOffset, FName _callbackFunction, TArray<TSoftObjectPtr<UWorld>> _levels, int& _index, bool& _bool, bool _load = true);

	UFUNCTION() void EndLoad();
	UFUNCTION() void EndUnload();
	UFUNCTION(BlueprintCallable) void SetEndOfCinematic();
};
