#include "B_BossMovement.h"
#include "A_Arena.h"
#include "A_ArenaPoint.h"
#include "IU_Macros.h"
#include "M_ArenaManager.h"
#include "M_GameMode.h"
#include "B_Boss.h"
#include "IU_HealthSystemComponent.h"
#include "IU_Player.h"
#include "M_BlackBoardManager.h"
#include "M_BossManager.h"
#include "M_WorldTimeManager.h"
#include "P_PatternCaster.h"
#include "STRUCT_CustomMath.h"
#include "Kismet/KismetMathLibrary.h"

#define MOVE_AROUND_TARGET_LOG_ERROR(_functionName) \
LOG_ERROR("UB_BossMovement::%s -> Invalid goal location (forward, backward, right, left)", *FString(_functionName)); \
LOG_ERROR("UB_BossMovement::%s -> Is player in the arena ?", *FString(_functionName)); \
LOG_ERROR("UB_BossMovement::%s -> Is the ground collision of arena type ?", *FString(_functionName)); \
LOG_ERROR("UB_BossMovement::%s -> Trying to call Reach Goal but it's very unsafe (can probably stuck boss BT)", *FString(_functionName));

UB_BossMovement::UB_BossMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UB_BossMovement::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void UB_BossMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	onTick.Broadcast();
}

#pragma region Init
void UB_BossMovement::Init()
{
	world = GetWorld();
	timerManager = &world->GetTimerManager();
	
	boss = Cast<AB_Boss>(GetOwner());
	if (!boss)return;
	
	vOwnerLocation = vOwnerStartLocation = boss->GetActorLocation();
	vOwnerRotation = vOwnerStartRotation = boss->GetActorRotation();

	patternCaster = boss->GetPatternCaster();
	ROG_ERROR(!patternCaster, "UB_BossMovement::Init -> invalid pattern caster");
	ROG_ERROR(arenaLayer.Num() < 1, "UB_BossMovement::Init -> Arena Layer is empty (raycast will not work)");
	
	gameMode = world->GetAuthGameMode<AM_GameMode>();
	ROG_ERROR(!gameMode, "UB_BossMovement::Init -> Invalid gamemode");

	blackBoardManager = gameMode->GetBlackBoardManager();
	ROG_ERROR(!blackBoardManager, "UB_BossMovement::Init -> Invalid BlackBoardManager");

	if (AM_WorldTimeManager* _worldTimeManager = gameMode->GetWorldTimeManager())
		Event_AddUnique(_worldTimeManager->OnWorldSpeedChange(), UB_BossMovement::Internal_ChangeWorldSpeed);
	
	Event_Add(boss->OnBossActivated(), UB_BossMovement::InitPlayerAndManager);
	Event_Add(boss->OnBossReset(), UB_BossMovement::Internal_ResetBossLocation);
	Event_AddUnique(boss->OnBossStunned(), UB_BossMovement::Internal_WhenBossIsStunned);
	Event_AddUnique(boss->OnBossMidLifeAnimStart(), UB_BossMovement::Internal_WhenBossIsStunned);//
	
	//OnGoalReach.RegisterDynamic(this, &UB_BossMovement::DEBUG_ReachGoalBroadcasted);
	//Event_Add(onReachGoal, UB_BossMovement::DEBUG_ReachGoalBroadcasted);
	
	if (UIU_HealthSystemComponent* _bossHealth = boss->GetHealthComponent())
		_bossHealth->OnDeath()->AddDynamic(this, &UB_BossMovement::Internal_UnSubAllEvent);
	else
		LOG_ERROR("UB_BossMovement::Init -> Invalid Health Component");

	FTimerHandle _initTimer;
	timerManager->SetTimer(_initTimer, this, &UB_BossMovement::InitToArena, 0.1f);
}

void UB_BossMovement::InitMovementPointsArray()
{
	TArray<FString> _names;	
	const int& _max = movementPoints->GetKeys(_names);

	ROG_ERROR(_max < 1, "UB_BossMovement::InitMovementPointsArray -> The movement point data table has broke");
	
	for (int i = 0; i < _max; ++i)
	{
		AA_ArenaPoint* _movementPoint = movementPoints->FindChecked(_names[i]);
		ROG(!_movementPoint, "UB_BossMovement::InitMovementPointsArray -> Some movement arena point are nullptr");		
		allMovementPoints.Add(_movementPoint);
	}
}

void UB_BossMovement::InitPlayerAndManager()
{
	Event_Remove(boss->OnBossActivated(), UB_BossMovement::InitPlayerAndManager);
	player = gameMode->GetPlayer();
	Internal_GetBlackBoardManager()->Set_OnReachGoal_BossMovement(this);
}

void UB_BossMovement::InitToArena()
{
	if (const AM_GameLogic* _gameLogic = gameMode->GetGameLogic())
	{
		if (const AM_ArenaManager* _arenaManager = _gameLogic->GetArenaManager())
		{
			if (const AA_Arena* _arena = _arenaManager->GetArena())
			{
				movementPoints = _arena->GetMovementPoints();
				InitMovementPointsArray();
			}
		}
	}
	
	if (AM_BossManager* _bossManager = gameMode->GetBossManager())
		Event_AddUnique(_bossManager->OnArenaChanged(), UB_BossMovement::PostInitToArena);
}

void UB_BossMovement::PostInitToArena(AA_Arena* _arena)
{
	ROG_ERROR(!_arena, "UB_BossMovement::PostInitToArena -> Invalid Arena");
	
	allMovementPoints.Empty();
	movementPoints = _arena->GetMovementPoints();
	InitMovementPointsArray();
}
#pragma endregion Init

#pragma region Waiting
void UB_BossMovement::Internal_StartWait()
{
	ROG_ERROR(!timerManager, "UB_BossMovement::Internal_StartWait -> Timer Manager nullptr");
	
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	if (bUseRandomWaitTime)
	{
		const float& _time = FMath::RandRange(fMinimumRandWaitTime, fMaximumRandWaitTime);
		timerManager->SetTimer(waitTimer, this, &UB_BossMovement::Internal_FinishWaitCallGoalReach,_time);
	}
	else
	{
		timerManager->SetTimer(waitTimer, this, &UB_BossMovement::Internal_FinishWaitCallGoalReach,fWaitTime);
	}
}

