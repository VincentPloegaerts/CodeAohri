#include "M_GameMode.h"

#include "HUD_Game.h"
#include "M_GameLogic.h"
#include "M_WorldTimeManager.h"
#include "M_UIManager.h"
#include "M_BossManager.h"
#include "M_PatternsTickManager.h"
#include "M_GPE_PatternTickManager.h"
#include "M_FeedbackManager.h"
#include "M_CollectibleManager.h"
#include "M_CinematicManager.h"
#include "M_BlackBoardManager.h"
#include "M_LeaderboardManager.h"
#include "LevelLoadingManager.h"
#include "IU_Macros.h"
#include "M_GameInstance.h"
#include "S_GameSave.h"

void AM_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	world = GetWorld();
	InitializeManagers();
}

void AM_GameMode::InitializeManagers()
{
	gameLogic = CreateManager(gameLogicType, "Manager_GameLogic");
	if (gameLogic)
		gameLogic->InitializeManagers();

	worldTimeManager = CreateManager(worldTimeManagerType, "Manager_WorldTime");
	uiManager = CreateManager(uiManagerType, "Manager_UI");
	bossManager = CreateManager(bossManagerType, "Manager_Boss");
	patternsTickManager = CreateManager(patternsTickManagerType, "Manager_PatternTick");
	gpePatternTickManager = CreateManager(gpePatternTickManagerType, "Manager_GPEPatternTick");
	feedBackManager = CreateManager(feedBackManagerType, "Manager_Feedback");
	collectibleManager = CreateManager(collectibleManagerType, "Manager_Collectible");
	cinematicManager = CreateManager(cinematicManagerType, "Manager_Cinematic");
	blackBoardManager = CreateManager(blackBoardManagerType, "Manager_BlackBoard");
	leaderboardManager = CreateManager(leaderboardManagerType, "Manager_Leaderboard");
	levelLoadingManager = CreateManager(levelLoadingManagerType, "Manager_LevelLoading");
	if (levelLoadingManager)
		levelLoadingManager->OnFinishedLoading().AddUniqueDynamic(this, &AM_GameMode::FirstLoadFinished);

#if WITH_EDITOR

	if (!worldTimeManager)
		LOG("AM_GameMode::InitializeManagers -> Failed to create WorldTimeManager");

	if (!gameLogic)
		LOG("AM_GameMode::InitializeManagers -> Failed to create GameLogic");

	if (!patternsTickManager)
		LOG("AM_GameMode::InitializeManagers -> Failed to create PatternsTickManager");

	if (!gpePatternTickManager)
		LOG("AM_GameMode::InitializeManagers -> Failed to create GPE PatternTickManager");

	if (!feedBackManager)
		LOG("AM_GameMode::InitializeManagers -> Failed to create GPE FeedbackManager");

	if (!collectibleManager)
		LOG("AM_GameMode::InitializeManagers -> Failed to create GPE CollectibleManager");

	if (!bossManager)
		LOG("AM_GameMode::InitializeManagers -> Failed to create Boss Manager");

	if (!uiManager)
		LOG("AM_GameMode::InitializeManagers -> Failed to create UI Manager");

	if (!cinematicManager)
		LOG("AM_GameMode::InitializeManagers -> Failed to create Cinematic Manager");

	if (!blackBoardManager)
		LOG("AM_GameMode::InitializeManagers -> Failed to create BlackBoard Manager");

	if (!leaderboardManager)
		LOG("AM_GameMode::InitializeManagers -> Failed to create Leaderboard Manager");

	if (!levelLoadingManager)
		LOG("AM_GameMode::InitializeManagers -> Failed to create Level Loading Manager");
#endif
}

void AM_GameMode::FirstLoadFinished()
{
	levelLoadingManager->OnFinishedLoading().RemoveDynamic(this, &AM_GameMode::FirstLoadFinished);
	AHUD_Game* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Game>();
	if (const UM_GameInstance* _gi = GetGameInstance<UM_GameInstance>())
	{
		if (const US_GameSave* _gs = _gi->GetCurrentGameSave())
		{
			if (_gs->GetProgressDone())
			{
				if (_hud) _hud->ShowInitialWidget();
			}
			else if (cinematicManager)
			{
				cinematicManager->PlayLevelIntroCinematic();
			}
		}
		else
		{
			if (_hud) _hud->ShowInitialWidget();			
		}
	}	
}
