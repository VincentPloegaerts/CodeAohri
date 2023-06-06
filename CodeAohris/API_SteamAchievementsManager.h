// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "API_SteamAchievementsManager.generated.h"

class UM_GameInstance;
/**
 * 
 */
UCLASS()
class INSIDEUS_API UAPI_SteamAchievementsManager : public UObject
{
	GENERATED_BODY()


#pragma region Exploration Success


		UPROPERTY(VisibleAnywhere, Category = "Settings | Exploration")
			int currentFallDeath = 0;

		UPROPERTY(VisibleAnywhere, Category = "Settings | Exploration")
			int currentChestOpened = 0;


#pragma endregion

#pragma region Platforming Success
#pragma endregion

#pragma region Boss Success
#pragma endregion


public :



FORCEINLINE void IncreaseCurrentFallDeath() { currentFallDeath++; }
FORCEINLINE void IncreaseCurrentChestOpened() { currentChestOpened++; }


public:
	UAPI_SteamAchievementsManager();

	UFUNCTION() void CheckForPlatformingAchievementsValidation(UM_GameInstance* _gi);
	
};
