#pragma once

#include "CoreMinimal.h"
#include "ENUM_BossPhase.h"
#include "GameFramework/SaveGame.h"

#include "ENUM_DifficultyType.h"
#include "GPE_SaveTrigger.h"
#include "STRUCT_BossProgress.h"
#include "S_GameSave.generated.h"


UCLASS()
class INSIDEUS_API US_GameSave : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Game Save | Save Info")
	int iSaveIndex = 0;
	UPROPERTY(EditAnywhere, Category = "Game Save | Save Info")
	FString saveName = "NONE";
	
	UPROPERTY(EditAnywhere, Category = "Game Save | Level")
	FName levelName = "NONE";
	
	UPROPERTY(EditAnywhere, Category = "Game Save | Difficulty")
	TEnumAsByte<EDifficultyType> difficulty = EDifficultyType::DIFFICULTY_BORN;

	UPROPERTY(VisibleAnywhere, Category = "Game Save | GPE")
	TMap<int32, FString> gpeData = { };

	UPROPERTY(VisibleAnywhere, Category = "Game Save | Player")
	float fExtraLife = 0;
	UPROPERTY(VisibleAnywhere, Category = "Game Save | Player")
	float fExtraSlowMo = 0;
	UPROPERTY(VisibleAnywhere, Category = "Game Save | Player")
		FTransform playerRespawnPoint = FTransform();
	UPROPERTY(VisibleAnywhere, Category = "Game Save | Player")
		bool progressDone = false;
	UPROPERTY(VisibleAnywhere, Category = "Game Save | Player")
		TSoftObjectPtr<AGPE_SaveTrigger> saveTrigger;
	UPROPERTY(VisibleAnywhere, Category = "Game Save | Player")
		TSoftObjectPtr<AGPE_SaveTrigger> saveTriggerDeath;

	UPROPERTY()
	TEnumAsByte<EBossPhase> bossPhase = EBossPhase::UN;

	//For steam success : To know if a gpe Shot has already been destroyed
	UPROPERTY(VisibleAnywhere, Category = "Game Save | Steam Success | Shot GPE destroyed")
		TMap<FString, bool> gpeShotData = { };

	//For steam success : To know if a gpe Shot has already been destroyed
	UPROPERTY(VisibleAnywhere, Category = "Game Save | Steam Success | Chest GPE destroyed")
		TMap<FString, bool> gpeChestData = { };

	//For steam success : To know if the level one intro cinematic was not skipped
	UPROPERTY(VisibleAnywhere, Category = "Game Save | Steam Success | Cinematic ")
		bool isLevelOneIntroCinematicNotSkipped = false;

	//For steam success : To know if the level three intro cinematic was not skipped
	UPROPERTY(VisibleAnywhere, Category = "Game Save | Steam Success | Cinematic ")
		bool isLevelThreeIntroCinematicNotSkipped = false;

	//For steam success : To know if the player got in the desert trigger
	UPROPERTY(VisibleAnywhere, Category = "Game Save | Steam Success | Exploration")
		bool isInDesertValidated = false;

	//For steam success : To know if the player got in the temple trigger
	UPROPERTY(VisibleAnywhere, Category = "Game Save | Steam Success | Exploration")
		bool isInTempleValidated = false;


public:
#pragma region Save Info
	FORCEINLINE const int& GetSaveIndex() const { return iSaveIndex; }
	FORCEINLINE const FString& GetSaveName() const { return saveName; }

	FORCEINLINE void SetSaveIndex(const int& _saveIndex) { iSaveIndex = _saveIndex; }
	FORCEINLINE void SetSaveName(const FString& _saveName) { saveName = _saveName; }
#pragma endregion

#pragma region Level 
	FORCEINLINE const FName& GetLevelName() const { return levelName; }
	
	FORCEINLINE void SetLevelName(const FName& _levelName) { levelName = _levelName; }
#pragma endregion

#pragma region Difficulty
	FORCEINLINE const TEnumAsByte<EDifficultyType>& GetDifficulty() const { return difficulty; }

	FORCEINLINE void SetDifficulty(const EDifficultyType& _difficulty) { difficulty = _difficulty; }	
#pragma endregion

#pragma region GPE
	FORCEINLINE const TMap<int32, FString>& GetGPEData() const { return gpeData; }
	FORCEINLINE TMap<FString, bool>& GetGPEShotData() { return gpeShotData; }
	FORCEINLINE TMap<FString, bool>& GetGPEChestData() { return gpeChestData; }

	FORCEINLINE void SetGPEData(const TMap<int32, FString>& _gpeData) { gpeData = _gpeData; }

	void AddGPEData(const int32& _gpeIndex, const FString& _data);
	void ClearGPEData();

	void CheckForTurretDeathSuccess(UWorld* TheWorldo);
#pragma endregion

#pragma region Player
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetExtraLife() const { return fExtraLife; }
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetExtraSlowMo() const { return fExtraSlowMo; }
	UFUNCTION(BlueprintCallable) FORCEINLINE FTransform& GetPlayerStartingTransform() { return playerRespawnPoint; }
	UFUNCTION(BlueprintCallable) FORCEINLINE AGPE_SaveTrigger* GetSaveTrigger() { return saveTrigger.Get(); }
	UFUNCTION(BlueprintCallable) FORCEINLINE AGPE_SaveTrigger* GetSaveTriggerDeath() { return saveTriggerDeath.Get(); }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetProgressDone() const { return progressDone; }
	
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetExtraLife(const float& _extraLife) { fExtraLife = _extraLife; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetExtraSlowMo(const float& _ExtraSlowMo) { fExtraSlowMo = _ExtraSlowMo; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetPlayerStartingTransform(const FTransform& _transform) { playerRespawnPoint = _transform; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetSaveTrigger(AGPE_SaveTrigger* _saveTrigger) { saveTrigger = _saveTrigger; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetSaveTriggerDeath(AGPE_SaveTrigger* _saveTrigger) { saveTriggerDeath = _saveTrigger; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetProgressDone(const bool& _status) { progressDone = _status; }



#pragma endregion

#pragma region Boss
	FORCEINLINE const TEnumAsByte<EBossPhase>& GetBossPhase() const { return bossPhase; }
	//Return a reference on BossProgress struct (allow to edit directly the struct)
	FORCEINLINE void SetBossPhase(const EBossPhase& _phase) { bossPhase = _phase; }
#pragma endregion

#pragma region SteamSuccess


	FORCEINLINE void SetIsInDesertValidated(bool _status) { isInDesertValidated = _status; }
	FORCEINLINE void SetIsInTempleValidated(bool _status) { isInTempleValidated = _status; }


	UFUNCTION(BlueprintCallable) void CheckForCinematicNotSkippedSuccess();
	UFUNCTION(BlueprintCallable) void CheckForExplorationAchievementsValidationInGameSave(UM_GameInstance* _gi);
	UFUNCTION(BlueprintCallable) FORCEINLINE void ValidateLevelOneIntroNotSkipped() { if (!isLevelOneIntroCinematicNotSkipped) isLevelOneIntroCinematicNotSkipped = true; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void ValidateLevelThreeIntroNotSkipped() { if (!isLevelThreeIntroCinematicNotSkipped) isLevelThreeIntroCinematicNotSkipped = true; }

	UFUNCTION() void CheckForOpenChestSuccess();

#pragma endregion
};
