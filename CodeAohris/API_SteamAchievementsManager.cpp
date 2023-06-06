// Fill out your copyright notice in the Description page of Project Settings.


#include "API_SteamAchievementsManager.h"
#include "M_GameInstance.h"
#include "IU_Macros.h"


#pragma region Exploration Success
#pragma endregion

#pragma region Platforming Success
#pragma endregion

#pragma region Boss Success
#pragma endregion

UAPI_SteamAchievementsManager::UAPI_SteamAchievementsManager()
{
}

UFUNCTION() void UAPI_SteamAchievementsManager::CheckForPlatformingAchievementsValidation(UM_GameInstance* _gi)
{

	if (_gi)
	{
		if (currentFallDeath == 10)
			_gi->GetSteamIntegration()->ValidateSuccess("ACH_DIE_FALLING_10");
		if (currentFallDeath == 30)
			_gi->GetSteamIntegration()->ValidateSuccess("ACH_DIE_FALLING_30");
		if (currentFallDeath == 50)
			_gi->GetSteamIntegration()->ValidateSuccess("ACH_DIE_FALLING_50");

	}
	
}
