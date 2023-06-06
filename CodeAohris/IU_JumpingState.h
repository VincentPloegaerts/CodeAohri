#pragma once
#include "CoreMinimal.h"
#include "IU_State.h"
#include "IU_JumpingState.generated.h"

UCLASS()
class INSIDEUS_API UIU_JumpingState : public UIU_State
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "State", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bCanJump = true;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fAirControl = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fGravityScale = 0.0f;
	
	// Debug to see what was the calculated destination
	UPROPERTY(VisibleAnywhere, Category = "State", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FVector jumpDestination = FVector::ZeroVector;
	
	// Struct containing jump properties init from playerMovementComponent
	UPROPERTY(VisibleAnywhere, Category = "State", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FJumpData jumpData = FJumpData();
	
	UPROPERTY(VisibleAnywhere, Category = "State", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FTimerHandle handle = FTimerHandle();
	
public:
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetCanJumpStatus(bool _status) { bCanJump = _status; }

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

public:
	// Called when pressing inputs possibly changing current state if conditions are met
	UFUNCTION(BlueprintCallable) void Jump();
	
private:
	UFUNCTION(BlueprintCallable) void JumpFinished();
	#pragma endregion
};