void UB_BossMovement::Internal_FinishWaitCallGoalReach()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnReachGoal();
	
	//OnGoalReach.Invoke();
	onReachGoal.Broadcast();
}
#pragma endregion Waiting

#pragma region Move On Ground
void UB_BossMovement::SetMoveOnGround(const bool& _waitIfOnPoint, const bool& _rotate, const bool& _isRandom,
	const FString& _name, const TArray<FString>& _names, const float& _speed, const float& _randomRange)
{
	fSpeed = _speed;
	
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();

	bool _isValid = false;
	const AA_ArenaPoint* _arenaPoint = _isRandom ? Internal_GetValidNotCloseRandomArenaPoint(_waitIfOnPoint, _names, _isValid)
	                                             : Internal_GetValidNotCloseArenaPoint(_waitIfOnPoint, _name, _isValid);
	if (!_isValid)
	{
		Internal_StartWait();
		return;
	}
	ROG_ERROR(!_arenaPoint, "UB_BossMovement::SetMoveOnGround -> Invalid Arena Point (unreal probably delete the data table)");

	_isValid = Internal_CheckValidMoveLocation(_arenaPoint, _randomRange);
	ROG_ERROR(!_isValid, "UB_BossMovement::SetMoveOnGround -> There is no valid movable location around %s", *_arenaPoint->GetName());
	
	bossMovementType = EBossMovementType::MOVE_ON_GROUND_;
	Internal_SetAnimationDirection(MOVE_ON_GROUND_FWD, MOVE_ON_GROUND_RIGHT, MOVE_ON_GROUND_LEFT);
	
	if (!onTick.Contains(this, "Internal_MoveOnGroundToGoal"))
		Event_AddUnique(onTick, UB_BossMovement::Internal_MoveOnGroundToGoal);
	
	if (_rotate)
		Internal_ActivateDefaultRotation();
}

void UB_BossMovement::Internal_MoveOnGroundToGoal()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	vOwnerLocation = Get_VLerpConst(vOwnerLocation, vGoalLocation, world->DeltaTimeSeconds, fSpeed * fWorldSpeed);
	boss->SetActorLocation(vOwnerLocation);
	
	if (FVector::Dist(vOwnerLocation, vGoalLocation) < 0.1f)
	{
		Event_Remove(onTick, UB_BossMovement::Internal_MoveOnGroundToGoal);
		onPlayMovementAnimState.Broadcast(EBossMovementAnimState::NONE_);
		onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::NONE_);

		Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnReachGoal();
		//OnGoalReach.Invoke();
		onReachGoal.Broadcast();
	}
}
#pragma endregion Move On Ground

#pragma region Move In Ground
void UB_BossMovement::SetMoveInGround(const bool& _waitIfOnPoint, const bool& _rotate, const bool& _isRandom,
	const FString& _name, const TArray<FString>& _names, const float& _speed, const float& _randomRange)
{
	fSpeed = _speed;
	
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	bool _isValid = false;
	const AA_ArenaPoint* _arenaPoint = _isRandom ? Internal_GetValidNotCloseRandomArenaPoint(_waitIfOnPoint, _names, _isValid)
	                                             : Internal_GetValidNotCloseArenaPoint(_waitIfOnPoint, _name, _isValid);
	if (!_isValid)
	{
		Internal_StartWait();
		return;
	}
	ROG_ERROR(!_arenaPoint, "UB_BossMovement::SetMoveInGround -> Invalid Arena Point (unreal probably delete the data table)");
	
	_isValid = Internal_CheckValidMoveLocation(_arenaPoint, _randomRange);
	ROG_ERROR(!_isValid, "UB_BossMovement::SetMoveInGround -> There is no valid movable location around %s", *_arenaPoint->GetName());
	
	bossMovementType = EBossMovementType::MOVE_IN_GROUND_;
	onPlayMovementAnimState.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_START);
	onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_START);
	
	boss->SetInGround();
	Event_AddUnique(boss->OnInGround(), UB_BossMovement::Internal_WhenBossGoInGroundStartMove);

	if (_rotate)
		Internal_ActivateDefaultRotation();
}

void UB_BossMovement::SetMoveInGroundBehindPlayer(const bool& _rotate, const float& _speed)
{
	fSpeed = _speed;
	
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();

	const FVector& _playerLocation = player->GetActorLocation();

	FVector _direction (_playerLocation - vOwnerLocation);
	_direction.Z = 0;
	_direction.Normalize();

	if (!Internal_CheckCrossAroundTargetValidMove(_playerLocation, _direction * fDistanceBehindPlayer, vGoalLocation))
	{
		MOVE_AROUND_TARGET_LOG_ERROR("SetMoveInGroundBehindPlayer");
		onPlayMovementAnimState.Broadcast(EBossMovementAnimState::NONE_);
		onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::NONE_);
		Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnReachGoal();
		//OnGoalReach.Invoke();
		onReachGoal.Broadcast();
		return;
	}

	bossMovementType = EBossMovementType::MOVE_IN_GROUND_;
	onPlayMovementAnimState.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_START);
	onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_START);
	
	boss->SetInGround();
	Event_AddUnique(boss->OnInGround(), UB_BossMovement::Internal_WhenBossGoInGroundStartMove);
	
	if (_rotate)
		Internal_ActivateDefaultRotation();
}

void UB_BossMovement::SetMoveInGroundBehindPlayerDynamic(const bool& _rotate, const float& _speed)
{
	fSpeed = _speed;
	
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	bossMovementType = EBossMovementType::MOVE_IN_GROUND_;
	onPlayMovementAnimState.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_START);
	onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_START);
	
	boss->SetInGround();
	Event_AddUnique(boss->OnInGround(), UB_BossMovement::Internal_DynamicWhenBossGoInGroundStartMove);
	
	if (_rotate)
		Internal_ActivateDefaultRotation();
}

void UB_BossMovement::Internal_MoveInGroundToGoal()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	vOwnerLocation = Get_VLerpConst(vOwnerLocation, vGoalLocation, world->DeltaTimeSeconds, fSpeed * fWorldSpeed);
	boss->SetActorLocation(vOwnerLocation);
	
	if (FVector::Dist(vOwnerLocation, vGoalLocation) < 0.1f)
	{
		onPlayMovementAnimState.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_END);
		onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_END);
		boss->SetOutGround();
		Event_AddUnique(boss->OnOutGround(), UB_BossMovement::Internal_WhenBossGoOutGroundCallGoalReach);
		Event_Remove(onTick, UB_BossMovement::Internal_MoveInGroundToGoal);
	}
}

