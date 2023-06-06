#include "CSteamAchievements.h"

#include "Steam/isteamuser.h"
#include "Steam/isteamuserstats.h"
#include "Steam/isteamutils.h"


CSteamAchievements::CSteamAchievements()
{
	bInitialized = false;
	iAppID = SteamUtils()->GetAppID();
	RequestStats();
}

CSteamAchievements::~CSteamAchievements() {}

#pragma region Steam Functions
bool CSteamAchievements::RequestStats()
{
	if (!SteamUserStats() || !SteamUser() )
		return false;
	
	if (!SteamUser()->BLoggedOn() )
		return false;

	bInitialized = true;
	return SteamUserStats()->RequestCurrentStats();
}

bool CSteamAchievements::SetAchievement(const char* _ID)
{
	if (bInitialized)
	{
		SteamUserStats()->SetAchievement(_ID);
		return SteamUserStats()->StoreStats();
	}
	
	return false;
}
#pragma endregion