#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "ENUM_LevelType.h"
#include "M_DifficultyManager.h"

#include "API_SteamIntegration.h"
#include "B_Boss.h"
#include "M_GameInstance.generated.h"

class USaveGame;
class UM_InputManager;
class US_PlayerSave;
class US_GameSave;

UCLASS()
class INSIDEUS_API UM_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelWillChange);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChanged, TEnumAsByte<ELevelType>, _levelType);

	UPROPERTY()
	UGameUserSettings* userSettings = nullptr;

#pragma region Input
	UPROPERTY(EditAnywhere, Category = "Game Instance | Input Manager")
	TSubclassOf<UM_InputManager> inputManagerType;
	UPROPERTY(VisibleAnywhere, Category = "Values")
	UM_InputManager* inputManager = nullptr;
#pragma endregion

#pragma region Save	
	UPROPERTY(EditAnywhere, Category = "Game Instance | Player Save")
	FString playerSaveSlot = "PlayerSave";
	UPROPERTY(EditAnywhere, Category = "Game Instance | Player Save")
	TSubclassOf<US_PlayerSave> playerSaveType;
	UPROPERTY()
	US_PlayerSave* playerSave = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Game Instance | Game Save", meta = (UIMin = "1", UIMax = "10", ClampMin = "1", ClampMax = "10", ToolTip = "Only for UI New/Load Game Widget. 3D Save selection can only have 3 Save"))
	int iMaxSaveSlot = 3;
	UPROPERTY(EditAnywhere, Category = "Game Instance | Game Save")
	FString gameSaveSlot = "GameSave";
	UPROPERTY(EditAnywhere, Category = "Game Instance | Game Save")
	TSubclassOf<US_GameSave> gameSaveType;
	UPROPERTY()
	US_GameSave* currentGameSave = nullptr;
#pragma endregion 

#pragma region Difficulty
	/* The type of difficulty manager */
	UPROPERTY(EditAnywhere, Category = "Game Instance | Difficulty")
	TSubclassOf<UM_DifficultyManager> difficultyManagerType = TSubclassOf<UM_DifficultyManager>();

	/* The pointer of difficulty manager */
	UPROPERTY(VisibleAnywhere, Category = "Values")
	UM_DifficultyManager* difficultyManager = nullptr;
#pragma endregion 

#pragma region Steam
	/* The current steam integration */
	UPROPERTY(VisibleAnywhere, Instanced, Category = "Game Instance | Steam")
		UAPI_SteamIntegration* steamIntegration = nullptr;
#pragma endregion 

