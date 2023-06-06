#include "M_BlackBoardManager.h"

#include "IU_Macros.h"
#include "BT_D_BossMovement.h"
#include "BT_TN_CastPattern.h"
#include "B_AIBoss.h"
#include "B_AI_AFKKiller.h"
#include "B_Boss.h"
#include "B_BossAnim.h"
#include "B_BossMovement.h"
#include "M_GameMode.h"

AM_BlackBoardManager::AM_BlackBoardManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AM_BlackBoardManager::BeginPlay()
{
	timerManager = &GetWorld()->GetTimerManager();
}

void AM_BlackBoardManager::Broadcast_BossMovement_OnReachGoal()
{
	if (onReachGoal_BossAnim)
	{
		onReachGoal_BossAnim->StopPlayMovement();
	}

	if (onReachGoal_DecoratorBossMovement)
	{
		onReachGoal_DecoratorBossMovement->StartShoot();

		onReachGoal_DecoratorBossMovement = nullptr;
	}

	if (onReachGoal_TaskNodeCastPattern)
	{
#if DEBUG_BB
		LOG_ERROR("%s", *onReachGoal_TaskNodeCastPattern->GetName());
#endif
		onReachGoal_TaskNodeCastPattern->SetMovementHasFinishTrue();
		if (onReachGoal_TaskNodeCastPattern_IsMoving)
			onReachGoal_TaskNodeCastPattern->InitCastBehavior();
		else
			onReachGoal_TaskNodeCastPattern->OnPatternBehaviourFinishedAndCheckMove();
		
		onReachGoal_TaskNodeCastPattern = nullptr;
	}

	if (onReachGoal_BossMovement)
	{
#if WITH_LOG_MOVEMENT_BOSS && WITH_LOG_DEBUG
		onReachGoal_BossMovement->DEBUG_ReachGoalBroadcasted();
#endif
		
		if (onReachGoal_BossMovement_LookPlayer)
		{
			onReachGoal_BossMovement->Internal_SetLookPlayer();
			onReachGoal_BossMovement_LookPlayer = false;
		}
	}

#if DEBUG_BB
	LOG_ERROR("on goal reach");
	LOG_ERROR(" ");
#endif
	UPDATE_DEBUG();
}

void AM_BlackBoardManager::Broadcast_BossMovement_OnDislodgePlayer(const FPatternMovementData& _data)
{
	if (onDislodgePlayer_DecoratorBossMovement)
	{
		onDislodgePlayer_DecoratorBossMovement->SetOwnerCasterPattern(_data);
		
		onDislodgePlayer_DecoratorBossMovement = nullptr;
	}
	UPDATE_DEBUG();
}

void AM_BlackBoardManager::Broadcast_Boss_OnBossMidLifeAnimStart()
{
	if (onBossMidLifeAnimStart_TaskNodeCastPattern)
	{
		onBossMidLifeAnimStart_TaskNodeCastPattern->SkipPattern();
		
		//onBossMidLifeAnimStart_TaskNodeCastPattern = nullptr;
	}
	
	if (onBossMidLifeAnimStart_TaskNodeCastPattern_AFK)
	{
		onBossMidLifeAnimStart_TaskNodeCastPattern_AFK->SkipPattern();
		
		//onBossMidLifeAnimStart_TaskNodeCastPattern_AFK = nullptr;
	}
	
#if DEBUG_BB
	LOG_ERROR("midlife start");
#endif
	UPDATE_DEBUG();
}

void AM_BlackBoardManager::Broadcast_Boss_OnBossMidLifeAnimEnd()
{
	if (onBossMidLifeAnimEnd_AIBoss)
	{
		onBossMidLifeAnimEnd_AIBoss->EndStun();
	}

	if (onBossMidLifeAnimEnd_AIAFKKiller)
	{
		onBossMidLifeAnimEnd_AIAFKKiller->EndStun();
	}

	if (onBossMidLifeAnimEnd_DecoratorBossMovement)
	{
		onBossMidLifeAnimEnd_DecoratorBossMovement->Init(false);
		
		onBossMidLifeAnimEnd_DecoratorBossMovement = nullptr;
	}

	if (onBossMidLifeAnimEnd_TaskNodeCastPattern)
	{
		onBossMidLifeAnimEnd_TaskNodeCastPattern->PostStunInitTask();
		
		//onBossMidLifeAnimEnd_TaskNodeCastPattern = nullptr;
	}

	if (onBossMidLifeAnimEnd_TaskNodeCastPattern_AFK)
	{
		onBossMidLifeAnimEnd_TaskNodeCastPattern_AFK->PostStunInitTask();
		
		//onBossMidLifeAnimEnd_TaskNodeCastPattern_AFK = nullptr;
	}
	
#if DEBUG_BB
	LOG_ERROR("midlife end");
#endif
	UPDATE_DEBUG();
}

void AM_BlackBoardManager::Broadcast_Boss_OnBossStunned()
{
	if (onBossStunned_TaskNodeCastPattern)
	{
		onBossStunned_TaskNodeCastPattern->SkipPattern();
		
		//onBossStunned_TaskNodeCastPattern = nullptr;
	}
	
	if (IsValid(onBossStunned_TaskNodeCastPattern_AFK))
	{
		onBossStunned_TaskNodeCastPattern_AFK->SkipPattern();
		
		//onBossStunned_TaskNodeCastPattern_AFK = nullptr;
	}
	UPDATE_DEBUG();
}

