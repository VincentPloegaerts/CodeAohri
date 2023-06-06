#include "BT_D_BossMovement.h"

#include "A_DT_ArenaDataTable.h"
#include "BT_BlackBoard.h"
#include "BT_TN_CastPattern.h"
#include "BT_TN_CastPatternSerie.h"
#include "B_Boss.h"
#include "B_BossMovement.h"
#include "IU_Macros.h"
#include "M_GameMode.h"
#include "B_BossAnim.h"
#include "M_BlackBoardManager.h"

UBT_D_BossMovement::UBT_D_BossMovement()
{
	bNotifyActivation = true;
	bNotifyDeactivation = true;
	bShootBeforeMoving = false;
}

void UBT_D_BossMovement::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);
	bShootBeforeMoving = false;

#if WITH_LOG_MOVEMENT_BOSS
	LOG(" ");
	LOG("----------------------------------------------------------------------------------------------------");
	LOG(" ");
#endif

#if DEBUG_BB
	LOG("UBT_D_BossMovement::OnNodeActivation");
#endif
	
	blackBoard = Cast<UBlackboardComponent>(SearchData.OwnerComp.GetBlackboardComponent());
	blackBoard->SetValueAsBool("NodeWillWaitForEndOfMovement", true);
	const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>();
	blackBoardManager = _gameMode->GetBlackBoardManager();

	blackBoardManager->UPDATE_DEBUG();
	blackBoardManager->Set_DEBUG_Stun_DecoratorBossMovement(this);
	
	if (blackBoard->GetValueAsBool(KEY_STUN))
	{
		boss = _gameMode->GetBoss();
		if (boss && blackBoardManager)
		{
#if WITH_LOG_MOVEMENT_BOSS
			LOG("D_Movement : Wait end of stun to init");
#endif

			blackBoardManager->Set_OnBossMidLifeAnimEnd_DecoratorBossMovement(this);
			blackBoardManager->Set_OnBossStunnedEnd_DecoratorBossMovement(this);
			Init(true);
		}
	}
	else
	{
		Init(false);
	}
}

void UBT_D_BossMovement::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	Super::OnNodeDeactivation(SearchData, NodeResult);
#if WITH_LOG_MOVEMENT_BOSS
	LOG("D_Movement : Deactivation");
#endif
	UnActivate();
}

void UBT_D_BossMovement::Init(bool _isstun)
{
#if DEBUG_BB
	LOG("UBT_D_BossMovement::Init");
#endif

	blackBoardManager->UPDATE_DEBUG();
	
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		boss = _gameMode->GetBoss();
		if (boss)
		{
			bossMovement = boss->GetBossMovement();
			bossAnim = boss->GetBossAnim();
			
			ROG(!bossMovement || !bossAnim || !blackBoardManager, "UBT_D_BossMovement::Init -> boss Movement is nullptr or doesn't have the good animation");
			
			blackBoardManager->Set_OnBossReset_DecoratorBossMovement(this);
			blackBoardManager->Set_OnReachGoal_DecoratorBossMovement(this);
			
			blackBoardManager->Set_OnDislodgePlayer_DecoratorBossMovement(this);

			if (!_isstun)
			{
				bossMovement->SetActivatePatternRotation(bLookShootedPattern, bLookPlayer);
			
				StartMovement();
			}
		}
	}
}