void UB_BossMovement::Internal_WhenBossGoInGroundStartMove()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	Event_Remove(boss->OnInGround(), UB_BossMovement::Internal_WhenBossGoInGroundStartMove);
	Event_Add(onTick, UB_BossMovement::Internal_MoveInGroundToGoal);
}

void UB_BossMovement::Internal_WhenBossGoOutGroundCallGoalReach()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	Event_Remove(boss->OnOutGround(), UB_BossMovement::Internal_WhenBossGoOutGroundCallGoalReach);
	onPlayMovementAnimState.Broadcast(EBossMovementAnimState::NONE_);
	onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::NONE_);
	Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnReachGoal();
	//OnGoalReach.Invoke();
	onReachGoal.Broadcast();
}

void UB_BossMovement::Internal_DynamicMoveInGroundTowardPlayer()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	const FVector& _playerLocation = player->GetActorLocation();
	
	vOwnerLocation = Get_VLerpConst(vOwnerLocation, _playerLocation, world->DeltaTimeSeconds, fSpeed * fWorldSpeed);
	boss->SetActorLocation(vOwnerLocation);

	if (FVector::Dist(vOwnerLocation, _playerLocation) < fDynamicDistanceBehindPlayer)
	{
		FVector _direction(_playerLocation - vOwnerLocation);
		_direction.Z = 0;
		_direction.Normalize();
		
		vDynamicOffsetBehindPlayer = _direction * fDynamicDistanceBehindPlayer;

		if(!Internal_CheckCrossAroundTargetValidMove(_playerLocation, vDynamicOffsetBehindPlayer, vGoalLocation))
		{
			MOVE_AROUND_TARGET_LOG_ERROR("Internal_DynamicMoveInGroundTowardPlayer");
			
			boss->SetOutGround();
			Event_AddUnique(boss->OnOutGround(), UB_BossMovement::Internal_DynamicWhenBossGoOutGroundCallGoalReach);
			Event_Remove(onTick, UB_BossMovement::Internal_DynamicMoveInGroundTowardPlayer);
			onPlayMovementAnimState.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_END);
			onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_END);
			Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnReachGoal();
			//OnGoalReach.Invoke();
			onReachGoal.Broadcast();
			return;
		}
		
		Event_AddUnique(onTick, UB_BossMovement::Internal_DynamicMoveInGroundBehindPlayer);
		Event_Remove(onTick, UB_BossMovement::Internal_DynamicMoveInGroundTowardPlayer);
	}
}

void UB_BossMovement::Internal_DynamicMoveInGroundBehindPlayer()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	if(!Internal_CheckCrossAroundTargetValidMove(player->GetActorLocation(), vDynamicOffsetBehindPlayer, vGoalLocation))
	{
		MOVE_AROUND_TARGET_LOG_ERROR("Internal_DynamicMoveInGroundBehindPlayer");
		boss->SetOutGround();
		Event_AddUnique(boss->OnOutGround(), UB_BossMovement::Internal_DynamicWhenBossGoOutGroundCallGoalReach);
		Event_Remove(onTick, UB_BossMovement::Internal_DynamicMoveInGroundBehindPlayer);
		onPlayMovementAnimState.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_END);
		onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_END);
		Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnReachGoal();
		//OnGoalReach.Invoke();
		onReachGoal.Broadcast();
		return;
	}
	
	vOwnerLocation = Get_VLerpConst(vOwnerLocation, vGoalLocation, world->DeltaTimeSeconds, fSpeed * fWorldSpeed);
	boss->SetActorLocation(vOwnerLocation);

	if (FVector::Dist(vOwnerLocation, vGoalLocation) < 0.1f)
	{
		onPlayMovementAnimState.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_END);
		onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::MOVE_IN_GROUND_END);
		boss->SetOutGround();
		Event_AddUnique(boss->OnOutGround(), UB_BossMovement::Internal_DynamicWhenBossGoOutGroundCallGoalReach);
		Event_Remove(onTick, UB_BossMovement::Internal_DynamicMoveInGroundBehindPlayer);
	}
}

void UB_BossMovement::Internal_DynamicWhenBossGoInGroundStartMove()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	Event_Remove(boss->OnInGround(), UB_BossMovement::Internal_DynamicWhenBossGoInGroundStartMove);
	Event_Add(onTick, UB_BossMovement::Internal_DynamicMoveInGroundTowardPlayer);
}

void UB_BossMovement::Internal_DynamicWhenBossGoOutGroundCallGoalReach()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();

	Event_Remove(boss->OnOutGround(), UB_BossMovement::Internal_DynamicWhenBossGoOutGroundCallGoalReach);
	onPlayMovementAnimState.Broadcast(EBossMovementAnimState::NONE_);
	onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::NONE_);
	Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnReachGoal();
	//OnGoalReach.Invoke();
	onReachGoal.Broadcast();
}
#pragma endregion Move In Ground

#pragma region Dash
void UB_BossMovement::SetDashInPlayerDirection(const bool& _rotate, const float& _speed)
{
	fSpeed = _speed;
	
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();

	const FVector& _playerLocation = player->GetActorLocation();
	
	FVector _direction (vOwnerLocation - _playerLocation);
	_direction.Z = 0;
	_direction.Normalize();

	const FVector& _hitLocation = _direction * fDashTowardPlayerMin + _playerLocation;

	if (!Internal_CheckArenaLayerLocation(_hitLocation, vGoalLocation))
	{
		LOG("UB_BossMovement::SetDashInPlayerDirection -> The location between player and boss is not area");
		LOG("UB_BossMovement::SetDashInPlayerDirection -> So we skip the movement (maybe locking the BT)");
		onPlayMovementAnimState.Broadcast(EBossMovementAnimState::NONE_);
		onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::NONE_);
		Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnReachGoal();
		//OnGoalReach.Invoke();
		onReachGoal.Broadcast();
		return;
	}

	bossMovementType = EBossMovementType::DASH;
	Internal_SetAnimationDirection(DASH_FWD_START, DASH_RIGHT_START, DASH_LEFT_START);
	
	boss->SetStartDash();
	Event_AddUnique(boss->OnStartDash(), UB_BossMovement::Internal_WhenBossFinishStartDashAnimStartMove);
	
	if (_rotate)
		Internal_ActivateDefaultRotation();
}

