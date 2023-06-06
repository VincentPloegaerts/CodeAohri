#pragma once
#include "CoreMinimal.h"

#include "IU_State.h"

#include "IU_FallingState.generated.h"


UCLASS()
class INSIDEUS_API UIU_FallingState : public UIU_State
{
	GENERATED_BODY()
	
	// Struct containing fall properties init from playerMovementComponent
	UPROPERTY(VisibleAnywhere, Category = "State",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FFallData fallData = FFallData();
	//timer handling coyote jump
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FTimerHandle coyoteTimer;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FTimerHandle freeFallTimer;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	FTimerHandle SetCanMoveTimer;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		bool canCoyoteJump = false;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		bool amplifyingAirControl = false;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float groundFriction = 0.0f;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FHitResult groundResult;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float progressAntiStuck = 0.0f;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FVector firstPosition = FVector(0);
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		bool unstuckOnce = false;

	UPROPERTY()
		float fallAnimationTimer = 0;
	UPROPERTY()
		float zFallPosition = -1000000;
	UPROPERTY()
		AIU_Player* player = nullptr;
	UPROPERTY(EditAnywhere)
		float durationOfStopMouvementAfterFalling = 0.20;
UPROPERTY(EditAnywhere)
		float timeBeforePlayerIsConsideredInFall = 0.40;

public:
	FORCEINLINE bool CanCoyoteJump() const { return canCoyoteJump; }
	
public:

	#pragma region FSMBase Methods
	
	// Called once on begin play settings properties
	virtual void Init(UIU_PlayerFSMBrain* _brain) override;
	
	// Called once every frame when state is the current one in the brain
	virtual void Update(const float _deltaTime) override;

	// Override this to check conditions for entering state
	virtual bool CanEnterState() override;

	// Called by playerMovementComponent to pass on its data
	virtual void PassData(const FMovementData& _data) override;

	#pragma endregion

	#pragma region State Methods

	// Called once when entering state
	virtual void Enter() override;
	virtual void Exit() override;

	#pragma endregion

	#pragma region Custom Methods
	
	// Called in update to move player during the fall
	UFUNCTION(BlueprintCallable) void AirControlPlayer(float _deltaTime) const;
	//used to cancel to capacity to jump at the beginning of the fall
	UFUNCTION(BlueprintCallable) void ResetCoyote();
	
	//used to cancel the amplification of the air control after the dash
	UFUNCTION(BlueprintCallable) void ResetAirControl();
	UFUNCTION(BlueprintCallable) void ResetHittingGroundAfterFreeFall();
	UFUNCTION(BlueprintCallable) void SetCanMoveAgain();

	UFUNCTION(BlueprintCallable) void UpdateStuckTime(const float& _deltaTime);
	#pragma endregion
};