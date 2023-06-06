#pragma once

#include "CoreMinimal.h"

#include "ENUM_BossMovementType.h"
#include "ENUM_BossMovementAnimState.h"
#include "STRUCT_PatternMovementData.h"

#include "Components/ActorComponent.h"
#include "B_BossMovement.generated.h"

class UP_PatternCaster;
class AM_GameMode;
class AB_Boss;
class AA_Arena;
class AA_ArenaPoint;
class AIU_Player;
class AM_BlackBoardManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UB_BossMovement : public UActorComponent
{
	GENERATED_BODY()

	friend class AM_BlackBoardManager;
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTick);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachGoal);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTooCloseFromPlayer);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossGoalLocation, const FVector&, _location);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDislodgePlayer, const FPatternMovementData&, _patternMovementData);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayMovementAnimState, const EBossMovementAnimState&, _patternMovementData);

private:
	UPROPERTY()	AIU_Player* player = nullptr;
	UPROPERTY()	AB_Boss* boss = nullptr;
	UPROPERTY()	UP_PatternCaster* patternCaster = nullptr;

	const TMap<FString, AA_ArenaPoint*>* movementPoints = nullptr;

	UPROPERTY()
	TArray<AA_ArenaPoint*> allMovementPoints = TArray<AA_ArenaPoint*>();
	
	UPROPERTY()	FVector vOwnerLocation = FVector::ZeroVector;
	UPROPERTY()	FVector vOwnerStartLocation = FVector::ZeroVector;
	UPROPERTY()	FRotator vOwnerRotation = FRotator::ZeroRotator;
	UPROPERTY()	FRotator vOwnerStartRotation = FRotator::ZeroRotator;
	
	UPROPERTY()
	TEnumAsByte<EBossMovementType> bossMovementType = EBossMovementType::NONE___;
	

	UPROPERTY(VisibleAnywhere, Category = "Boss Movement")
	float fSpeed = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Boss Movement")
	float fWorldSpeed = 1.f;

	UPROPERTY(EditAnywhere, Category = "Boss Movement", meta = (ClampMin = "0"))
	float fRotationSpeed = 1000.0f;

	//Location added to where the raycast is to avoid boss being in ground or floating
	UPROPERTY(EditAnywhere, Category = "Boss Movement")
	float fZLocationOffset = -476.0f;

	UPROPERTY(VisibleAnywhere, Category = "Boss Movement")
	FVector vGoalLocation = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, Category = "Boss Movement")
	FVector vGoalRotation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Boss Movement")
	TArray<TEnumAsByte<EObjectTypeQuery>> arenaLayer;


	//dislodge
	UPROPERTY(EditAnywhere, Category = "Boss Movement| Dislodge Player")
	TArray<FPatternMovementData> patternsToDislodgePlayer;
	
	UPROPERTY(EditAnywhere, Category = "Boss Movement| Dislodge Player", meta = (ClampMin = "0"))
	float fDefaultPlayerDetectionRange = 250.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Boss Movement| Dislodge Player")
	float fCurrentPlayerDetectionRange = 250.0f;
	

	//Distance
	UPROPERTY(EditAnywhere, Category = "Boss Movement| Move Nearest Point", meta = (ClampMin = "0"))
	float fMoveOnSameNearPointAvoidance = 250.0f;

	
	UPROPERTY(EditAnywhere, Category = "Boss Movement| Move Behind Player", meta = (ClampMin = "0"))
	float fDistanceBehindPlayer = 250.0f;
	
	UPROPERTY(EditAnywhere, Category = "Boss Movement| Move Behind Player", meta = (ClampMin = "10"))
	float fDynamicDistanceBehindPlayer = 250.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Boss Movement| Move Behind Player")
	FVector vDynamicOffsetBehindPlayer = FVector::ZeroVector;

	
	UPROPERTY(EditAnywhere, Category = "Boss Movement| Dash Toward Player", meta = (ClampMin = "0"))
	float fDashTowardPlayerMin = 250.0f;
	
	UPROPERTY(EditAnywhere, Category = "Boss Movement| Dash Nearest Point", meta = (ClampMin = "0"))
	float fDashOnSameNearPointAvoidance = 250.0f;
	
	UPROPERTY(EditAnywhere, Category = "Boss Movement| Dash Farest Point", meta = (ClampMin = "0"))
	float fDashOnSameFarPointAvoidance = 250.0f;


	//Wait
	UPROPERTY(EditAnywhere, Category = "Boss Movement| Wait")
	bool bUseRandomWaitTime = false;
	
	UPROPERTY(EditAnywhere, Category = "Boss Movement| Wait", meta = (ClampMin = "0.01", EditCondition = "!bUseRandomWaitTime", EditConditionHides))
	float fWaitTime = 2.0f;
	
	UPROPERTY(EditAnywhere, Category = "Boss Movement| Wait", meta = (ClampMin = "0.01", EditCondition = "bUseRandomWaitTime", EditConditionHides))
	float fMinimumRandWaitTime = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Boss Movement| Wait", meta = (ClampMin = "0.01", EditCondition = "bUseRandomWaitTime", EditConditionHides))
	float fMaximumRandWaitTime = 2.0f;
	

	UPROPERTY()	FTimerHandle waitTimer = FTimerHandle();
	
	FTimerManager* timerManager = nullptr;

	UPROPERTY()	UWorld* world = nullptr;
	UPROPERTY()	AM_GameMode* gameMode = nullptr;
	UPROPERTY()	AM_BlackBoardManager* blackBoardManager = nullptr;
	
	UPROPERTY()	FOnTick onTick = FOnTick();
	UPROPERTY()	FOnDislodgePlayer onDislodgePlayer = FOnDislodgePlayer();

