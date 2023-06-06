#include "BT_TN_CastPattern.h"

#include "BT_TN_CastPatternSerie.h"
#include "P_PatternCaster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "B_Boss.h"
#include "B_BossAnim.h"
#include "IU_Macros.h"
#include "M_BlackBoardManager.h"
#include "M_GameMode.h"
#include "P_DT_BaseDataTable.h"

UBT_TN_CastPattern::UBT_TN_CastPattern()
{
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBT_TN_CastPattern::InitCastTask()
{
	if (!patternCaster) return EBTNodeResult::Failed;
	
	if (bHasBeenReplacedWithDislodge)
	{
		pattern = replacedPattern;
		bWaitEndOfPattern = replacedWaitEndOfPattern;
		spawnSettings = replacedSpawnSettings;
		
		bHasBeenReplacedWithDislodge = false;
#if WITH_LOG_MOVEMENT_BOSS
		LOG("Caster : Replaced");
#endif
	}

#if DEBUG_BB
	LOG_ERROR("UBT_TN_CastPattern::InitCastTask");
#endif

	const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>();
	if (!_gameMode)
	{
		LOG("UBT_TN_CastPattern::InitCastTask -> Invalid gamemode");
		return EBTNodeResult::InProgress;
	}
	blackBoardManager = _gameMode->GetBlackBoardManager();
	if (!blackBoardManager)
	{
		LOG("UBT_TN_CastPattern::InitCastTask -> Invalid black board manager");
		return EBTNodeResult::InProgress;
	}
	blackBoardManager->UPDATE_DEBUG();

#if DEBUG_BB
	if (pattern)
		LOG("UBT_TN_CastPattern::InitCastTask -> Cast pattern %s %s", *GetName(), *pattern->GetName());
#endif
	
	//Avoid afk killer to override blackboard manager value.
	bIsSelfActorBoss = Cast<AB_Boss>(tree->GetBlackboardComponent()->GetValueAsObject("SelfActor")) ? true : false;
	const AB_Boss* _boss = patternCaster->GetBoss();
	if (!_boss)
	{
		LOG("UBT_TN_CastPattern::InitCastTask -> Invalid Boss");
		return EBTNodeResult::InProgress;
	}

	if (bIsSelfActorBoss)
		blackBoardManager->Set_DEBUG_Stun_TaskNodeCastPattern(this);
	else
		blackBoardManager->Set_DEBUG_Stun_TaskNodeCastPattern_AFK(this);

	if (bIsSelfActorBoss)//
	{//
		blackBoardManager->Set_OnBossMidLifeAnimStart_TaskNodeCastPattern(this);//
		blackBoardManager->Set_OnBossStunned_TaskNodeCastPattern(this);//
	}//
	else//
	{//
		blackBoardManager->Set_OnBossMidLifeAnimStart_TaskNodeCastPattern_AFK(this);//
		blackBoardManager->Set_OnBossStunned_TaskNodeCastPattern_AFK(this);//
	}//
	
	const UBlackboardComponent* _bb = tree->GetBlackboardComponent();
	if (_bb->GetValueAsBool("IsStun"))
	{
		
#if DEBUG_BB
		LOG_ERROR("UBT_TN_CastPattern::InitCastTask -> is stun");
#endif
		if (bIsSelfActorBoss)
		{
			//blackBoardManager->Set_OnReachGoal_TaskNodeCastPattern(this);//
			blackBoardManager->Set_OnBossStunnedEnd_TaskNodeCastPattern(this);
			blackBoardManager->Set_OnBossMidLifeAnimEnd_TaskNodeCastPattern(this);
		}
		else
		{
			blackBoardManager->Set_OnBossStunnedEnd_TaskNodeCastPattern_AFK(this);
			blackBoardManager->Set_OnBossMidLifeAnimEnd_TaskNodeCastPattern_AFK(this);
		}

#if WITH_LOG_STUN_BOSS
		LOG("CastPattern : Boss not init until stun end");
#endif
		return EBTNodeResult::InProgress;
	}

	/*if (bIsSelfActorBoss)
	{
		blackBoardManager->Set_OnBossMidLifeAnimStart_TaskNodeCastPattern(this);
		blackBoardManager->Set_OnBossStunned_TaskNodeCastPattern(this);
	}
	else
	{
		blackBoardManager->Set_OnBossMidLifeAnimStart_TaskNodeCastPattern_AFK(this);
		blackBoardManager->Set_OnBossStunned_TaskNodeCastPattern_AFK(this);
	}*/

	if (_bb->GetValueAsBool("NodeWillWaitForEndOfMovement"))
	{
		
#if DEBUG_BB
		LOG_ERROR("UBT_TN_CastPattern::InitCastTask -> is moving");
#endif
		const bool& _isMoving = _bb->GetValueAsBool("IsMoving");
		if (!_isMoving)
		{
			InitCastBehavior();
		}
		
		bMovementHasFinish = false;
		if (bIsSelfActorBoss)
		{
			blackBoardManager->Set_OnReachGoal_TaskNodeCastPattern(this);
			blackBoardManager->Set_OnReachGoal_TaskNodeCastPattern_IsMoving(_isMoving);
		}
		
		bossMovement = _boss->GetBossMovement();
	}
	else
	{
		
#if DEBUG_BB
		LOG_ERROR("UBT_TN_CastPattern::InitCastTask -> no movement");
#endif
		bMovementHasFinish = true;
		InitCastBehavior();
	}

	blackBoardManager->UPDATE_DEBUG();
	
	return EBTNodeResult::InProgress;
}

void UBT_TN_CastPattern::PostStunInitTask()
{
	const AB_Boss* _boss = patternCaster->GetBoss();
	ROG_ERROR(!_boss, "UBT_TN_CastPattern::PostStunInitTask -> Invalid Boss");

#if WITH_LOG_STUN_BOSS
	LOG("CastPattern : post stun init start");
#endif
#if DEBUG_BB
	LOG_ERROR("UBT_TN_CastPattern::PostStunInitTask");
#endif
	
	const UBlackboardComponent* _bb = tree->GetBlackboardComponent();
	if (_bb->GetValueAsBool("NodeWillWaitForEndOfMovement"))
	{
		
#if DEBUG_BB
		LOG_ERROR("UBT_TN_CastPattern::PostStunInitTask -> is moving");
#endif
		const bool& _isMoving = _bb->GetValueAsBool("IsMoving");
		if (!_isMoving)
		{
			InitCastBehavior();
		}
		
		bMovementHasFinish = false;
		
		//Avoid afk killer to override blackboard manager value.
		if (bIsSelfActorBoss)
		{
			blackBoardManager->Set_OnReachGoal_TaskNodeCastPattern(this);
			blackBoardManager->Set_OnReachGoal_TaskNodeCastPattern_IsMoving(_isMoving);
			
			//blackBoardManager->Set_OnBossStunnedEnd_TaskNodeCastPattern(nullptr);
			//blackBoardManager->Set_OnBossMidLifeAnimEnd_TaskNodeCastPattern(nullptr);
		}
		else
		{
			//blackBoardManager->Set_OnBossStunnedEnd_TaskNodeCastPattern_AFK(nullptr);
			//lackBoardManager->Set_OnBossMidLifeAnimEnd_TaskNodeCastPattern_AFK(nullptr);
		}
		bossMovement = _boss->GetBossMovement();
	}
	else
	{
		
#if DEBUG_BB
		LOG_ERROR("UBT_TN_CastPattern::PostStunInitTask -> No movement");
#endif
		bMovementHasFinish = true;
		InitCastBehavior();
	}

	blackBoardManager->UPDATE_DEBUG();
}

void UBT_TN_CastPattern::InitCastBehavior()
{
	ROG_ERROR(!pattern, "UBT_TN_CastPattern::InitCastBehavior -> You miss the pattern in a task node");
	
#if DEBUG_BB
	LOG_ERROR("UBT_TN_CastPattern::InitCastBehavior");
#endif
	
	blackBoardManager->UPDATE_DEBUG();
	
	pattern->GetTripleDeluxeFeedBack().bWaitEndOfPattern = bWaitEndOfPattern;
	
#if WITH_LOG_MOVEMENT_BOSS
	LOG("CastPattern : shoot %s on cast pattern %s", *pattern->GetName(), *GetName());
#endif
	if (bWaitEndOfPattern)
	{
		if (bIsSelfActorBoss)
			blackBoardManager->Set_OnPatternComplete_TaskNodeCastPattern(this);
		else
			blackBoardManager->Set_OnPatternComplete_TaskNodeCastPattern_AFK(this);

		patternCaster->CastPattern(pattern, spawnSettings, overridedFeedBack);
		return;
	}

	const float& _delay =  pattern->GetfDelay() + FMath::RandRange(0.0f,pattern->GetfRangeRandom());
	if (_delay > 0.0f)
	{
		patternCaster->CastPattern(pattern, spawnSettings, overridedFeedBack);
		
		GetWorld()->GetTimerManager().SetTimer(delayTimer,this, &UBT_TN_CastPattern::OnPatternBehaviourFinishedAndCheckMove,_delay,false);
		return;
	}

	patternCaster->CastPattern(pattern, spawnSettings, overridedFeedBack);
	OnPatternBehaviourFinishedAndCheckMove();
}

void UBT_TN_CastPattern::OnPatternBehaviourFinishedAndCheckMove()
{
	blackBoardManager->UPDATE_DEBUG();
	
	if (!bMovementHasFinish)
	{
		hasTryFinishButMovementIsNotEnd = true;
		return;
	}

#if DEBUG_BB
	LOG_ERROR("UBT_TN_CastPattern::OnPatternBehaviourFinishedAndCheckMove");
#endif
	
	if (patternCaster)
	{
		if (const AB_Boss* _boss = patternCaster->GetBoss())
		{
			if (bIsSelfActorBoss)
			{
				//blackBoardManager->Set_OnBossStunnedEnd_TaskNodeCastPattern(nullptr);
				//blackBoardManager->Set_OnBossMidLifeAnimEnd_TaskNodeCastPattern(nullptr);
				blackBoardManager->Set_OnPatternComplete_TaskNodeCastPattern(nullptr);
			
				//blackBoardManager->Set_OnBossStunned_TaskNodeCastPattern(nullptr);
				//blackBoardManager->Set_OnBossMidLifeAnimStart_TaskNodeCastPattern(nullptr);
			}
			else
			{
				//blackBoardManager->Set_OnBossStunnedEnd_TaskNodeCastPattern_AFK(nullptr);
				//blackBoardManager->Set_OnBossMidLifeAnimEnd_TaskNodeCastPattern_AFK(nullptr);
				blackBoardManager->Set_OnPatternComplete_TaskNodeCastPattern_AFK(nullptr);

				//blackBoardManager->Set_OnBossStunned_TaskNodeCastPattern_AFK(nullptr);
				//blackBoardManager->Set_OnBossMidLifeAnimStart_TaskNodeCastPattern_AFK(nullptr);
			}

			if (const UB_BossAnim* _bossAnim = _boss->GetBossAnim())
			{
#if WITH_LOG_ANIM_FEEDBACK
				LOG_ERROR("Caster : AnimState == %s", *UEnum::GetValueAsString(_bossAnim->GetAnimState()));
#endif
				if (_bossAnim->GetAnimState() != AnimState_None)
				{
#if WITH_LOG_ANIM_FEEDBACK
					LOG_ERROR("Caster : Delayed finish pattern");
#endif
					if (bIsSelfActorBoss)
						blackBoardManager->Set_OnAnimationEnd_TaskNodeCastPattern(this);
				}
				else
				{
#if WITH_LOG_ANIM_FEEDBACK
					LOG_ERROR("Caster : Instant finish pattern");
#endif
					if (patternCaster)
						patternCaster->BroadcastFinishedDataTable(pattern);
					WaitBossAnimAnimationEnd();
					return;
				}
			}
		}
	}

	if (patternCaster)
		patternCaster->BroadcastFinishedDataTable(pattern);
}

void UBT_TN_CastPattern::WaitBossAnimAnimationEnd()
{
	blackBoardManager->UPDATE_DEBUG();

#if DEBUG_BB
	LOG_ERROR("UBT_TN_CastPattern::WaitBossAnimAnimationEnd");
#endif
	
	if (bIsSelfActorBoss)
	{
		blackBoardManager->Set_OnAnimationEnd_TaskNodeCastPattern(nullptr);
		blackBoardManager->Set_DEBUG_Stun_TaskNodeCastPattern(nullptr);
	}
	else
	{
		blackBoardManager->Set_DEBUG_Stun_TaskNodeCastPattern_AFK(nullptr);
	}
	tree->OnTaskFinished(this, EBTNodeResult::Succeeded);
}

void UBT_TN_CastPattern::SetMovementHasFinishTrue()
{
#if WITH_LOG_MOVEMENT_BOSS
	LOG("Caster : Boss has finish moving");
#endif
#if DEBUG_BB
	LOG_ERROR("UBT_TN_CastPattern::SetMovementHasFinishTrue");
#endif

	blackBoardManager->UPDATE_DEBUG();
	
	bMovementHasFinish = true;

	if (hasTryFinishButMovementIsNotEnd)
	{
#if WITH_LOG_MOVEMENT_BOSS
		LOG("Caster : hasTryFinishButMovementIsNotEnd re try to finish pattern");
#endif
		OnPatternBehaviourFinishedAndCheckMove();
	}
}

void UBT_TN_CastPattern::SkipPattern()
{
#if DEBUG_BB
	LOG_ERROR("UBT_TN_CastPattern::SkipPattern");
#endif

	blackBoardManager->UPDATE_DEBUG();
	
	bMovementHasFinish = true;
	OnPatternBehaviourFinishedAndCheckMove();
}

void UBT_TN_CastPattern::ReplacePatternDataAndCastIt(const FPatternMovementData& _patternMovementData)
{
	bHasBeenReplacedWithDislodge = true;

	replacedPattern = pattern;
	pattern = _patternMovementData.pattern;
	
	replacedWaitEndOfPattern = bWaitEndOfPattern;
	bWaitEndOfPattern = _patternMovementData.bWaitForEndOfPattern;

	replacedSpawnSettings = spawnSettings;
	spawnSettings = _patternMovementData.spawnSettings;

	GetWorld()->GetTimerManager().ClearTimer(delayTimer);
	InitCastBehavior();
}

void UBT_TN_CastPattern::OverrideTripleFeedBack(const FTripleDeluxeFeedBack* _overrideFeedBack)
{
	overridedFeedBack = _overrideFeedBack;
}

void UBT_TN_CastPattern::PostInitProperties()
{
	Super::PostInitProperties();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	NodeName = FString("Cast Pattern");
#endif
}

FString UBT_TN_CastPattern::GetStaticDescription() const
{
	const FString& _patternName = pattern ? pattern->GetName() : FString("NONE");
	return FString("Cast Pattern : ") + _patternName + FString(" ");
}