void UBT_D_BossMovement::StartMovement()
{
#if DEBUG_BB
	LOG("UBT_D_BossMovement::StartMovement");
#endif
	SetIsMovingKeyTrue();
	
	if (bossMovementMode == MOVE_ON_GROUND)
	{
		if (bActivatePlayerDetection)
			bossMovement->SetActivatePlayerDetection(bOverridePlayerDetection, fPlayerDetectionRange);
		bossMovement->SetMoveOnGround(bIfAlreadyOnPointJustWait, bLookGoalMovement, bIsRandomPoint, arenaPointName, randomArenaPointName, fSpeed, fRandomRange);
	}
	
	else if (bossMovementMode == MOVE_IN_GROUND)
	{
		if (bActivatePlayerDetection)
			bossMovement->SetActivatePlayerDetection(bOverridePlayerDetection, fPlayerDetectionRange);
		bossMovement->SetMoveInGround(bIfAlreadyOnPointJustWait, bLookGoalMovement, bIsRandomPoint, arenaPointName, randomArenaPointName, fSpeed, fRandomRange);
	}
	
	else if (bossMovementMode == MOVE_IN_GROUND_BEHIND_PLAYER)
	{
		bossMovement->SetMoveInGroundBehindPlayer(bLookGoalMovement, fSpeed);
	}
	
	else if (bossMovementMode == MOVE_IN_GROUND_BEHIND_PLAYER_DYNAMIC)
	{
		bossMovement->SetMoveInGroundBehindPlayerDynamic(bLookGoalMovement, fSpeed);
	}
	
	else if (bossMovementMode == DASH_IN_PLAYER_DIRECTION)
	{
		if (bActivatePlayerDetection)
			bossMovement->SetActivatePlayerDetection(bOverridePlayerDetection, fPlayerDetectionRange);
		bossMovement->SetDashInPlayerDirection(bLookGoalMovement, fSpeed);
	}
	
	else if (bossMovementMode == DASH_NEAREST_FROM_BOSS)
	{
		if (bActivatePlayerDetection)
			bossMovement->SetActivatePlayerDetection(bOverridePlayerDetection, fPlayerDetectionRange);
		bossMovement->SetDashNearestFromBoss(bLookGoalMovement, fSpeed);
	}
	
	else if (bossMovementMode == DASH_FAREST_FROM_PLAYER)
	{
		if (bActivatePlayerDetection)
			bossMovement->SetActivatePlayerDetection(bOverridePlayerDetection, fPlayerDetectionRange);
		bossMovement->SetDashFarestFromPlayer(bLookGoalMovement, fSpeed);
	}

	else if (bossMovementMode == DASH_ON_GROUND)
	{
		if (bActivatePlayerDetection)
			bossMovement->SetActivatePlayerDetection(bOverridePlayerDetection, fPlayerDetectionRange);
		bossMovement->SetDashOnGround(bIfAlreadyOnPointJustWait, bLookGoalMovement, bIsRandomPoint, arenaPointName, randomArenaPointName, fSpeed, fRandomRange);
	}
	
	blackBoardManager->UPDATE_DEBUG();

#if WITH_LOG_MOVEMENT_BOSS
	LOG("D_Movement : %s", *UEnum::GetValueAsString(bossMovementMode));
#endif
}

void UBT_D_BossMovement::StartShoot()
{
#if WITH_LOG_MOVEMENT_BOSS
	LOG("D_Movement : Start Shooting");
#endif
#if DEBUG_BB
	LOG("UBT_D_BossMovement::StartShoot");
#endif
	blackBoardManager->UPDATE_DEBUG();
	SetIsMovingKeyFalse();
}

void UBT_D_BossMovement::SetIsMovingKeyTrue()
{
#if DEBUG_BB
	LOG("UBT_D_BossMovement::SetIsMovingKeyTrue");
#endif
	blackBoardManager->UPDATE_DEBUG();
	blackBoard->SetValueAsBool("IsMoving", true);
}

void UBT_D_BossMovement::SetIsMovingKeyFalse()
{
#if WITH_LOG_MOVEMENT_BOSS
	LOG("D_Movement : Set Boss has finish moving");
#endif
#if DEBUG_BB
	LOG("UBT_D_BossMovement::SetIsMovingKeyFalse");
#endif
	blackBoardManager->UPDATE_DEBUG();
	blackBoard->SetValueAsBool("IsMoving", false);

	onStopMovingAndNotStun.Broadcast(blackBoard->GetValueAsBool("IsStun"));
}

void UBT_D_BossMovement::SetOwnerCasterPattern(const FPatternMovementData& _patternMovementData)
{
	if (UBTCompositeNode* _node = GetParentNode())
	{
		TArray<FBTCompositeChild>& _children = _node->Children;

		const uint& _max = _children.Num();					
		for (uint i(0); i < _max; ++i)
		{
			FBTCompositeChild& _compositeChild = _children[i];

			TArray<TObjectPtr<UBTDecorator>>& _decorator = _compositeChild.Decorators;
			const uint& _max2 = _decorator.Num();
			for (uint j(0); j < _max2; ++j)
			{
				if (const UBT_D_BossMovement* _checkBossLife = Cast<UBT_D_BossMovement>(_decorator[j]))
				{
					if (GetExecutionIndex() == _checkBossLife->GetExecutionIndex())
					{
						if (UBT_TN_CastPattern* _patternOwner = Cast<UBT_TN_CastPattern>(_compositeChild.ChildTask))
						{
							_patternOwner->ReplacePatternDataAndCastIt(_patternMovementData);
						}
						return;
					}
				}
			}
		}
	}
}