void UB_BossMovement::SetDashNearestFromBoss(const bool& _rotate, const float& _speed)
{
	fSpeed = _speed;
	
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();

	const AA_ArenaPoint* _arenaPoint = Internal_GetArenaPointNearestFromTarget(vOwnerLocation);
	ROG_ERROR(!_arenaPoint, "UB_BossMovement::SetDashNearestFromBoss -> Invalid Arena Point");
	
	if (!Internal_CheckArenaLayerLocation(_arenaPoint->GetActorLocation(), vGoalLocation))
	{
		LOG("UB_BossMovement::SetDashNearestFromBoss -> The location under %s is not Arena", *_arenaPoint->GetName());
		LOG("UB_BossMovement::SetDashNearestFromBoss -> Skipping the movement (maybe locking the BT)");
		onPlayMovementAnimState.Broadcast(EBossMovementAnimState::NONE_);
		onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::NONE_);
		Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnReachGoal();
		//OnGoalReach.Invoke();
		onReachGoal.Broadcast();
		return;
	}

	bossMovementType = EBossMovementType::DASH;
	Internal_SetAnimationDirection(DASH_FWD_START, DASH_RIGHT_START, DASH_LEFT_START);
	
	boss->SetStartDash();
	Event_AddUnique(boss->OnStartDash(), UB_BossMovement::Internal_WhenBossFinishStartDashAnimStartMove);
	
	if (_rotate)
		Internal_ActivateDefaultRotation();
}

void UB_BossMovement::SetDashFarestFromPlayer(const bool& _rotate, const float& _speed)
{
	fSpeed = _speed;
	
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();

	const AA_ArenaPoint* _arenaPoint = Internal_GetArenaPointFarestFromTarget(player->GetActorLocation());
	ROG_ERROR(!_arenaPoint, "UB_BossMovement::SetDashFarestFromPlayer -> Invalid Arena Point");
	
	if (!Internal_CheckArenaLayerLocation(_arenaPoint->GetActorLocation(), vGoalLocation))
	{
		LOG("UB_BossMovement::SetDashFarestFromPlayer -> The location under %s is not Arena", *_arenaPoint->GetName());
		LOG("UB_BossMovement::SetDashFarestFromPlayer -> Skipping the movement (maybe locking the BT)");
		onPlayMovementAnimState.Broadcast(EBossMovementAnimState::NONE_);
		onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::NONE_);
		Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnReachGoal();
		//OnGoalReach.Invoke();
		onReachGoal.Broadcast();
		return;
	}

	bossMovementType = EBossMovementType::DASH;
	Internal_SetAnimationDirection(DASH_FWD_START, DASH_RIGHT_START, DASH_LEFT_START);
	
	boss->SetStartDash();
	Event_AddUnique(boss->OnStartDash(), UB_BossMovement::Internal_WhenBossFinishStartDashAnimStartMove);
	
	if (_rotate)
		Internal_ActivateDefaultRotation();
}

void UB_BossMovement::SetDashOnGround(const bool& _waitIfOnPoint, const bool& _rotate, const bool& _isRandom,
	const FString& _name, const TArray<FString>& _names, const float& _speed, const float& _randomRange)
{
	fSpeed = _speed;
	
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	bool _isValid = false;
	const AA_ArenaPoint* _arenaPoint = _isRandom ? Internal_GetValidNotCloseRandomArenaPoint(_waitIfOnPoint, _names, _isValid)
	                                             : Internal_GetValidNotCloseArenaPoint(_waitIfOnPoint, _name, _isValid);
	if (!_isValid)
	{
		Internal_StartWait();
		return;
	}
	ROG_ERROR(!_arenaPoint, "UB_BossMovement::SetDashOnGround -> Invalid Arena Point (unreal probably delete the data table)");

	_isValid = Internal_CheckValidMoveLocation(_arenaPoint, _randomRange);
	ROG_ERROR(!_isValid, "UB_BossMovement::SetDashOnGround -> There is no valid movable location around %s", *_arenaPoint->GetName());

	bossMovementType = EBossMovementType::DASH;
	Internal_SetAnimationDirection(DASH_FWD_START, DASH_RIGHT_START, DASH_LEFT_START);

#if WITH_LOG_MOVEMENT_BOSS
	LOG("Dash : Start dash");
#endif
	
	boss->SetStartDash();
	Event_AddUnique(boss->OnStartDash(), UB_BossMovement::Internal_WhenBossFinishStartDashAnimStartMove);
	
	if (_rotate)
		Internal_ActivateDefaultRotation();
}

void UB_BossMovement::Internal_Dash()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	vOwnerLocation = Get_VLerpConst(vOwnerLocation, vGoalLocation, world->DeltaTimeSeconds, fSpeed * fWorldSpeed);
	boss->SetActorLocation(vOwnerLocation);
	
	if (FVector::Dist(vOwnerLocation, vGoalLocation) < 0.1f)
	{
		onPlayMovementAnimState.Broadcast(EBossMovementAnimState::DASH_END);
		onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::DASH_END);
		boss->SetEndDash();
		Event_AddUnique(boss->OnEndDash(), UB_BossMovement::Internal_WhenBossFinishEndDashAnimCallGoalReach);
		Event_Remove(onTick, UB_BossMovement::Internal_Dash);
#if WITH_LOG_MOVEMENT_BOSS
		LOG("Dash : End dash");
#endif
	}
}

void UB_BossMovement::Internal_WhenBossFinishStartDashAnimStartMove()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	onPlayMovementAnimState.Broadcast(EBossMovementAnimState::DASH_);
	onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::DASH_);
	Event_Remove(boss->OnStartDash(), UB_BossMovement::Internal_WhenBossFinishStartDashAnimStartMove);
	Event_AddUnique(onTick, UB_BossMovement::Internal_Dash);
#if WITH_LOG_MOVEMENT_BOSS
	LOG("Dash : Dash");
#endif
}