#pragma region Levels
	/* List of all levels names */
	UPROPERTY(EditAnywhere, Category = "Game Instance | Levels", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TMap<TEnumAsByte<ELevelType>, FName> levels = TMap<TEnumAsByte<ELevelType>, FName>();

	/* Type of the current level */
	UPROPERTY(VisibleAnywhere, Category = "Game Instance | Levels", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TEnumAsByte<ELevelType> currentLevel = UI_LEVEL; 
#pragma endregion 

	/* Called when the level is going to change */
	UPROPERTY()
		FOnLevelWillChange onLevelWillChange = FOnLevelWillChange();
	
	/* Called when the level as changed */
	UPROPERTY()
		FOnLevelChanged onLevelChanged = FOnLevelChanged();
	UPROPERTY(EditAnywhere, Category = "Game Instance | Boss Rush", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSoftObjectPtr<AB_Boss> currentStepBossRush;
	
public:
	FORCEINLINE UGameUserSettings* GetGameUserSettings() const { return userSettings; }
	FORCEINLINE TSoftObjectPtr<AB_Boss> GetCurrentStepBossRush() const { return currentStepBossRush; }
	FORCEINLINE void SetCurrentStepBossRush(TSoftObjectPtr<AB_Boss> _boss)  {currentStepBossRush = _boss; }

	UFUNCTION(BlueprintCallable) FORCEINLINE UM_InputManager* GetInputManager() const { return inputManager; }
	
	FORCEINLINE UM_DifficultyManager* GetDifficultyManager() const { return difficultyManager; }
	FORCEINLINE TEnumAsByte<ELevelType> GetCurrentLevel() const { return currentLevel; }
	FORCEINLINE FOnLevelWillChange* OnLevelWillChange() { return &onLevelWillChange; }
	FORCEINLINE FOnLevelChanged* OnLevelChanged() { return &onLevelChanged; }
	
	FORCEINLINE UAPI_SteamIntegration* GetSteamIntegration() const { return steamIntegration; }
	UFUNCTION(BlueprintCallable) UAPI_SteamIntegration* GetBPSteamIntegration() const { return steamIntegration; }
	
public:
	UM_GameInstance();

	void QuitGame() const;

private:
	virtual void Init() override;
	virtual void Shutdown() override;

	void InitializeGameInstance();
	void TerminateGameInstance() const;

	void InitUserSettings(const bool _firstTime) const;

	#pragma region Input
	void InitializeInputManager();
	void TerminateInputManager() const;
	#pragma endregion

	#pragma region Level
protected:
	void OnLeaveLevel(const ELevelType& _levelType);
	void OnEnterLevel(const ELevelType& _levelType);
	
	UFUNCTION(BlueprintImplementableEvent) void OnEnterMenuLevel();
	UFUNCTION(BlueprintImplementableEvent) void OnEnterMainLevel();
	UFUNCTION(BlueprintImplementableEvent) void OnEnterRushLevel();
	
	UFUNCTION(BlueprintImplementableEvent) void OnLeaveMenuLevel();
	UFUNCTION(BlueprintImplementableEvent) void OnLeaveMainLevel();
	UFUNCTION(BlueprintImplementableEvent) void OnLeaveRushLevel();
	
public:
	virtual void ReturnToMainMenu() override;
	void ChangeLevel(const ELevelType& _levelType);
	UFUNCTION(BlueprintCallable) void ResetLevel();
	#pragma endregion

	#pragma region Save
public:
	FORCEINLINE US_PlayerSave* GetPlayerSave() const { return playerSave; }
	FORCEINLINE TSubclassOf<US_PlayerSave> GetDefaultPlayerSave() const { return playerSaveType; }
	
	FORCEINLINE const int& GetMaxSaveSlot() const { return iMaxSaveSlot; }
	UFUNCTION(BlueprintCallable) FORCEINLINE US_GameSave* GetCurrentGameSave() const { return currentGameSave; }
	//Return the Game Save assign to the index (if the save does not exist, return nullptr)
	US_GameSave* GetGameSaveByIndex(const int& _saveIndex) const;

	//Create a new Game Save and assign it to the given index
	US_GameSave* CreateGameSave(const int& _saveIndex);
	//Load a Game Save assign with the given index
	void LoadGameSave(const int& _saveIndex);
	//Delete a Game Save assign with the given index (if Exist)
	void DeleteGameSave(const int& _saveIndex);
	//Check if a Game Save assign to and Index already exist
	bool DoesGameSaveExist(const int& _saveIndex) const;
	//Check if any Save assign to slot exist
	bool DoesSaveExist(const FString& _slot) const;

	void SavePlayerSave() const;
	void SaveCurrentGameSave() const;
	
private:
	template<class SaveClass>
	SaveClass* GetSave(const FString& _slot, const TSubclassOf<SaveClass>& _saveType) const
	{
		SaveClass* _save = nullptr;
		
		if (DoesSaveExist(_slot))
			_save = Cast<SaveClass>(UGameplayStatics::LoadGameFromSlot(_slot, 0));
		else
		{
			if (_saveType.Get())
			{
				_save = Cast<SaveClass>(UGameplayStatics::CreateSaveGameObject(_saveType));	
				SetSave(_slot, _save);
			}
			else	
				UE_LOG(LogTemp, Warning, TEXT("UM_GameInstance::GetSave => Save Type is not valid."));
		}
		return _save;
	}
	
	void SetSave(const FString& _slot, USaveGame* _save, const bool& _async = false) const;
	#pragma endregion
};