#include "M_GameInstance.h"

#include "M_InputManager.h"

#include "S_GameSave.h"
#include "S_PlayerSave.h"

//#include "API_SteamIntegration.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"

#include "IU_Macros.h"

UM_GameInstance::UM_GameInstance()
{
	steamIntegration = CreateDefaultSubobject<UAPI_SteamIntegration>("steamIntegration");
}

void UM_GameInstance::QuitGame() const
{
	SaveCurrentGameSave();
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UM_GameInstance::Init()
{
	InitializeGameInstance();
	Super::Init();
	OnEnterMenuLevel();
}
void UM_GameInstance::Shutdown()
{
	TerminateGameInstance();
	
	Super::Shutdown();
}

void UM_GameInstance::InitializeGameInstance()
{
	InitializeInputManager();
	
	difficultyManager = NewObject<UM_DifficultyManager>(this, difficultyManagerType);
	
	userSettings = UGameUserSettings::GetGameUserSettings();

	const bool& _firstTime = !DoesSaveExist(playerSaveSlot); 
	playerSave = GetSave(playerSaveSlot, playerSaveType);
	InitUserSettings(_firstTime);
	
	steamIntegration->BeginSteam();
}
void UM_GameInstance::TerminateGameInstance() const
{
	SavePlayerSave();
	TerminateInputManager();
	steamIntegration->CloseSteam();
}

void UM_GameInstance::InitUserSettings(const bool _firstTime) const
{
	if (!_firstTime) return;
	
	const int32& _scalability = playerSave ? playerSave->GetDefaultScalability() : 2;	//Default Scalability will be High if player save if not valid (should not happen)
	userSettings->SetOverallScalabilityLevel(_scalability);
	userSettings->SetScreenResolution(userSettings->GetDesktopResolution());
	userSettings->SetFullscreenMode(EWindowMode::Fullscreen);
	userSettings->SetResolutionScaleNormalized(1);
	userSettings->ApplySettings(false);	
}

#pragma region Input Manager
void UM_GameInstance::InitializeInputManager()
{
	if (!inputManagerType.Get()) return;

	inputManager = NewObject<UM_InputManager>(this, inputManagerType);
	inputManager->InitializeGameInput();
}
void UM_GameInstance::TerminateInputManager() const
{
	if (!inputManager) return;

	inputManager->TerminateGameInput();
}
#pragma endregion

#pragma region Level
void UM_GameInstance::ReturnToMainMenu()
{
	ChangeLevel(UI_LEVEL);
	currentGameSave = nullptr;
	difficultyManager->SetDifficulty(EDifficultyType::DIFFICULTY_NONE);
}
void UM_GameInstance::ChangeLevel(const ELevelType& _levelType)
{
	if (!levels.Contains(_levelType)) return;

	SaveCurrentGameSave();
	
	onLevelWillChange.Broadcast();

	OnLeaveLevel(currentLevel);
	const FName& _levelName = levels[_levelType];
	UGameplayStatics::OpenLevel(GetWorld(), _levelName);
	currentLevel = _levelType;
	OnEnterLevel(currentLevel);	
	onLevelChanged.Broadcast(currentLevel);
}

void UM_GameInstance::OnLeaveLevel(const ELevelType& _levelType)
{
	if (_levelType == ELevelType::NONE_LEVEL) return;
	if (_levelType == ELevelType::UI_LEVEL) OnLeaveMenuLevel();
	if (_levelType == ELevelType::GAME_LEVEL) OnLeaveMainLevel();
	if (_levelType == ELevelType::RUSH_LEVEL) OnLeaveRushLevel();
}
void UM_GameInstance::OnEnterLevel(const ELevelType& _levelType)
{
	if (_levelType == ELevelType::NONE_LEVEL) return;
	if (_levelType == ELevelType::UI_LEVEL) OnEnterMenuLevel();
	if (_levelType == ELevelType::GAME_LEVEL) OnEnterMainLevel();
	if (_levelType == ELevelType::RUSH_LEVEL) OnEnterRushLevel();
}

void UM_GameInstance::ResetLevel()
{
	ChangeLevel(currentLevel);
}
#pragma endregion

#pragma region Save
US_GameSave* UM_GameInstance::GetGameSaveByIndex(const int& _saveIndex) const
{
	const FString& _slot = gameSaveSlot + FString::FromInt(_saveIndex);
	US_GameSave* _save = nullptr;
	if (DoesSaveExist(_slot))
	{
		_save = Cast<US_GameSave>(UGameplayStatics::LoadGameFromSlot(_slot, 0));
		_save->SetSaveIndex(_saveIndex);
	}

	return _save;
}

US_GameSave* UM_GameInstance::CreateGameSave(const int& _saveIndex)
{
	const FString& _slot = gameSaveSlot + FString::FromInt(_saveIndex);
	if (DoesSaveExist(_slot))
		UGameplayStatics::DeleteGameInSlot(_slot, 0);

	currentGameSave = GetSave(_slot, gameSaveType);
	currentGameSave->SetSaveIndex(_saveIndex);
	currentGameSave->SetSaveName("Save " + FString::FromInt(_saveIndex));
	
	SetSave(_slot, currentGameSave);
	return currentGameSave;
}
void UM_GameInstance::LoadGameSave(const int& _saveIndex)
{
	const FString& _slot = gameSaveSlot + FString::FromInt(_saveIndex);

	if (!DoesSaveExist(_slot))
		CreateGameSave(_saveIndex);
	else
		currentGameSave = GetSave(_slot, gameSaveType);

	GetDifficultyManager()->SetDifficulty(currentGameSave->GetDifficulty());
	
	LOG("Game Instance => Load GameSave : %s", *_slot)
}
void UM_GameInstance::DeleteGameSave(const int& _saveIndex)
{
	const FString& _slot = gameSaveSlot + FString::FromInt(_saveIndex);

	if (DoesSaveExist(_slot))
		UGameplayStatics::DeleteGameInSlot(_slot, 0);

	currentGameSave = nullptr;
}

bool UM_GameInstance::DoesGameSaveExist(const int& _saveIndex) const
{
	return DoesSaveExist(gameSaveSlot + FString::FromInt(_saveIndex));
}
bool UM_GameInstance::DoesSaveExist(const FString& _slot) const
{
	return UGameplayStatics::DoesSaveGameExist(_slot, 0);	
}

void UM_GameInstance::SavePlayerSave() const
{
	SetSave(playerSaveSlot, playerSave);
	LOG("Game Instance => Save PlayerSave")
}
void UM_GameInstance::SaveCurrentGameSave() const
{
	if (!currentGameSave) return;
	const FString& _slot = gameSaveSlot + FString::FromInt(currentGameSave->GetSaveIndex());
	//Set Current Level Name in currentGameSave
	//Set other (day etc)
	SetSave(_slot, currentGameSave);
	LOG("Game Instance => Save CurrentSaveGame : %s", *_slot)
}

void UM_GameInstance::SetSave(const FString& _slot, USaveGame* _save, const bool& _async) const
{
	if (!_save)
	{
		LOG("Game Instance => Can't save (nullptr)")
		return;
	}

	if (_async)
		UGameplayStatics::AsyncSaveGameToSlot(_save, _slot, 0);
	else
		UGameplayStatics::SaveGameToSlot(_save, _slot, 0);
}
#pragma endregion 