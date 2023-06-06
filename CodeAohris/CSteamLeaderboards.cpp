#include "CSteamLeaderboards.h"

#include "API_SteamIntegration.h"

#include "IU_Macros.h"

CSteamLeaderboards::CSteamLeaderboards() :
currentLeaderboard(NULL),
leaderboardEntries(0)
{
}

void CSteamLeaderboards::FindLeaderboard(const FName& _leaderboardName)
{
	leaderboardName = _leaderboardName;
	FindLeaderboard(TCHAR_TO_ANSI(*leaderboardName.ToString()));
}
void CSteamLeaderboards::FindLeaderboard(const char* pchLeaderboardName)
{
	currentLeaderboard = NULL;

	const SteamAPICall_t _steamAPICall = SteamUserStats()->FindLeaderboard(pchLeaderboardName);
	callResultFindLeaderboard.Set(_steamAPICall, this, &CSteamLeaderboards::OnFindLeaderboard);
}
void CSteamLeaderboards::OnFindLeaderboard(LeaderboardFindResult_t* pResult, bool bIOFailure)
{
	// see if we encountered an error during the call
	if (!pResult->m_bLeaderboardFound || bIOFailure)
	{
		ON_SCREEN_LOG_TIME(10, FColor::Red, "Leaderboard could not be found");
		return;
	}

	currentLeaderboard = pResult->m_hSteamLeaderboard;
	
	ON_SCREEN_LOG_TIME(10, FColor::Green, *FString::Printf(TEXT("Leaderboard %s found"), *leaderboardName.ToString()));
}

bool CSteamLeaderboards::UploadScore(int score, const ELeaderboardUploadScoreMethod& _uploadMethod)
{
	if (!currentLeaderboard)
		return false;

	const SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore( currentLeaderboard, _uploadMethod, score, NULL, 0 );
	callResultUploadScore.Set(hSteamAPICall, this, &CSteamLeaderboards::OnUploadScore);
	
	ON_SCREEN_LOG_TIME_(10, "Uploading Score : %d sec.", score)
	
	return true;
}
bool CSteamLeaderboards::UploadScore(UAPI_SteamIntegration* _steamIntegration, int score)
{
	if (!currentLeaderboard)
		return false;

	const SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore( currentLeaderboard, k_ELeaderboardUploadScoreMethodKeepBest, score, NULL, 0 );
	callResultUploadScoreIntegration.Set(hSteamAPICall, _steamIntegration, &UAPI_SteamIntegration::OnUploadScore);
	
	ON_SCREEN_LOG_TIME_(10, "Uploading Score : %d sec.", score)
	
	return true;
}

void CSteamLeaderboards::OnUploadScore(LeaderboardScoreUploaded_t* pResult, bool bIOFailure)
{
	if ( !pResult->m_bSuccess || bIOFailure )
	{
		LOG( "Score could not be uploaded to Steam\n" );
	}
}


bool CSteamLeaderboards::DownloadScores(UAPI_SteamIntegration* _steamIntegration, const ELeaderboardDataRequest& _requestType, const int _min, const int _max)
{
	if (!currentLeaderboard)
		return false;

	ON_SCREEN_LOG_TIME_(10, "Downloading Scores in Leaderboard %s...", *leaderboardName.ToString());
	
	const SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(currentLeaderboard, _requestType, _min, _max);
	callResultDownloadScoreIntegration.Set(hSteamAPICall, _steamIntegration, &UAPI_SteamIntegration::OnDownloadScore);
	return true;
}

bool CSteamLeaderboards::DownloadScoresInternal(UAPI_SteamIntegration* _steamIntegration, const ELeaderboardDataRequest& _requestType, const int _min, const int _max)
{
	if (!currentLeaderboard)
		return false;

	ON_SCREEN_LOG_TIME_(10, "Downloading Scores in Leaderboard %s...", *leaderboardName.ToString());
	
	const SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(currentLeaderboard, _requestType, _min, _max);
	callResultDownloadScoreIntegration.Set(hSteamAPICall, _steamIntegration, &UAPI_SteamIntegration::OnDownloadScoreInternal);
	return true;
}

void CSteamLeaderboards::OnDownloadScore(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure)
{
	if (!bIOFailure)
	{
		leaderboardEntries = std::min(pResult->m_cEntryCount, 10);

		for (int index = 0; index < leaderboardEntries; index++)
		{
			SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, index, &entries[index], NULL, 0);
		}
		
		ON_SCREEN_LOG_TIME_(10, "Download Score complete : %d Entries in %s", leaderboardEntries, *leaderboardName.ToString());
	}
}

