#pragma once
#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/ArrowComponent.h"

#include "GPE_BaseGPE.h"
#include "GPE_Elevator.h"
#include "M_LevelStreamer.h"
#include "M_LevelStreamerDeluxeEdition.h"
#include "M_LevelThreeTriggerCinematic.h"
#include "GPE_SaveTrigger.generated.h"

class AGPE_Elevator;
class ATriggerBoxLevelLoading;
class AM_GameMode;
class AM_GameLogic;
class AIU_Player;

UCLASS()
class INSIDEUS_API AGPE_SaveTrigger : public AGPE_BaseGPE
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaved);
	
	UPROPERTY(EditAnywhere, Category = "Save trigger values", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UBoxComponent* triggerBox = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Savetrigger values", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		USceneComponent* respawnPoint = nullptr;

	UPROPERTY(EditAnywhere, Category = "Savetrigger values", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UBillboardComponent* billboardComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Savetrigger values", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UArrowComponent* arrow = nullptr;

	UPROPERTY(EditAnywhere, Category = "Savetrigger values", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bIsASaveTriggerForDeath = false;

	//boolean used to trigger in game Save Progression (ex : GPE status, last checkpoint used, etc) 
	UPROPERTY(EditAnywhere, Category = "Savetrigger values", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bSaveProgression = false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True)) //TODO REMOVE DEPRACATED
		TArray<TSoftObjectPtr<AM_LevelStreamer>> levelStreamers = TArray<TSoftObjectPtr<AM_LevelStreamer>>();
	
	UPROPERTY(EditAnywhere, Category = "Savetrigger values", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<TSoftObjectPtr<ATriggerBoxLevelLoading>> correspondingLevelStreamers;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		AM_GameMode* gameMode = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		AM_GameLogic* gameLogic = nullptr;
	UPROPERTY(BlueprintCallable,BlueprintAssignable, meta = (AllowPrivateAccess = True))
	FOnSaved onSaved;

	#pragma region Player LOD
	
	UPROPERTY(EditAnywhere, Category = "GPE")
		float fPlayerDetectionDistance = 8000.0f;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		AIU_Player* player = nullptr;

	FTimerManager* timerManager;

	#pragma endregion
	
	UPROPERTY(EditAnywhere, Category = "Savetrigger values") TSoftObjectPtr<AM_LevelThreeTriggerCinematic> cinematicLevelThreeTrigger;
	UPROPERTY(EditAnywhere, Category = "Savetrigger values") TSoftObjectPtr<AGPE_Elevator> gpeElevatorToActivate;
	
public:
	FORCEINLINE AM_LevelThreeTriggerCinematic* GetCinematicTrigger() const { return cinematicLevelThreeTrigger.Get(); }
	FORCEINLINE AGPE_Elevator* GetElevatorToActivate() const { return gpeElevatorToActivate.Get(); }
	
public:
	AGPE_SaveTrigger();

	void RollbackLevelStreamer();
	UFUNCTION(BlueprintCallable) FORCEINLINE TArray<TSoftObjectPtr<ATriggerBoxLevelLoading>> CorrespondingLevelStreamer() {return correspondingLevelStreamers;}
	FORCEINLINE FOnSaved& OnSaved() {return onSaved;}
private:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable) void OnCollision(AActor* _self,AActor* _other);
	UFUNCTION(BlueprintCallable) void GetPlayerGameMode();
	UFUNCTION(BlueprintCallable) void CheckPlayerDistance() const;

public:
	UFUNCTION() void ActivateSave();
};