void UB_BossMovement::Internal_WhenBossFinishEndDashAnimCallGoalReach()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	Event_Remove(boss->OnEndDash(), UB_BossMovement::Internal_WhenBossFinishEndDashAnimCallGoalReach);
	onPlayMovementAnimState.Broadcast(EBossMovementAnimState::NONE_);
	onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::NONE_);
	Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnReachGoal();
	//OnGoalReach.Invoke();
	onReachGoal.Broadcast();
#if WITH_LOG_MOVEMENT_BOSS
	LOG("Dash : Reach Goal");
#endif
}
#pragma endregion Dash

#pragma region Rotation
void UB_BossMovement::SetActivatePatternRotation(const bool& _lookPattern, const bool& _lookPlayer)
{
	ROG_ERROR(!patternCaster, "UB_BossMovement::SetActivatePatternRotation -> Invalid pattern caster");
	
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	if (_lookPattern)
		Event_AddUnique(patternCaster->OnTargetLocation(), UB_BossMovement::Internal_SetLookGoalMovement);
	else
		Internal_GetBlackBoardManager()->Set_OnReachGoal_BossMovement_LookPlayer(_lookPlayer);
		//OnGoalReach.RegisterDynamic(this, &UB_BossMovement::Internal_SetLookPlayer);
		//Event_AddUnique(onReachGoal, UB_BossMovement::Internal_SetLookPlayer);
}

void UB_BossMovement::SetUnactivatePatternRotation()
{
	ROG_ERROR(!patternCaster, "UB_BossMovement::SetUnactivatePatternRotation -> Invalid pattern caster");
	
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	Event_Remove(patternCaster->OnTargetLocation(), UB_BossMovement::Internal_SetLookGoalMovement);
	Internal_GetBlackBoardManager()->Set_OnReachGoal_BossMovement_LookPlayer(false);
	//OnGoalReach.UnRegisterDynamic(this, &UB_BossMovement::Internal_SetLookPlayer);
	//Event_Remove(onReachGoal, UB_BossMovement::Internal_SetLookPlayer);
	Event_Remove(onTick, UB_BossMovement::Internal_DefaultRotate);
}

void UB_BossMovement::Internal_DefaultRotate()
{
	FVector _lookDirection (vGoalRotation - vOwnerLocation);
	_lookDirection.Z = 0;

	const FRotator& _lookRotation = Get_LookRotation(_lookDirection);

	vOwnerRotation = Get_RLerpConst(vOwnerRotation, _lookRotation, world->DeltaTimeSeconds, fRotationSpeed * fWorldSpeed);
	boss->SetActorRotation(vOwnerRotation);

	if (FCustomMath::AngleBetweenVectors(_lookDirection, boss->GetActorForwardVector()) < 0.1f)
		Event_Remove(onTick, UB_BossMovement::Internal_DefaultRotate);
}

void UB_BossMovement::Internal_PlayerRotate()
{
	FVector _lookDirection (player->GetActorLocation() - vOwnerLocation);
	_lookDirection.Z = 0;

	const FRotator& _lookRotation = Get_LookRotation(_lookDirection);

	vOwnerRotation = Get_RLerpConst(vOwnerRotation, _lookRotation, world->DeltaTimeSeconds, fRotationSpeed * fWorldSpeed);
	boss->SetActorRotation(vOwnerRotation);
}

void UB_BossMovement::Internal_ActivateDefaultRotation()
{
	Event_Remove(onTick, UB_BossMovement::Internal_PlayerRotate);
	Event_Remove(onTick, UB_BossMovement::Internal_DefaultRotate);
	
	vGoalRotation = vGoalLocation;
	Event_AddUnique(onTick, UB_BossMovement::Internal_DefaultRotate);
}

void UB_BossMovement::Internal_SetLookGoalMovement(const FVector& _location)
{
	Event_Remove(patternCaster->OnTargetLocation(), UB_BossMovement::Internal_SetLookGoalMovement);
	Event_Remove(onTick, UB_BossMovement::Internal_PlayerRotate);
	
	vGoalRotation = _location;
	Event_AddUnique(onTick, UB_BossMovement::Internal_DefaultRotate);
}

void UB_BossMovement::Internal_SetLookPlayer()
{
	Internal_GetBlackBoardManager()->Set_OnReachGoal_BossMovement_LookPlayer(false);
	//OnGoalReach.UnRegisterDynamic(this, &UB_BossMovement::Internal_SetLookPlayer);
	//Event_Remove(onReachGoal, UB_BossMovement::Internal_SetLookPlayer);
	Event_Remove(onTick, UB_BossMovement::Internal_DefaultRotate);
	
	Event_AddUnique(onTick, UB_BossMovement::Internal_PlayerRotate);
}
#pragma endregion Rotation

#pragma region Player Dislodge
void UB_BossMovement::SetActivatePlayerDetection(const bool& _override, const float& _playerDetectionRange)
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	fCurrentPlayerDetectionRange = _override ? _playerDetectionRange : fDefaultPlayerDetectionRange;
	Event_AddUnique(onTick, UB_BossMovement::Internal_CheckPlayerNear);
	Event_AddUnique(onTooCloseFromPlayer, UB_BossMovement::Internal_WhenPlayerTooCloseStopMove);
}

void UB_BossMovement::SetUnactivatePlayerDetection()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	Event_Remove(onTick, UB_BossMovement::Internal_MoveOnGroundToGoal);
	Event_Remove(onTick, UB_BossMovement::Internal_MoveInGroundToGoal);
	Event_Remove(onTick, UB_BossMovement::Internal_Dash);
	Event_Remove(onTick, UB_BossMovement::Internal_DefaultRotate);
	Event_Remove(onTick, UB_BossMovement::Internal_CheckPlayerNear);

	Event_Remove(onTooCloseFromPlayer, UB_BossMovement::Internal_WhenPlayerTooCloseStopMove);
	bossMovementType = NONE___;
}