void UBT_D_BossMovement::UnActivate()
{
	// In case we skip the task node, it's better to remove every sub.
#if DEBUG_BB
	LOG("UBT_D_BossMovement::UnActivate");
#endif

	blackBoardManager->UPDATE_DEBUG();
	
	if (blackBoardManager)
	{
		//blackBoardManager->Set_OnReachGoal_DecoratorBossMovement(nullptr);
		blackBoardManager->Set_OnBossStunnedEnd_DecoratorBossMovement(nullptr);
		blackBoardManager->Set_OnBossMidLifeAnimEnd_DecoratorBossMovement(nullptr);
		blackBoardManager->Set_OnBossReset_DecoratorBossMovement(nullptr);

		blackBoardManager->Set_DEBUG_Stun_DecoratorBossMovement(nullptr);
	}
	
	if (bossMovement)
	{
		bossMovement->SetUnactivatePatternRotation();
		bossMovement->SetUnactivatePlayerDetection();
		blackBoardManager->Set_OnDislodgePlayer_DecoratorBossMovement(nullptr);
	}
	
	//blackBoard->SetValueAsBool("IsMoving", false);
	blackBoard->SetValueAsBool("NodeWillWaitForEndOfMovement", false);

#if DEBUG_BB
	LOG(" ");
#endif
}

#if WITH_EDITOR
TArray<FString> UBT_D_BossMovement::GetMovementPointNames() const
{
	if (const UBT_BlackBoard* _blackBoard = Cast<UBT_BlackBoard>(GetTreeAsset()->GetBlackboardAsset()))
	{
		if (const UA_DT_ArenaDataTable* _arenaDataTable = _blackBoard->GetArenaDataTable())
		{
			TArray<FString> _movementPointNames = _arenaDataTable->GetMovementPointNames();

			if (_movementPointNames.Num() < 1)
				LOG("UBT_D_BossMovement::GetMovementPointNames -> There is no Movement Point Name in Arena DataTable");
			
			return _movementPointNames;
		}
		LOG("UBT_D_BossMovement::GetMovementPointNames -> Invalid ArenaDataTable");
		return TArray<FString>();
	}
	LOG("UBT_D_BossMovement::GetMovementPointNames -> Invalid Blackboard");
	return TArray<FString>();
}

bool UBT_D_BossMovement::CanEditChange(const FProperty* InProperty) const
{
	if(!Super::CanEditChange(InProperty))
	{
		return false;
	}

	const FString& _name = InProperty->GetName();
	if (_name == "bIsRandomPoint" || _name == "fRandomRange" || _name == "bIfAlreadyOnPointJustWait")
	{
		return bossMovementMode == EBossMovementMode::MOVE_ON_GROUND
		|| bossMovementMode == EBossMovementMode::MOVE_IN_GROUND
		|| bossMovementMode == EBossMovementMode::DASH_ON_GROUND;
	}
	if (_name == "arenaPointName")
	{
		return bossMovementMode == EBossMovementMode::MOVE_ON_GROUND && !bIsRandomPoint
		|| bossMovementMode == EBossMovementMode::MOVE_IN_GROUND && !bIsRandomPoint
		|| bossMovementMode == EBossMovementMode::DASH_ON_GROUND && !bIsRandomPoint;
	}
	if (_name == "randomArenaPointName")
	{
		return bossMovementMode == EBossMovementMode::MOVE_ON_GROUND && bIsRandomPoint
		|| bossMovementMode == EBossMovementMode::MOVE_IN_GROUND && bIsRandomPoint
		|| bossMovementMode == EBossMovementMode::DASH_ON_GROUND && bIsRandomPoint;
	}
	if (_name == "bActivatePlayerDetection" || _name == "bOverridePlayerDetection")
	{
		return bossMovementMode != EBossMovementMode::MOVE_IN_GROUND_BEHIND_PLAYER
		&& bossMovementMode != EBossMovementMode::MOVE_IN_GROUND_BEHIND_PLAYER_DYNAMIC;
	}
	if (_name == "fPlayerDetectionRange")
	{
		return bActivatePlayerDetection
		&& bOverridePlayerDetection
		&& bossMovementMode != EBossMovementMode::MOVE_IN_GROUND_BEHIND_PLAYER
		&& bossMovementMode != EBossMovementMode::MOVE_IN_GROUND_BEHIND_PLAYER_DYNAMIC;
	}
	if (_name == "bLookPlayer")
	{
		return !bLookShootedPattern;
	}
	if (_name == "bLookShootedPattern")
	{
		return !bLookPlayer;
	}
	
	return true;
}
#endif