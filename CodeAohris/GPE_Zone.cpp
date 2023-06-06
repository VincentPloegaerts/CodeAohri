#include "GPE_Zone.h"

#include "C_RespawnComponent.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "M_BlackBoardManager.h"
#include "M_FeedbackManager.h"
#include "M_GameMode.h"
#include "P_DT_ZoneDataTable.h"

AGPE_Zone::AGPE_Zone()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGPE_Zone::InitializeGPEBehavior(UP_DT_ZoneDataTable* _zoneDataTable)
{
	world = GetWorld();
	gameMode = world->GetAuthGameMode<AM_GameMode>();
	if (gameMode)
	{
		blackBoardManager = gameMode->GetBlackBoardManager();
		player = gameMode->GetPlayer();
		playerRespawn = player->RespawnComponent();
		timerManager = &world->GetTimerManager();

		timerManager->SetTimer(debugBossTimer, this, &AGPE_Zone::DebugBossStun, FMath::RandRange(1.0f, 3.0f),
			                                                                            true,
			                                                                            FMath::RandRange(1.0f, 3.0f));
		
		if (playerRespawn)
			Event_Add(playerRespawn->OnRespawn(), AGPE_Zone::CompleteBehavior);
	}
}

void AGPE_Zone::InitializeGPEBehavior(FGPE_GlobalSettings& _patternData)
{
	
}

void AGPE_Zone::InitFeedback(AM_FeedbackManager* _feedbackManager, const FSpawnerFeedBack& _feedback)
{
	feedbackManager = _feedbackManager;

	ROG_ERROR(!feedbackManager, "AGPE_Zone::InitFeedback -> invalid feedback Manager");
	
	if (const float& _delay = _feedback.fDelayFX)
	{
		FTimerDelegate _delegate;
		_delegate.BindUFunction(this, "PlayFeedback", _feedback);
		GetWorld()->GetTimerManager().SetTimer(feedbackTimer, _delegate, _delay, false);
	}
	else
		PlayFeedback(_feedback);
}

void AGPE_Zone::InitKnockBackSettings(const FKnockbackSettings& _knockBackSettings)
{
	knockbackSettings = _knockBackSettings;
}

void AGPE_Zone::BehaviorTick(const float& _deltaTime)
{
	
}

void AGPE_Zone::CompleteBehavior()
{
	Super::CompleteBehavior();
	if (playerRespawn)
		Event_Remove(playerRespawn->OnRespawn(), AGPE_Zone::CompleteBehavior);
	
	timerManager->ClearTimer(feedbackTimer);
	timerManager->ClearTimer(debugBossTimer);
}

void AGPE_Zone::PlayFeedback(const FSpawnerFeedBack& _feedback) const
{
	ROG_ERROR(!feedbackManager, "AGPE_Zone::PlayFeedback -> invalid feedback Manager");

	feedbackManager->RequestPlaySpawnerFeedback(_feedback, this);
}

void AGPE_Zone::DebugBossStun()
{
	blackBoardManager->UPDATE_DEBUG();
}