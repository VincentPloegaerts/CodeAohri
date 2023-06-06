#pragma once

#include "CoreMinimal.h"

class INSIDEUS_API CSteamAchievements
{
	
private:
	int64 iAppID;
	bool bInitialized;

public:
	CSteamAchievements();
	~CSteamAchievements();

	bool RequestStats();
	bool SetAchievement(const char* _ID);
};