void AM_BlackBoardManager::Broadcast_Boss_OnBossStunnedEnd()
{
	if (onBossStunnedEnd_DecoratorBossMovement)
	{
		onBossStunnedEnd_DecoratorBossMovement->Init(false);
		
		onBossStunnedEnd_DecoratorBossMovement = nullptr;
	}
		

	if (onBossStunnedEnd_TaskNodeCastPattern)
	{
		onBossStunnedEnd_TaskNodeCastPattern->PostStunInitTask();
		
		//onBossStunnedEnd_TaskNodeCastPattern = nullptr;
	}

	if (IsValid(onBossStunnedEnd_TaskNodeCastPattern_AFK))
	{
		onBossStunnedEnd_TaskNodeCastPattern_AFK->PostStunInitTask();
		
		//onBossStunnedEnd_TaskNodeCastPattern_AFK = nullptr;
	}
	UPDATE_DEBUG();
}

void AM_BlackBoardManager::Broadcast_Boss_OnBossReset()
{
	if (onBossReset_DecoratorBossMovement)
	{
		onBossReset_DecoratorBossMovement->UnActivate();
		
		onBossReset_DecoratorBossMovement = nullptr;
	}
	UPDATE_DEBUG();
}

void AM_BlackBoardManager::Broadcast_PatternCaster_OnPatternComplete()
{
	if (onPatternComplete_TaskNodeCastPattern)
	{
		onPatternComplete_TaskNodeCastPattern->OnPatternBehaviourFinishedAndCheckMove();
		
		onPatternComplete_TaskNodeCastPattern = nullptr;
	}
	
	if (onPatternComplete_TaskNodeCastPattern_AFK)
	{
		onPatternComplete_TaskNodeCastPattern_AFK->OnPatternBehaviourFinishedAndCheckMove();
		
		onPatternComplete_TaskNodeCastPattern_AFK = nullptr;
	}
	UPDATE_DEBUG();
}

void AM_BlackBoardManager::Broadcast_BossAnim_OnAnimationEnd()
{
	if (onAnimationEnd_TaskNodeCastPattern)
	{
		onAnimationEnd_TaskNodeCastPattern->WaitBossAnimAnimationEnd();
		
		onAnimationEnd_TaskNodeCastPattern = nullptr;
	}
	UPDATE_DEBUG();
}

void AM_BlackBoardManager::UPDATE_DEBUG()
{
	if (!bIsBossActivated)
	{
#if DEBUG_BB
		LOG_GREY("CANT UPDATE DEBUG");
#endif
		return;
	}
#if DEBUG_BB
	LOG_GREY("UPDATE DEBUG");
#endif
	timerManager->SetTimer(debugTimer, this, &AM_BlackBoardManager::DEBUG_BossLock, fDebugBossStunTime);
}

void AM_BlackBoardManager::STOP_DEBUG()
{
#if DEBUG_BB
	LOG_GREY("STOP DEBUG");
#endif
	timerManager->ClearTimer(debugTimer);
	debugStun_TaskNodeCastPattern = nullptr;
	debugStun_TaskNodeCastPattern_AFK = nullptr;
	debugStun_DecoratorBossMovement = nullptr;
	bIsBossActivated = false;
}

void AM_BlackBoardManager::DEBUG_BossLock()
{
	if (bShowDebugLog)
	{
		LOG_GREY("BOSS WAS LOCKED");
		LOG("BOSS WAS LOCKED");
		LOG_ERROR("BOSS WAS LOCKED");
	}

	if (!debugStun_TaskNodeCastPattern && !debugStun_TaskNodeCastPattern_AFK && !debugStun_DecoratorBossMovement)
	{
		//TODO reset behavior tree and every abonnement
		if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		{
			if (const AB_Boss* _boss = _gameMode->GetBoss())
			{
				if (const AB_AIBoss* _aiBoss = _boss->GetAIBoss())
				{
					if (UBrainComponent* _brain = _aiBoss->BrainComponent)
						_brain->RestartLogic();
				}
			}
		}
		
		return;
	}
	
	if (debugStun_TaskNodeCastPattern)
	{
		if (bShowDebugLog)
			LOG_ERROR("Current LOCKED Task Node %s", *debugStun_TaskNodeCastPattern->GetName());
		
		debugStun_TaskNodeCastPattern->SkipPattern();
		debugStun_TaskNodeCastPattern = nullptr;
	}
#if DEBUG_BB
	else
		LOG_GREY("AM_BlackBoardManager::DEBUG_BossLock -> No TaskNode to DEBUG");
#endif

	if (debugStun_TaskNodeCastPattern_AFK)
	{
		if (bShowDebugLog)
			LOG_ERROR("Current LOCKED Task Node AFK %s", *debugStun_TaskNodeCastPattern_AFK->GetName());
		
		debugStun_TaskNodeCastPattern_AFK->SkipPattern();
		debugStun_TaskNodeCastPattern_AFK = nullptr;
	}
#if DEBUG_BB
	else
		LOG_GREY("AM_BlackBoardManager::DEBUG_BossLock -> No TaskNode AFK to DEBUG");
#endif

	if (debugStun_DecoratorBossMovement)
	{
		if (bShowDebugLog)
			LOG_ERROR("Current LOCKED Decorator %s", *debugStun_DecoratorBossMovement->GetName());
		
		debugStun_DecoratorBossMovement->UnActivate();
		debugStun_DecoratorBossMovement = nullptr;
	}
#if DEBUG_BB
	else
		LOG_GREY("AM_BlackBoardManager::DEBUG_BossLock -> No Decorator to DEBUG");
#endif
}

void AM_BlackBoardManager::SetBossIsActivated()
{
	bIsBossActivated = true;
}