void UB_BossMovement::Internal_WhenPlayerTooCloseStopMove()
{
	ROG_ERROR(bossMovementType == NONE___, "UB_BossMovement::Internal_WhenPlayerTooCloseStopMove -> The boss movement is NONE but has detection activated");
	
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	if (bossMovementType == EBossMovementType::MOVE_ON_GROUND_)
	{
		Event_Remove(onTick, UB_BossMovement::Internal_MoveOnGroundToGoal);
		Internal_WhenTooCloseFromPlayerCallGoalReach();
	}	
	else if (bossMovementType == EBossMovementType::MOVE_IN_GROUND_)
	{
		Event_Remove(onTick, UB_BossMovement::Internal_MoveInGroundToGoal);
		
		boss->SetOutGround();
		Event_AddUnique(boss->OnOutGround(), UB_BossMovement::Internal_WhenTooCloseFromPlayerCallGoalReach);
	}	
	else if (bossMovementType == EBossMovementType::DASH)
	{
		Event_Remove(onTick, UB_BossMovement::Internal_Dash);
		
		boss->SetEndDash();
		Event_AddUnique(boss->OnEndDash(), UB_BossMovement::Internal_WhenTooCloseFromPlayerCallGoalReach);
	}
	
	Event_Remove(onTick, UB_BossMovement::Internal_DefaultRotate);
	Event_Remove(onTick, UB_BossMovement::Internal_CheckPlayerNear);
	Event_Remove(onTooCloseFromPlayer, UB_BossMovement::Internal_WhenPlayerTooCloseStopMove);
	
	bossMovementType = NONE___;
}

void UB_BossMovement::Internal_WhenTooCloseFromPlayerCallGoalReach()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	Event_Remove(boss->OnEndDash(), UB_BossMovement::Internal_WhenTooCloseFromPlayerCallGoalReach);
	Event_Remove(boss->OnOutGround(), UB_BossMovement::Internal_WhenTooCloseFromPlayerCallGoalReach);
	
	Internal_ShootDislodgePattern();
	
	onPlayMovementAnimState.Broadcast(EBossMovementAnimState::NONE_);
	onPlayMovementAnimStateForChildrenBP.Broadcast(EBossMovementAnimState::NONE_);

	Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnReachGoal();
	//OnGoalReach.Invoke();
	onReachGoal.Broadcast();
}

void UB_BossMovement::Internal_CheckPlayerNear()
{
	if (FVector::Dist(player->GetActorLocation(), vOwnerLocation) < fCurrentPlayerDetectionRange)
	{
		onTooCloseFromPlayer.Broadcast();
	}
}

void UB_BossMovement::Internal_ShootDislodgePattern()
{
	Internal_GetBlackBoardManager()->UPDATE_DEBUG();
	
	const int& _max = patternsToDislodgePlayer.Num();
	ROG(_max < 1, "UB_BossMovement::Internal_DislodgePattern -> Boss movement pattern array to dislodge is empty");
	
	const int& _index = FMath::RandRange(0, _max - 1);
	const FPatternMovementData& _data = patternsToDislodgePlayer[_index];
	
	ROG(!_data.pattern, "UB_BossMovement::Internal_DislodgePattern -> The pattern in dislodge array is nullptr at index %i", _index);
	
	Internal_GetBlackBoardManager()->Broadcast_BossMovement_OnDislodgePlayer(_data);
	onDislodgePlayer.Broadcast(_data);
}
#pragma endregion Player Dislodge

#pragma region Utility
const AA_ArenaPoint* UB_BossMovement::Internal_GetArenaPoint(const FString& _name) const
{
	ROG_NULLPTR(!movementPoints || !movementPoints->Contains(_name), "UB_BossMovement::Internal_GetArenaPoint -> MovementPoints array is probably empty");
	return movementPoints->FindChecked(_name);
}

const AA_ArenaPoint* UB_BossMovement::Internal_GetArenaRandomPoint(const TArray<FString>& _names) const
{
	const uint& _namesNum = _names.Num();
	ROG_ERROR_NULLPTR(_namesNum < 1, "UB_BossMovement::Internal_GetArenaRandomPoint -> You activate Random point selection with no point is the array");
	
	const uint& _index = FMath::Rand() % _namesNum;
	const FString& _name = _names[_index];
	
	ROG_NULLPTR(!movementPoints || !movementPoints->Contains(_name), "UB_BossMovement::Internal_GetArenaRandomPoint -> MovementPoints array is probably empty");
	return movementPoints->FindChecked(_name);
}

const AA_ArenaPoint* UB_BossMovement::Internal_GetValidNotCloseArenaPoint(const bool& _waitIfOnPoint, const FString& _name, bool& _isValid) const
{
	_isValid = false;
	const AA_ArenaPoint* _arenaPoint = Internal_GetArenaPoint(_name);
	
	ROG_ERROR_NULLPTR(!_arenaPoint, "UB_BossMovement::Internal_GetValidNotCloseArenaPoint -> There is an invalid point");

	FVector _arenaLocation(_arenaPoint->GetActorLocation());
	_arenaLocation.Z = 0.0f;
	FVector _ownerLocation(vOwnerLocation);
	_ownerLocation.Z = 0.0f;

	_isValid = !_waitIfOnPoint || fMoveOnSameNearPointAvoidance < FVector::Dist(_arenaLocation, _ownerLocation);
	
	return _arenaPoint;
}

const AA_ArenaPoint* UB_BossMovement::Internal_GetValidNotCloseRandomArenaPoint(const bool& _waitIfOnPoint, const TArray<FString>& _names, bool& _isValid) const
{
	const AA_ArenaPoint* _arenaPoint = nullptr;
	uint _iteration = 0;
	_isValid = false;
	
	FVector _ownerLocation(vOwnerLocation);
	_ownerLocation.Z = 0.0f;
	
	bool _isValidArenaPoint = false;
	while (_isValidArenaPoint == false && _iteration < PASTIS)
	{
		_arenaPoint = Internal_GetArenaRandomPoint(_names);
		ROG_ERROR_NULLPTR(!_arenaPoint, "UB_BossMovement::Internal_GetValidNotCloseRandomArenaPoint -> There is an invalid point");
		
		FVector _arenaLocation(_arenaPoint->GetActorLocation());
		_arenaLocation.Z = 0.0f;		

		_isValidArenaPoint = fMoveOnSameNearPointAvoidance < FVector::Dist(_arenaLocation, _ownerLocation);
		if (_waitIfOnPoint && !_isValidArenaPoint)
			return _arenaPoint;
		
		++_iteration;
	}
	
	ROG_ERROR_NULLPTR(_iteration >= PASTIS, "UB_BossMovement::Internal_GetValidNotCloseRandomArenaPoint -> Boss is alrealdy at this point %s", *_arenaPoint->GetName());
	
	_isValid = true;
	return _arenaPoint;
}

