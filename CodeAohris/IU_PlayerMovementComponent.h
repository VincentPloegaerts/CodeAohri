#pragma once
#include "CoreMinimal.h"

#include "IU_Camera.h"

#include "GameFramework/PawnMovementComponent.h"
#include "IU_PlayerMovementComponent.generated.h"

class UIU_PlayerFSMBrain;

// Struct to pass data on to MovingOnGroundState
USTRUCT(BlueprintType)
struct FMoveOnGroundData 
{
	GENERATED_BODY()
	
	// Default move speed used for ground movements
	UPROPERTY(EditAnywhere, Category = "Movement | Values", meta = (UIMin = "0.0", UIMax = "10000.0", ClampMin = "0.0", ClampMax = "10000.0"),BlueprintReadWrite)
		float moveSpeed = 100.0f;
	
	// Range on -Z to detect ground through collisions should be > 0
	UPROPERTY(EditAnywhere,Category = "Movement | Values",BlueprintReadWrite)
		float groundHeightDetection = 10.0f;

	// Max slope angle the player can move on without sliding off
	UPROPERTY(EditAnywhere,Category = "Movement | Values",BlueprintReadWrite)
		float acceptableSlopeAngle = 40.0f;

	// Height added to capsule root to detect collisions
	// => Too high might make it snap on platforms
	// => Too low won't let the player move up a bit when there's a height difference on the ground
	UPROPERTY(EditAnywhere,Category = "Movement | Values",BlueprintReadWrite)
		float stepMaxHeight = 10.0;

	// How fast will the player stop when releasing inputs/landing
	// 1 will not brake at all (so he'll slide forever)
	UPROPERTY(EditAnywhere,Category = "Movement | Values", meta = (ClampMin = "1.0", ClampMax = "10000.0"),BlueprintReadWrite)
		float groundFriction =10.0f;
	
	FMoveOnGroundData() {}
};

// Struct to pass data on to DashState
USTRUCT(BlueprintType)
struct FDashData
{
	GENERATED_BODY()
	
	// Input Name linking into UE input manager 
	UPROPERTY(EditAnywhere,Category = "Dash | Inputs",BlueprintReadWrite)
	FName dashInputName = "Dash";
	
	// Determine How Far the dash will go
	UPROPERTY(EditAnywhere,Category = "Dash | Values",BlueprintReadWrite)
	float dashPower = 100.0f;
	
	// Duration in seconds
	UPROPERTY(EditAnywhere,Category = "Dash | Values",BlueprintReadWrite)
	float dashDuration = 1.0f;
	
	// DashCooldown when used on ground
	// => Starts when pressing the key (= not at the end of the dash)
	UPROPERTY(EditAnywhere,Category = "Dash | Values",BlueprintReadWrite)
	float dashCooldown = 2.0f;
	
	FDashData() {}
};

// Struct to pass data on to JumpingState
USTRUCT(BlueprintType)
struct FJumpData
{
	GENERATED_BODY()
	
	// Input Name linking into UE input manager 
	UPROPERTY(EditAnywhere,Category = "Jump | Inputs",BlueprintReadWrite)
		FName jumpInputName = "Jump";
	
	// Determine how far the player will go based on inputs
	// UPROPERTY(EditAnywhere,Category = "Jump | Values")
	// 	float jumpHorizontalPower = 100.0f;
	
	// Determine how high the player will go
	UPROPERTY(EditAnywhere,Category = "Jump | Values",BlueprintReadWrite)
		float jumpVerticalPower = 100.0f;

	//Time before the gravity multiplier apply
	UPROPERTY(EditAnywhere,Category = "State | Values",BlueprintReadWrite)
		float ascentDuration = 0.2f;
	FJumpData() {}
};

// Struct to pass data on to FallState
USTRUCT(BlueprintType)
struct FFallData
{
	GENERATED_BODY()
	
	// Gravity scale multiplying downward Movement
	UPROPERTY(EditAnywhere, Category = "Movement | Fall", meta = (ClampMin = "0.0", ClampMax = "10000.0"),BlueprintReadWrite)
		float gravityScale = 100.0f;
	
