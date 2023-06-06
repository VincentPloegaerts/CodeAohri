#include "S_GameSave.h"
#include "M_GameInstance.h"

#pragma region GPE
void US_GameSave::AddGPEData(const int32& _gpeIndex, const FString& _data)
{
	if (gpeData.Contains(_gpeIndex))
		gpeData[_gpeIndex] = _data;
	else
		gpeData.Add(_gpeIndex, _data);
}
void US_GameSave::ClearGPEData()
{
	gpeData.Empty();
}
void US_GameSave::CheckForTurretDeathSuccess(UWorld* TheWorldo)
{
	int _count = 0;

	const int& _max = gpeShotData.Num();
	for (const TTuple<FString, bool>& current : gpeShotData)
	{
		if (current.Value)
		{
			_count++;

		}
	}

	if (_count == _max)
	{
		
		if (const UM_GameInstance* _gameInstance = TheWorldo->GetGameInstance<UM_GameInstance>())
		{
			if(UAPI_SteamIntegration* _steamAchievementManager = _gameInstance->GetSteamIntegration())
				_steamAchievementManager->ValidateSuccess("ACH_PILE_RUBBLE");
		}
	}
}
#pragma endregion 


UFUNCTION(BlueprintCallable) void US_GameSave::CheckForCinematicNotSkippedSuccess()
{
	if (isLevelOneIntroCinematicNotSkipped && isLevelThreeIntroCinematicNotSkipped)
	{
		UWorld* world = GEngine->GameViewport->GetWorld();

		if (!world)return;

		if (const UM_GameInstance* _gameInstance = world->GetGameInstance<UM_GameInstance>())
		{
			if (UAPI_SteamIntegration* _steamAchievementManager = _gameInstance->GetSteamIntegration())

				_steamAchievementManager->ValidateSuccess("ACH_NOT_PASS");
		}
	}
	
}

UFUNCTION(BlueprintCallable) void US_GameSave::CheckForExplorationAchievementsValidationInGameSave(UM_GameInstance* _gi)
{
	if (_gi)
	{
		if (isInDesertValidated && isInTempleValidated)
		{
			_gi->GetSteamIntegration()->ValidateSuccess("ACH_TRAVELLER_SEA_CLOUD");

		}
	}
}

UFUNCTION() void US_GameSave::CheckForOpenChestSuccess()
{
	int _count = 0;

	UWorld* _world = GEngine->GameViewport->GetWorld();

	const int& _max = gpeChestData.Num();
	for (const TTuple<FString, bool>& current : gpeChestData)
	{
		if (current.Value)
		{
			_count++;

		}
	}

	if (_count == _max)
	{

		if (const UM_GameInstance* _gameInstance = _world->GetGameInstance<UM_GameInstance>())
		{

			if (UAPI_SteamIntegration* _steamAchievementManager = _gameInstance->GetSteamIntegration())
				_steamAchievementManager->ValidateSuccess("ACH_TREASURE_PASSION");
		}
	}
}