const AA_ArenaPoint* UB_BossMovement::Internal_GetArenaPointNearestFromTarget(FVector _targetLocation) const
{
	const AA_ArenaPoint* _nearestArenaPoint = nullptr;
	float _minimalDistance = BIG_FLOAT;
	_targetLocation.Z = 0.0f;

	const int& _max = allMovementPoints.Num();
	for (int i = 0; i < _max; ++i)
	{
		const AA_ArenaPoint* _currentArenaPoint = allMovementPoints[i];
		ROG_ERROR_NULLPTR(!_currentArenaPoint, "UB_BossMovement::Internal_GetArenaPointNearestFromTarget -> Invalid Arena Point");
		
		FVector _currentArenaPointLocation(_currentArenaPoint->GetActorLocation());
		_currentArenaPointLocation.Z = 0.0f;
		
		const float& _currentDist = FVector::Dist(_currentArenaPointLocation, _targetLocation);
		if (_currentDist < _minimalDistance && _currentDist > fDashOnSameNearPointAvoidance)
		{
			_nearestArenaPoint = _currentArenaPoint;
			_minimalDistance = _currentDist;
		}
	}
	return _nearestArenaPoint;
}

const AA_ArenaPoint* UB_BossMovement::Internal_GetArenaPointFarestFromTarget(FVector _targetLocation) const
{
	const AA_ArenaPoint* _nearestArenaPoint = nullptr;
	float _minimalDistance = 0.0f;
	
	FVector _bossLocation(vOwnerLocation);
	_bossLocation.Z = 0.0f;
	_targetLocation.Z = 0.0f;
	
	const uint& _max = allMovementPoints.Num();
	for (uint i = 0; i < _max; ++i)
	{
		const AA_ArenaPoint* _currentArenaPoint = allMovementPoints[i];
		ROG_ERROR_NULLPTR(!_currentArenaPoint, "UB_BossMovement::Internal_GetArenaPointFarestFromTarget -> Invalid Arena Point");
		
		FVector _currentArenaPointLocation(_currentArenaPoint->GetActorLocation());
		_currentArenaPointLocation.Z = 0.0f;
		
		const float& _currentDist = FVector::Dist(_currentArenaPointLocation, _targetLocation);		
		const float& _distanceBossPoint = FVector::Dist(_currentArenaPointLocation, _bossLocation);		
		if (_currentDist > _minimalDistance && _distanceBossPoint > fDashOnSameNearPointAvoidance)
		{
			_nearestArenaPoint = _currentArenaPoint;
			_minimalDistance = _currentDist;
		}
	}
	
	return _nearestArenaPoint;
}

bool UB_BossMovement::Internal_CheckCrossAroundTargetValidMove(const FVector& _targetLocation, FVector _directionBehindTarget, FVector& _inLocation) const
{
	//forward
	const FVector& _tryHitForwardLocation = _directionBehindTarget + _targetLocation;
	if (!Internal_CheckArenaLayerLocation(_tryHitForwardLocation, _inLocation))
	{
		//backward
		const FVector& _tryHitBackwardLocation = -_directionBehindTarget + _targetLocation;
		if (!Internal_CheckArenaLayerLocation(_tryHitBackwardLocation, _inLocation))
		{
			_directionBehindTarget = _directionBehindTarget.RotateAngleAxis(90.0f, FVector::UpVector);

			//right
			const FVector& _tryHitRightLocation = _directionBehindTarget + _targetLocation;
			if (!Internal_CheckArenaLayerLocation(_tryHitRightLocation, _inLocation))
			{
				//left
				const FVector& _tryHitLeftLocation = -_directionBehindTarget + _targetLocation;
				if (!Internal_CheckArenaLayerLocation(_tryHitLeftLocation, _inLocation))
					return false;
			}
		}
	}
	return true;
}

bool UB_BossMovement::Internal_CheckArenaLayerLocation(const FVector& _tryHitLocation, FVector& _inLocation) const
{
	const FVector& _offset = FVector(0, 0, 2000);

#if WITH_LOG_MOVEMENT_BOSS
	DrawDebugLine(world, _tryHitLocation + _offset, _tryHitLocation - _offset, FColor::Red, false, 10, 0, 5);
#endif
	
	FHitResult _hitResult;
	if (world->LineTraceSingleByObjectType(_hitResult, _tryHitLocation + _offset, _tryHitLocation - _offset, arenaLayer))
	{
		_inLocation = _hitResult.Location;
		_inLocation.Z += fZLocationOffset;
		onBossGoalLocation.Broadcast(_inLocation);
		return true;
	}
	return false;
}

bool UB_BossMovement::Internal_CheckValidMoveLocation(const AA_ArenaPoint* _validArenaPoint, const float& _randomRange)
{
	bool _isValidLocation = false;
	uint _iteration = 0;
	const FVector& _arenaPointLocation = _validArenaPoint->GetActorLocation();

	while (_isValidLocation == false && _iteration < PASTIS)
	{
		const float& _range = FMath::RandRange(0.0f, _randomRange);
		const float& _randomRad = To_Rad(FMath::RandRange(0.0f, 359.9f));
	
		const float& _x = FMath::Cos(_randomRad) * _range;
		const float& _y = FMath::Sin(_randomRad) * _range;
	
		const FVector& _hitLocation = _arenaPointLocation + FVector(_x, _y,0.0f);

		_isValidLocation = Internal_CheckArenaLayerLocation(_hitLocation, vGoalLocation);
		++_iteration;
	}

	return _iteration < PASTIS;
}

void UB_BossMovement::Internal_SetAnimationDirection(const EBossMovementAnimState& _fwd, const EBossMovementAnimState& _right, const EBossMovementAnimState& _left) const
{
	FVector _lookDirection (vGoalLocation - vOwnerLocation);
	_lookDirection.Z = 0;

	if (To_Deg(FCustomMath::AngleBetweenVectors(_lookDirection, boss->GetActorForwardVector())) < 45.0f)
	{
		onPlayMovementAnimState.Broadcast(_fwd);
		onPlayMovementAnimStateForChildrenBP.Broadcast(_fwd);
		return;
	}
	
	const float& _angleBetweenRight = To_Deg(FCustomMath::AngleBetweenVectors(_lookDirection, boss->GetActorRightVector()));
	if (_angleBetweenRight < 45.0f)
	{
		onPlayMovementAnimState.Broadcast(_right);
		onPlayMovementAnimStateForChildrenBP.Broadcast(_right);
	}
	else if (_angleBetweenRight > 135.0f)
	{
		onPlayMovementAnimState.Broadcast(_left);
		onPlayMovementAnimStateForChildrenBP.Broadcast(_left);
	}
	else //Goal location Behind
	{
		onPlayMovementAnimState.Broadcast(_fwd);
		onPlayMovementAnimStateForChildrenBP.Broadcast(_fwd);
	}
}