	// Air control influence
	UPROPERTY(EditAnywhere, Category = "Movement | Fall", meta = (ClampMin = "0.0", ClampMax = "10000.0"),BlueprintReadWrite)
		float airControl = 100.0f;
	//multiply gravity affecting capsule during fall state (so not while ascending during jump)
	UPROPERTY(EditAnywhere, Category = "Movement | Fall", meta = (ClampMin = "0.0", ClampMax = "10.0"),BlueprintReadWrite)
		float gravityMultiplier = 1.0f;
	//duration while jump is enable while falling
	UPROPERTY(EditAnywhere, Category = "Movement | Fall", meta = (ClampMin = "0.0", ClampMax = "10.0"),BlueprintReadWrite)
		float coyoteTime = 0.1f;
	//factor on air control after the dash
	UPROPERTY(EditAnywhere, Category = "Movement | Fall", meta = (ClampMin = "0.0", ClampMax = "100.0"),BlueprintReadWrite)
		float airControlAmplifier = 3.0f;
	//duration of the air control amplification after the dash
	UPROPERTY(EditAnywhere, Category = "Movement | Fall", meta = (ClampMin = "0.0", ClampMax = "10.0"),BlueprintReadWrite)
		float amplificationTime = 1.0f;
	//Time before the anti stuck bumps the player
	UPROPERTY(EditAnywhere, Category = "Movement | Fall", meta = (ClampMin = "0.0", ClampMax = "10.0"),BlueprintReadWrite)
		float antiStuckTime = 10.0f;
	
	FFallData() {}
};

// Containing all movements structs that goes into states
// => Giga giga struct giga giga struct ohé ohé
USTRUCT(BlueprintType)
struct FMovementData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Datas",BlueprintReadWrite)
	FMoveOnGroundData movingOnGroundData = FMoveOnGroundData();

	UPROPERTY(EditAnywhere, Category = "Datas",BlueprintReadWrite)
	FDashData dashData = FDashData();
	
	UPROPERTY(EditAnywhere, Category = "Datas",BlueprintReadWrite)
	FJumpData jumpData = FJumpData();
	
	UPROPERTY(EditAnywhere, Category = "Datas",BlueprintReadWrite)
	FFallData fallData = FFallData();
	
	FMovementData() {}
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitGround);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitGroundWithResult,FHitResult,_result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDashEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJumpEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeaveGroundEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UIU_PlayerMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdate, const float&, _deltaTime);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHorizontalValueChange, float, _axis);
	FOnHorizontalValueChange onHorizontalValueChange;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVerticalValueChange, float, _axis);
	FOnVerticalValueChange onVerticalValueChange;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJumpingEvent, bool, _isJumping);
	FOnJumpingEvent onJumpEvent;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDashingEvent, bool, _isDashing);
	FOnDashingEvent onDashEvent;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFallingEvent, bool, _isFalling);
	FOnFallingEvent onFallEvent;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHittingGroundAfterFreeFall, bool, _isHittingGroundAfterFreeFall);
	FOnHittingGroundAfterFreeFall onHittingGroundAfterFreeFall;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStopWalking,bool, _canDoStopAnimation);
	UPROPERTY(VisibleAnywhere,BlueprintAssignable,BlueprintCallable,meta = (DisplayName = "FOnPlayerStopWalking"))
	FOnPlayerStopWalking onPlayerStopWalking;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerTwist);
	UPROPERTY(VisibleAnywhere,BlueprintAssignable,BlueprintCallable,meta = (DisplayName = "FOnPlayerTwist"))
	FOnPlayerTwist onPlayerTwist;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSliding);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnSliding"))
		FOnSliding onSliding;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlidingEnd);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnSlidingEnd"))
		FOnSlidingEnd onSlidingEnd;

	#pragma region Events
protected:
	
	// Event called when hitting ground during state
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite)
		FOnHitGround onHitGround = FOnHitGround();
	// Event called when hitting ground during state
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite)
		FOnHitGroundWithResult onHitGroundWithResult = FOnHitGroundWithResult();
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite)
		FOnDashEvent onDashed = FOnDashEvent();
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite)
		FOnDashEvent onDashEnded = FOnDashEvent();
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite)
		FOnJumpEvent onJump = FOnJumpEvent();
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite)
		FOnLeaveGroundEvent onLeaveGround = FOnLeaveGroundEvent();
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite)
		FOnUpdate onUpdate;
	