protected:
	UPROPERTY(BlueprintReadWrite, BlueprintCallable, BlueprintAssignable) FOnReachGoal onReachGoal = FOnReachGoal();
	UPROPERTY(BlueprintReadWrite, BlueprintCallable, BlueprintAssignable) FOnBossGoalLocation onBossGoalLocation = FOnBossGoalLocation();
	UPROPERTY(BlueprintReadWrite, BlueprintCallable, BlueprintAssignable) FOnTooCloseFromPlayer onTooCloseFromPlayer = FOnTooCloseFromPlayer();
	UPROPERTY(BlueprintReadWrite, BlueprintCallable, BlueprintAssignable) FOnPlayMovementAnimState onPlayMovementAnimState = FOnPlayMovementAnimState();
	UPROPERTY(BlueprintReadWrite, BlueprintCallable, BlueprintAssignable) FOnPlayMovementAnimState onPlayMovementAnimStateForChildrenBP = FOnPlayMovementAnimState();
	
public:
	FORCEINLINE FOnReachGoal& OnReachGoal() { return onReachGoal; }
	FORCEINLINE FOnBossGoalLocation& OnBossGoalLocation() { return onBossGoalLocation; }
	FORCEINLINE FOnTooCloseFromPlayer& OnTooCloseFromPlayer() { return onTooCloseFromPlayer; }
	FORCEINLINE FOnDislodgePlayer& OnDislodgePlayer() { return onDislodgePlayer; }

	//anim
	FORCEINLINE FOnPlayMovementAnimState& OnPlayMovementAnimState() { return onPlayMovementAnimState; }

public:
	UB_BossMovement();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Init
private:
	//Init movement.
	void Init();
	//Init movement point used in some Movement.
	UFUNCTION() void InitMovementPointsArray();
	//Init player.
	UFUNCTION() void InitPlayerAndManager();
	//Try instant init arena. If he can't will sub to M_BossManager.
	UFUNCTION() void InitToArena();
	//Init arena from M_BossManager.
	UFUNCTION() void PostInitToArena(AA_Arena* _arena);
#pragma endregion Init

#pragma region Waiting
private:
	//Start wait.
	UFUNCTION() void Internal_StartWait();
	//Call onReachGoal when finishing wait.
	UFUNCTION() void Internal_FinishWaitCallGoalReach();
#pragma endregion Waiting

#pragma region Move On Ground
public:
	UFUNCTION() void SetMoveOnGround(const bool& _waitIfOnPoint, const bool& _rotate, const bool& _isRandom,
		const FString& _name, const TArray<FString>& _names, const float& _speed, const float& _randomRange);
	
private:
	//Move on ground and call onReachGoal.
	UFUNCTION() void Internal_MoveOnGroundToGoal();
#pragma endregion Move On Ground

#pragma region Move In Ground
public:
	UFUNCTION() void SetMoveInGround(const bool& _waitIfOnPoint, const bool& _rotate, const bool& _isRandom,
		const FString& _name, const TArray<FString>& _names, const float& _speed, const float& _randomRange);
	UFUNCTION() void SetMoveInGroundBehindPlayer(const bool& _rotate, const float& _speed);
	UFUNCTION() void SetMoveInGroundBehindPlayerDynamic(const bool& _rotate, const float& _speed);

private:
	//Move in ground until reach goal location.
	UFUNCTION() void Internal_MoveInGroundToGoal();
	//Activate move in ground.
	UFUNCTION() void Internal_WhenBossGoInGroundStartMove();
	//When move is finish, call onReachGoal and move anim to NONE.
	UFUNCTION() void Internal_WhenBossGoOutGroundCallGoalReach();

	//Move toward player until close to it.
	UFUNCTION() void Internal_DynamicMoveInGroundTowardPlayer();
	//Move behind the player or if he can't, in front, right or left.
	UFUNCTION() void Internal_DynamicMoveInGroundBehindPlayer();
	//Activate dynamic move.
	UFUNCTION() void Internal_DynamicWhenBossGoInGroundStartMove();
	//When dynamic movement end, call onReachGoal and move anim to NONE.
	UFUNCTION() void Internal_DynamicWhenBossGoOutGroundCallGoalReach();
#pragma endregion Move In Ground