void UB_BossMovement::Internal_UnSubAllEvent()
{
	Event_Remove(boss->OnBossActivated(), UB_BossMovement::InitPlayerAndManager);

	AB_Boss::FOnInGround& _inGround = boss->OnInGround();
	Event_Remove(_inGround, UB_BossMovement::Internal_WhenBossGoInGroundStartMove);
	Event_Remove(_inGround, UB_BossMovement::Internal_DynamicWhenBossGoInGroundStartMove);
	
	AB_Boss::FOnOutGround& _outGround = boss->OnOutGround();
	Event_Remove(_outGround, UB_BossMovement::Internal_WhenBossGoOutGroundCallGoalReach);
	Event_Remove(_outGround, UB_BossMovement::Internal_DynamicWhenBossGoOutGroundCallGoalReach);
	
	Event_Remove(boss->OnStartDash(), UB_BossMovement::Internal_WhenBossFinishStartDashAnimStartMove);
	Event_Remove(boss->OnEndDash(), UB_BossMovement::Internal_WhenBossFinishEndDashAnimCallGoalReach);
	
	Event_Remove(onTick, UB_BossMovement::Internal_MoveOnGroundToGoal);
	Event_Remove(onTick, UB_BossMovement::Internal_MoveInGroundToGoal);
	Event_Remove(onTick, UB_BossMovement::Internal_Dash);
	Event_Remove(onTick, UB_BossMovement::Internal_DefaultRotate);
	Event_Remove(onTick, UB_BossMovement::Internal_PlayerRotate);
	Event_Remove(onTick, UB_BossMovement::Internal_CheckPlayerNear);
	Event_Remove(onTick, UB_BossMovement::Internal_DynamicMoveInGroundTowardPlayer);
	Event_Remove(onTick, UB_BossMovement::Internal_DynamicMoveInGroundBehindPlayer);
	
	Event_Remove(onTooCloseFromPlayer, UB_BossMovement::Internal_WhenPlayerTooCloseStopMove);
	
	Event_Remove(patternCaster->OnTargetLocation(), UB_BossMovement::Internal_SetLookGoalMovement);

	//OnGoalReach.UnRegisterDynamic(this, &UB_BossMovement::Internal_SetLookPlayer);
	//Event_Remove(onReachGoal, UB_BossMovement::Internal_SetLookPlayer);
	Internal_GetBlackBoardManager()->Set_OnReachGoal_BossMovement_LookPlayer(false);
	timerManager->ClearTimer(waitTimer);
}

void UB_BossMovement::Internal_ResetBossLocation()
{
	boss->SetActorLocation(vOwnerStartLocation);
	vOwnerLocation = vOwnerStartLocation;
	boss->SetActorRotation(vOwnerStartRotation);
	vOwnerRotation = vOwnerStartRotation;
	Internal_UnSubAllEvent();
}

void UB_BossMovement::Internal_WhenBossIsStunned()
{
#if DEBUG_BB
	LOG("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
#endif
	
	AB_Boss::FOnInGround& _inGround = boss->OnInGround();
	Event_Remove(_inGround, UB_BossMovement::Internal_WhenBossGoInGroundStartMove);
	Event_Remove(_inGround, UB_BossMovement::Internal_DynamicWhenBossGoInGroundStartMove);
	
	AB_Boss::FOnOutGround& _outGround = boss->OnOutGround();
	Event_Remove(_outGround, UB_BossMovement::Internal_WhenBossGoOutGroundCallGoalReach);
	Event_Remove(_outGround, UB_BossMovement::Internal_DynamicWhenBossGoOutGroundCallGoalReach);
	
	Event_Remove(boss->OnStartDash(), UB_BossMovement::Internal_WhenBossFinishStartDashAnimStartMove);
	Event_Remove(boss->OnEndDash(), UB_BossMovement::Internal_WhenBossFinishEndDashAnimCallGoalReach);
	
	Event_Remove(onTick, UB_BossMovement::Internal_MoveOnGroundToGoal);
	Event_Remove(onTick, UB_BossMovement::Internal_MoveInGroundToGoal);
	Event_Remove(onTick, UB_BossMovement::Internal_Dash);
	Event_Remove(onTick, UB_BossMovement::Internal_DefaultRotate);
	Event_Remove(onTick, UB_BossMovement::Internal_PlayerRotate);
	Event_Remove(onTick, UB_BossMovement::Internal_CheckPlayerNear);
	Event_Remove(onTick, UB_BossMovement::Internal_DynamicMoveInGroundTowardPlayer);
	Event_Remove(onTick, UB_BossMovement::Internal_DynamicMoveInGroundBehindPlayer);
	
	Event_Remove(onTooCloseFromPlayer, UB_BossMovement::Internal_WhenPlayerTooCloseStopMove);
	
	Event_Remove(patternCaster->OnTargetLocation(), UB_BossMovement::Internal_SetLookGoalMovement);
	
	Internal_GetBlackBoardManager()->Set_OnReachGoal_BossMovement_LookPlayer(false);
	timerManager->ClearTimer(waitTimer);
}

void UB_BossMovement::Internal_ChangeWorldSpeed(const float& _speed)
{
	fWorldSpeed = _speed;
}

AM_BlackBoardManager* UB_BossMovement::Internal_GetBlackBoardManager()
{
	if (!blackBoardManager)
	{
		ROG_NULLPTR(!gameMode, "UB_BossMovement::Internal_GetBlackBoardManager -> Invalid Gamemode");
		blackBoardManager = gameMode->GetBlackBoardManager();
		return blackBoardManager;
	}
	return blackBoardManager;
}
#pragma endregion Utility

#pragma region Debug
void UB_BossMovement::DEBUG_ReachGoalBroadcasted()
{
	LOG_ERROR("DEBUG Goal Reached");
}
#pragma endregion Debug