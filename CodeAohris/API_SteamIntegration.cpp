#include "API_SteamIntegration.h"
#include "API_SteamAchievementsManager.h"
#include "IU_Macros.h"

void UAPI_SteamIntegration::BeginSteam()
{
	FFileHelper::SaveStringToFile(TEXT(RAW_APP_ID), TEXT("steam_appid.txt"));
	
	if(bTryInitSteam)
		bIsSteamEnabled = InitSteam();
	
	if(bIsSteamEnabled)
	{
		MyID = SteamUser()->GetSteamID();

		
		const FString& _userName = SteamFriends()->GetFriendPersonaName(MyID);
		const FString& _msg = "My ID : " + FString::FromInt(MyID.GetAccountID()) + "/" + _userName;
		ON_SCREEN_LOG_TIME(10, FColor::Green, _msg);
		steamAchievements = new CSteamAchievements();
		steamAchievementsManager = NewObject<UAPI_SteamAchievementsManager>();

		InitializeLeaderboards();
	}
}
bool UAPI_SteamIntegration::InitSteam()
{
	if(SteamAPI_RestartAppIfNecessary(atoi(APP_ID)))
	{
		ON_SCREEN_LOG_TIME(10.0f, FColor::Red, "Steam : Restart App");
		return false;
	}

	if(!SteamAPI_Init())
	{
		ON_SCREEN_LOG_TIME(10.0f, FColor::Red, "Steam : Initialization Failed");
		return false;
	}

	ON_SCREEN_LOG_TIME(10.0f, FColor::Green,"Steam : Initialization Completed");
	return true;
}
void UAPI_SteamIntegration::CloseSteam()
{
	SteamAPI_Shutdown();
	
	return;
	if (steamAchievements)
		delete steamAchievements;
}
void UAPI_SteamIntegration::RunCallback()
{
	SteamAPI_RunCallbacks();
}

void UAPI_SteamIntegration::InitializeLeaderboards()
{
	leaderboardTotal = new CSteamLeaderboards();
	leaderboardTotal->FindLeaderboard(leaderboardTotalName);
	
	leaderboardBosses[EBossPhase::UN] =  new CSteamLeaderboards();;
	leaderboardBosses[EBossPhase::UN]->FindLeaderboard(leaderboardBossOneName);
	
	leaderboardBosses[EBossPhase::DEUX] =  new CSteamLeaderboards();;
	leaderboardBosses[EBossPhase::DEUX]->FindLeaderboard(leaderboardBossTwoName);
	
	leaderboardBosses[EBossPhase::TROIS] =  new CSteamLeaderboards();;
	leaderboardBosses[EBossPhase::TROIS]->FindLeaderboard(leaderboardBossThreeName);
}

bool UAPI_SteamIntegration::UpdateLeaderboardBoss(const EBossPhase& _phase, const int _score)
{
	if (!leaderboardBosses.Contains(_phase) || !leaderboardBosses[_phase]) return false;

	return leaderboardBosses[_phase]->UploadScore(this, _score);
}
bool UAPI_SteamIntegration::UpdateLeaderboardTotal(const int _score)
{
	if (leaderboardTotal)
		return leaderboardTotal->UploadScore(_score, k_ELeaderboardUploadScoreMethodForceUpdate);

	return false;
}

void UAPI_SteamIntegration::OnUploadScore(LeaderboardScoreUploaded_t* pResult, bool bIOFailure)
{
	//reset total score
	totalScore[EBossPhase::UN] = -1;
	totalScore[EBossPhase::DEUX] = -1;
	totalScore[EBossPhase::TROIS] = -1;

	DownloadBossScoreInternal(EBossPhase::UN);
}

void UAPI_SteamIntegration::DownloadTotalLeaderboard( const ELeaderboardDataRequest& _requestType, const int _min, const int _max)
{
	if (!leaderboardTotal) return;
	
	leaderboardTotal->DownloadScores(this, _requestType, _min, _max);
}
void UAPI_SteamIntegration::DownloadBossLeaderboard(const EBossPhase& _phase, const ELeaderboardDataRequest& _requestType, const int _min, const int _max)
{
	if (leaderboardBosses.Contains(_phase))
	{
		if (CSteamLeaderboards* _leaderboard = leaderboardBosses[_phase])
			_leaderboard->DownloadScores(this, _requestType, _min, _max);
	}
}
void UAPI_SteamIntegration::DownloadBossScoreInternal(const EBossPhase& _phase)
{
	if (CSteamLeaderboards* _leaderboard = leaderboardBosses[_phase])
		_leaderboard->DownloadScoresInternal(this, ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobalAroundUser, 0, 0);
}

void UAPI_SteamIntegration::OnDownloadScore(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure)
{
	if (!bIOFailure)
	{
		ON_SCREEN_LOG_TIME(10, FColor::Green, "Scores Download completed")
		
		const int _max = std::min(pResult->m_cEntryCount, 10);

		TArray<FLeaderboardEntry> _entries = { };
		
		for (int index = 0; index < _max; index++)
		{
			LeaderboardEntry_t _entry;
			SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, index, &_entry, NULL, 0);
			_entries.Add(_entry);
		}

		onLeaderboardDownloadCompleted.Broadcast(_entries);
	}
	else
		ON_SCREEN_LOG_TIME(10, FColor::Red, "Failed to download Scores")
}
void UAPI_SteamIntegration::OnDownloadScoreInternal(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure)
{
	LeaderboardEntry_t _entry;

	if (pResult->m_cEntryCount == 0)	//Entries should be 0 or 1 only (0 : No score) 
		_entry.m_nScore = 0;			//If user have no score for in the leaderboard set score value to 0 => change with 'return' if no update of Total if not all bosses have been defeated 
	else
		SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, 0, &_entry, NULL, 0);

	if (totalScore[EBossPhase::UN] == -1)
	{
		totalScore[EBossPhase::UN] = _entry.m_nScore;
		DownloadBossScoreInternal(EBossPhase::DEUX);
	}
	else if (totalScore[EBossPhase::DEUX] == -1)
	{
		totalScore[EBossPhase::DEUX] = _entry.m_nScore;
		DownloadBossScoreInternal(EBossPhase::TROIS);
	}
	else if (totalScore[EBossPhase::TROIS] == -1)
	{
		totalScore[EBossPhase::TROIS] = _entry.m_nScore;
		const int _total = totalScore[EBossPhase::UN] + totalScore[EBossPhase::DEUX] + totalScore[EBossPhase::TROIS];
		UpdateLeaderboardTotal(_total);
	}
}

void UAPI_SteamIntegration::ValidateSuccess(const FString name)
{
	const char* _charName = TCHAR_TO_ANSI(*name);
	
	if(steamAchievements)
		steamAchievements->SetAchievement(_charName);
}
