#pragma once
#include "Steam/isteamuserstats.h"

#include "STRUCT_LeaderboardEntry.generated.h"

USTRUCT()
struct FLeaderboardEntry
{
	GENERATED_BODY()
	
	LeaderboardEntry_t entry = LeaderboardEntry_t();

	FLeaderboardEntry() { }
	FLeaderboardEntry(LeaderboardEntry_t _entry) : entry(_entry)
	{ }
};