public:
	FORCEINLINE FOnHitGround& OnHitGround() { return onHitGround; }
	FORCEINLINE FOnHitGroundWithResult& OnHitGroundWithResult() { return onHitGroundWithResult; }
	FORCEINLINE FOnDashEvent& OnDashed() { return onDashed; }
	FORCEINLINE FOnDashEvent& OnDashEnded() { return onDashEnded; }
	FORCEINLINE FOnJumpEvent& OnJump() { return onJump; }
	FORCEINLINE FOnLeaveGroundEvent& OnLeaveGround() { return onLeaveGround; }
	
	FORCEINLINE FOnHorizontalValueChange& OnHorizontalValueChange() { return onHorizontalValueChange; }
	FORCEINLINE FOnVerticalValueChange& OnVerticalValueChange() { return onVerticalValueChange; }
	FORCEINLINE FOnJumpingEvent& OnJumpEvent() { return onJumpEvent; }
	FORCEINLINE FOnDashingEvent& OnDashEvent() { return onDashEvent; }
	FORCEINLINE FOnFallingEvent& OnFallingEvent() { return onFallEvent; }
	FORCEINLINE FOnHittingGroundAfterFreeFall& OnHittingGroundAfterFreeFall() { return onHittingGroundAfterFreeFall; }
	
	FORCEINLINE FOnPlayerStopWalking& OnPlayerStopWalking() { return onPlayerStopWalking; }
	FORCEINLINE FOnPlayerTwist& OnPlayerTwist() { return onPlayerTwist; }
	FORCEINLINE FOnSliding& OnSliding() { return onSliding; }
	FORCEINLINE FOnSlidingEnd& OnSlidingEnd() { return onSlidingEnd; }
	
	#pragma endregion
	
