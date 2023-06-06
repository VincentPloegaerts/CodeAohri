#pragma once

#include "CoreMinimal.h"

#include "STRUCT_LeaderboardEntry.h"

#include "CSteamAchievements.h"
#include "CSteamLeaderboards.h"
#include "ENUM_BossPhase.h"

#include "Steam/steam_api.h"
#include "API_SteamIntegration.generated.h"

#define RAW_APP_ID "2200400"

class UAPI_SteamAchievementsManager;

UCLASS()
class INSIDEUS_API UAPI_SteamIntegration : public UObject
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeaderboardDownloadCompleted, const TArray<FLeaderboardEntry>&, _entries);
	
	UPROPERTY(EditAnywhere, Category = "Steam | Leaderboards")
	FName leaderboardTotalName = "LeaderboardTotal";
	UPROPERTY(EditAnywhere, Category = "Steam | Leaderboards")
	FName leaderboardBossOneName = "LeaderboardBossOne";
	UPROPERTY(EditAnywhere, Category = "Steam | Leaderboards")
	FName leaderboardBossTwoName = "LeaderboardBossTwo";
	UPROPERTY(EditAnywhere, Category = "Steam | Leaderboards")
	FName leaderboardBossThreeName = "LeaderboardBossThree";	
	
	UPROPERTY(EditAnywhere, Category = "Steam Values")
		bool bTryInitSteam = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Steam Values")
		bool bIsSteamEnabled = false;

	static constexpr char* APP_ID = RAW_APP_ID;
	CSteamID MyID;
	
	CSteamAchievements* steamAchievements = nullptr;

	UAPI_SteamAchievementsManager* steamAchievementsManager = nullptr;
	
	CSteamLeaderboards* leaderboardTotal = nullptr;
	TMap<EBossPhase, CSteamLeaderboards*> leaderboardBosses = 
	{
		{ EBossPhase::UN, nullptr },
		{ EBossPhase::DEUX, nullptr },
		{ EBossPhase::TROIS, nullptr },
	};

	TMap<EBossPhase, int> totalScore =
	{
		{ EBossPhase::UN, -1 },
		{ EBossPhase::DEUX, -1 },
		{ EBossPhase::TROIS, -1 },
	}; 
	
	UPROPERTY()
	FOnLeaderboardDownloadCompleted onLeaderboardDownloadCompleted;

public:
	FORCEINLINE FOnLeaderboardDownloadCompleted& OnLeaderboardDownloadCompleted() { return onLeaderboardDownloadCompleted; }
		
	FORCEINLINE const bool& GetIsSteamEnabled() const { return bIsSteamEnabled; }
	FORCEINLINE UAPI_SteamAchievementsManager* GetAchievementsManager() { return steamAchievementsManager; }
	UFUNCTION(BlueprintCallable) FORCEINLINE UAPI_SteamAchievementsManager* GetBPAchievementsManager() { return steamAchievementsManager; }
	
public:
	void BeginSteam();
	void CloseSteam();
	UFUNCTION() void RunCallback();

	UFUNCTION(BlueprintCallable) void ValidateSuccess(const FString name);	

	void InitializeLeaderboards();

	//Update leaderboard assign to given Phase and upload the score 
	bool UpdateLeaderboardBoss(const EBossPhase& _phase, const int _score);
	
	void DownloadTotalLeaderboard(const ELeaderboardDataRequest& _requestType = ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal, const int _min = 1, const int _max = 10);
	void DownloadBossLeaderboard(const EBossPhase& _phase, const ELeaderboardDataRequest& _requestType = ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal, const int _min = 1, const int _max = 10);

	//Call by Steam when a boss score is upload to a leader board
	void OnUploadScore(LeaderboardScoreUploaded_t* pResult, bool bIOFailure);

	//Call by Steam when score download is complete (Broadcast received entries) 
	void OnDownloadScore(LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);
	//Call by Steam when score download is complete
	void OnDownloadScoreInternal(LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);
	
private:
	bool InitSteam();

	//Download Boss score assign to given Phase (used for total score only)
	void DownloadBossScoreInternal(const EBossPhase& _phase);
	//Update to Total Leaderboard
	bool UpdateLeaderboardTotal(const int _score);
};
