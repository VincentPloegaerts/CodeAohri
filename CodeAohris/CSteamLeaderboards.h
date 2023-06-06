#pragma once
#include "Steam/isteamuserstats.h"

class UAPI_SteamIntegration;

class CSteamLeaderboards
{
	SteamLeaderboard_t currentLeaderboard;					// Handle to leaderboard
	FName leaderboardName = FName("None");
	
public:
	int leaderboardEntries;									// How many entries do we have?
	LeaderboardEntry_t entries[10];							// The entries
	
public:
	CSteamLeaderboards();
	~CSteamLeaderboards() { }
	
	void FindLeaderboard(const FName& _leaderboardName);
	bool UploadScore(int score, const ELeaderboardUploadScoreMethod& _uploadMethod = k_ELeaderboardUploadScoreMethodKeepBest);
	bool UploadScore(UAPI_SteamIntegration* _steamIntegration, int score);
	
	bool DownloadScores(UAPI_SteamIntegration* _steamIntegration, const ELeaderboardDataRequest& _requestType = ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal, const int _min = 1, const int _max = 10);
	bool DownloadScoresInternal(UAPI_SteamIntegration* _steamIntegration, const ELeaderboardDataRequest& _requestType = ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal, const int _min = 1, const int _max = 10);
	
private:
	
	void FindLeaderboard(const char *pchLeaderboardName);
	
	void OnFindLeaderboard(LeaderboardFindResult_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardFindResult_t> callResultFindLeaderboard;
	void OnUploadScore(LeaderboardScoreUploaded_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardScoreUploaded_t> callResultUploadScore;
	void OnDownloadScore(LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardScoresDownloaded_t> callResultDownloadScore;
	
	CCallResult<UAPI_SteamIntegration, LeaderboardScoreUploaded_t> callResultUploadScoreIntegration;
	CCallResult<UAPI_SteamIntegration, LeaderboardScoresDownloaded_t> callResultDownloadScoreIntegration;
};