#pragma region Dash
public:
	UFUNCTION() void SetDashInPlayerDirection(const bool& _rotate, const float& _speed);
	UFUNCTION() void SetDashNearestFromBoss(const bool& _rotate, const float& _speed);
	UFUNCTION() void SetDashFarestFromPlayer(const bool& _rotate, const float& _speed);
	UFUNCTION() void SetDashOnGround(const bool& _waitIfOnPoint, const bool& _rotate, const bool& _isRandom,
		const FString& _name, const TArray<FString>& _names, const float& _speed, const float& _randomRange);
	
private:
	//Dash toward goal location.
	UFUNCTION() void Internal_Dash();
	//Activate Dash.
	UFUNCTION() void Internal_WhenBossFinishStartDashAnimStartMove();
	//Call onReachGoal and move animation to NONE.
	UFUNCTION() void Internal_WhenBossFinishEndDashAnimCallGoalReach();
#pragma endregion Dash

#pragma region Rotation
public:
	//Activate the rotation toward target location.
	UFUNCTION() void SetActivatePatternRotation(const bool& _lookPattern, const bool& _lookPlayer);
	UFUNCTION() void SetUnactivatePatternRotation();
	
private:
	//Rotate until reach goal rotation.
	UFUNCTION() void Internal_DefaultRotate();
	//Rotate constantly towards player.
	UFUNCTION() void Internal_PlayerRotate();
	//Set the goal Rotation.
	UFUNCTION() void Internal_ActivateDefaultRotation();
	UFUNCTION() void Internal_SetLookGoalMovement(const FVector& _location);
	UFUNCTION() void Internal_SetLookPlayer();
#pragma endregion Rotation

#pragma region Player Dislodge
public:
	//Activate player detection
	UFUNCTION() void SetActivatePlayerDetection(const bool& _override, const float& _playerDetectionRange);
	//Unactivate player detection used in Decorator to reset event.
	UFUNCTION() void SetUnactivatePlayerDetection();
	
private:
	//Stop boss movement when player too close.
	UFUNCTION() void Internal_WhenPlayerTooCloseStopMove();
	//Call onReachGoal and Shoot pattern.
	UFUNCTION() void Internal_WhenTooCloseFromPlayerCallGoalReach();
	//Check if player is close to boss.
 	UFUNCTION() void Internal_CheckPlayerNear();
	//When player near, shoot a random dislodging pattern.
	UFUNCTION() void Internal_ShootDislodgePattern();
#pragma endregion Player Dislodge

#pragma region Utility
private:
	//Return the correspondant ArenaPoint, can be nullptr.
	UFUNCTION() const AA_ArenaPoint* Internal_GetArenaPoint(const FString& _name) const;
	//Return a random ArenaPoint corresponding to an array name, can be nullptr.
	UFUNCTION() const AA_ArenaPoint* Internal_GetArenaRandomPoint(const TArray<FString>& _names) const;

	//Return an ArenaPoint and say if it's valid with the referenced bool.
	UFUNCTION() const AA_ArenaPoint* Internal_GetValidNotCloseArenaPoint(const bool& _waitIfOnPoint, const FString& _name, bool& _isValid) const;
	//Return an ArenaPoint and say if it's valid with the referenced bool.
	UFUNCTION() const AA_ArenaPoint* Internal_GetValidNotCloseRandomArenaPoint(const bool& _waitIfOnPoint, const TArray<FString>& _names, bool& _isValid) const;
	//Return the nearest ArenaPoint from target and not close to boss.
	UFUNCTION() const AA_ArenaPoint* Internal_GetArenaPointNearestFromTarget(FVector _targetLocation) const;
	//Return the farest ArenaPoint from target and not close to boss.
	UFUNCTION() const AA_ArenaPoint* Internal_GetArenaPointFarestFromTarget(FVector _targetLocation) const;

	//Check if there is a valid location in front, behind, right or left referencing to the direction toward player.
	UFUNCTION() bool Internal_CheckCrossAroundTargetValidMove(const FVector& _targetLocation, FVector _directionBehindTarget, FVector& _inLocation) const;
	//Check if the location he's trying to go is valid.
	UFUNCTION() bool Internal_CheckArenaLayerLocation(const FVector& _tryHitForwardLocation, FVector& _inLocation) const;
	//Check if he can found a valid location around ArenaPoint.
	UFUNCTION() bool Internal_CheckValidMoveLocation(const AA_ArenaPoint* _validArenaPoint, const float& _randomRange);
	//Broadcast an animation depending on the goal direction.
	UFUNCTION() void Internal_SetAnimationDirection(const EBossMovementAnimState& _fwd, const EBossMovementAnimState& _right, const EBossMovementAnimState& _left) const;

	//Remove all event.
	UFUNCTION() void Internal_UnSubAllEvent();
	//Reset boss location and rotation, call UnSubAllEvent.
	UFUNCTION() void Internal_ResetBossLocation();

	UFUNCTION() void Internal_WhenBossIsStunned();

	UFUNCTION() void Internal_ChangeWorldSpeed(const float& _speed);

	UFUNCTION() AM_BlackBoardManager* Internal_GetBlackBoardManager();
#pragma endregion Utility

#pragma region Debug
private:
	UFUNCTION() void DEBUG_ReachGoalBroadcasted();

#pragma endregion Debug
};