private:
	// Dedicated player camera
	UPROPERTY(EditAnywhere, Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		AIU_Camera* playerCamera = nullptr;
	
	// Player capsule reference stocked to avoid using getters all the time
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UCapsuleComponent* ownerCapsule = nullptr;
	
	// Default movement status
	UPROPERTY(EditAnywhere, Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		bool canMove = true;
	
	// Last direction from inputs kept until next frame to use in components if needed unlike framedirection
	UPROPERTY(VisibleAnywhere, Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FVector lastDirection = FVector(0.0f);
	
	// Temporary inputs that'll move into lastDirection and reset to 0
	UPROPERTY(VisibleAnywhere, Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FVector frameDirection = FVector(0.0f);
	
	// Temporary inputs that'll move into lastDirection and reset to 0
	UPROPERTY(VisibleAnywhere, Category = "Values")
		FVector2f frameInputs = FVector2f(0.0f);

	// Struct containing all properties for movement 
	UPROPERTY(EditAnywhere, Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FMovementData movementStats = FMovementData();
	
	//Max speed the player can attain 
	UPROPERTY(EditAnywhere,Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float maxVelocity = 10.0f;
	
	//How much input is needed to considered it not being drift
	UPROPERTY(EditAnywhere,Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta = (ClampMin = "0.0",ClampMax = "1.0"))
		float axisAcceptance = 0.1f;
	
	//Defines up to when is the input considered being "slow" and by so when it stops being constant on 'axisSlowValue'
	UPROPERTY(EditAnywhere,Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta = (ClampMin = "0.0",ClampMax = "1.0"))
		float axisMaxSlowInput = 0.4f;
	
	//How much input is taken into account when the real input is less than 'aaxisMaxSlowInput'
	UPROPERTY(EditAnywhere,Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta = (ClampMin = "0.0",ClampMax = "1.0"))
		float axisSlowValue = 0.1f;
	
	//Defines at which point is the input considered being "fast" and by so when it starts being constant on '1'
	UPROPERTY(EditAnywhere,Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta = (ClampMin = "0.0",ClampMax = "1.0"))
		float axisMinFastInput = 0.95f;
	
	//Speed at which the player will orientate herself towards the left stick input 
	UPROPERTY(EditAnywhere,Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float playerRotateSpeed = 2.0f;
	
	//Force dragging the player in the air and when stopping (inertia)
	UPROPERTY(VisibleAnywhere,Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FVector momentum = FVector(-1, 0, 0);

	// How much speed the player keep going backward
	UPROPERTY(EditAnywhere,Category = "Values",BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"),meta = (AllowPrivateAccess = True))
		float backwardSpeedSlowPercent = 70.0f;

	// How long does the player keep the invicibility state after pressing dash
	UPROPERTY(EditAnywhere,Category = "Values",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float dashImmunityDuration = 0.5f;

	// Useful for tuto, to know if the player dashed through the projectile wall
	UPROPERTY(VisibleAnywhere, Category = "State",BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		bool bHasDashed = false;
	
	UPROPERTY()
		FTimerHandle handleDashInvincibility;
	
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UIU_PlayerFSMBrain* brain = nullptr;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		bool canSwitchState = true;

	UPROPERTY()
		FQuat boxRotation = FQuat(FVector(0.f, 0.f, -1.f), PI * 0.25f);
	
	UPROPERTY()
		UWorld* world = nullptr;
	
	UPROPERTY()
		UC_PlayerAttackComponent* playerAttackComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere,Category = "Values", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float fSlowFactor = 1.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Animation",meta = (AllowPrivateAccess = True))
	float timeBeforeStopAnimationPlay = 0.15;

	//ANIMATION PROPERTIES
	UPROPERTY() bool canRotateAnim = true;
	UPROPERTY() bool hasAlreadyStopped = false;
	UPROPERTY() bool hasAlreadyMovedWhenStop = false;
	UPROPERTY() float previousZAngle = 0.0f;
	UPROPERTY() float fNoInputForAnimationTimer = 0.0f;
	UPROPERTY() float moveWhenStoppedTimer = 0.0f;
	UPROPERTY() FVector currentAnimDirection = FVector::ZeroVector;
	UPROPERTY() FVector previousAnimDirection = FVector::ZeroVector;
	UPROPERTY() FTimerHandle animDirectionTimer;
	UPROPERTY() float currentRuningTime = 0;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Animation",meta = (AllowPrivateAccess = True)) float runningTimeNeededToDeclenchStopAnimation = 2;
	UPROPERTY() bool isSliding = false;

#pragma region KnockBack
private:
	UPROPERTY()
	bool bIsKnockBack = false;
	UPROPERTY()
	FTimerHandle knockBackTimer = FTimerHandle();
#pragma endregion KnockBack
	
public:
	FORCEINLINE void SetPlayerCamera(AIU_Camera* _playerCamera) { playerCamera = _playerCamera; }

#pragma region Getters
	
	FORCEINLINE bool CanMove() const{ return canMove; }
	FORCEINLINE bool CanSwitchState() const{ return canSwitchState; }
	FORCEINLINE FVector OwnerLocation() const { return ownerPawn->GetActorLocation(); }
	FORCEINLINE FRotator OwnerRotation() const { return ownerPawn->GetActorRotation(); }
	FORCEINLINE AIU_Camera* GetPlayerCamera() const { return playerCamera; }
	FORCEINLINE FVector GetCameraForwardVector() const { return playerCamera ? playerCamera->GetActorForwardVector() : FVector(0.0f); }
	FORCEINLINE FVector GetCameraRightVector() const { return playerCamera ? playerCamera->GetActorRightVector() : FVector(0.0f); }
	FORCEINLINE float RotateSpeed() const { return playerRotateSpeed; }
	FORCEINLINE float MaxVelocity() const { return maxVelocity; }
	FORCEINLINE float GetSlowFactor() const { return fSlowFactor; }
	FORCEINLINE FVector LastInputsDirection() const { return lastDirection; }
	FORCEINLINE FVector2f FrameInputs() const { return frameInputs; }
	FORCEINLINE FVector Momentum() const { return momentum; }
	FORCEINLINE FMovementData& MovementStats() { return movementStats; }
	FORCEINLINE bool GetHasDashed() const { return bHasDashed; }
	FORCEINLINE bool GetIsSliding() const { return isSliding; }

#pragma endregion

#pragma region Setters
	
FORCEINLINE void SetCanMove(bool _status) {canMove = _status;}
FORCEINLINE void SetCanSwitchState(bool _status) {canSwitchState = _status;}
FORCEINLINE void SetSlowFactor(float _value) {fSlowFactor = _value;}
FORCEINLINE void SetHasDashed(bool _status) { bHasDashed = _status; }
FORCEINLINE void SetIsSliding(bool _status) { isSliding = _status; }

#pragma endregion
	
public:
	UIU_PlayerMovementComponent();	

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


#pragma region AFK_Movement
private:
	UPROPERTY(VisibleAnywhere, Category = "AFK")
	bool bIsMovingWithoutJumping = false;
	
	UPROPERTY(VisibleAnywhere, Category = "AFK")
	bool bIsJumping = false;
	
	UPROPERTY(VisibleAnywhere, Category = "AFK")
	bool bIsInArena = false;

	UPROPERTY(EditAnywhere, Category = "AFK", meta = (ClampMin = "0.1"))
	float fCheckRate = 0.8f;

	UPROPERTY(VisibleAnywhere, Category = "AFK")
	float fAFKTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "AFK", meta = (ClampMin = "1"))
	float fMinimalMoveDistance = 100.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "AFK")
	FVector vLastLocationWithoutJumping = FVector::ZeroVector;
	
	UPROPERTY()
	AIU_Player* ownerPawn = nullptr;
	
	UPROPERTY()
	FTimerHandle playerMoveTimer;

public:
	FORCEINLINE const bool& GetIsMovingWithoutJumping() const { return bIsMovingWithoutJumping; }
	FORCEINLINE const float& GetAFKTime() const { return fAFKTime; }
	
	UFUNCTION() void SetIsInArena(const bool _state);
	
private:
	UFUNCTION() void CheckPlayerHasMoveWithoutJumping();
	UFUNCTION() void ResetPlayerAFKTime();
	UFUNCTION() void ResetPlayerTakeDamage() const;
	UFUNCTION() void CallResetTakeDamageWithDelay();
	UFUNCTION() void IncreasePlayerAFKTime(const float& _deltaTime);
	UFUNCTION() void SetIsJumpingTrue();
	UFUNCTION() void SetIsJumpingFalse();	
#pragma endregion AFK_Movement
	
public:

	#pragma region Movement Methods
	
	// Called from inputs
	UFUNCTION(BlueprintCallable) void MoveForward(float _axis);
	
	// Called from inputs
	UFUNCTION(BlueprintCallable) void MoveRight(float _axis);

	// Move player while checking collisions and sliding off them 
	UFUNCTION(BlueprintCallable) void MovePlayer(FVector _direction);
	
	// Rotate player toward vector
	UFUNCTION(BlueprintCallable)void RotateToDirection(const FVector& _direction, const float _rotateSpeed) const;


	UFUNCTION(BlueprintCallable) void SetMomentum(const FVector& _momentum, bool _useLimit = true);
	UFUNCTION(BlueprintCallable) void IncreaseMomentum(const FVector& _momentum, bool _useLimit = true);

	void MovePlayerWithMomentum();
	#pragma endregion 

	#pragma region Ground Methods
	
	// Collision and raycast to see if player is on the ground
	UFUNCTION(BlueprintCallable) bool CheckIsGrounded(FHitResult& _outHit) const;

	// Check if the surface from FHitResult is not too steep
	UFUNCTION(BlueprintCallable) bool CheckAcceptableSlope(const FHitResult& _result) const;

	#pragma endregion

	#pragma region Animation Methods

	// Useful to know if the player need to do the stop or the twist animation
	UFUNCTION(BlueprintCallable) void CheckAnimToPlay();

	UFUNCTION(BlueprintCallable) void UpdateAnimDirectionVectors();
	UFUNCTION(BlueprintCallable) void SetCanAnimRotationToTrue();
	UFUNCTION(BlueprintCallable) void CheckForStopAnimation();
	UFUNCTION(BlueprintCallable) void ResetStopAnimation();

	#pragma endregion

#pragma region KnockBack
public:
	UFUNCTION() void StartKnockBack(const FVector& _knockDirection, const float& _duration);
	UFUNCTION() void StopKnockBack();
#pragma endregion KnockBack
};