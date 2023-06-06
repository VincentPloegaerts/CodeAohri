#pragma once
#include "CoreMinimal.h"

#include "IU_State.h"

#include "IU_DashingState.generated.h"

UCLASS()
class INSIDEUS_API UIU_DashingState : public UIU_State
{
	GENERATED_BODY()

	// Master boolean allowing or not to dash
	UPROPERTY(VisibleAnywhere,Category = "State",BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		bool bCanDash = true;
	
	// Master boolean allowing or not to dash
	UPROPERTY(VisibleAnywhere,Category = "State",BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		bool bDashIsInCooldown = false;

	UPROPERTY(BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		bool bIsGrounded = true;

	UPROPERTY(BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		float fAirControl = 0.0f;
	
	// DashDirection set when entering state
	UPROPERTY(VisibleAnywhere,Category = "State",BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		FVector dashDirection = FVector::ZeroVector;
	
	// DashTimer ticking for dash duration while dashing, stopping it at the end
	UPROPERTY(VisibleAnywhere, Category = "State",BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		FTimerHandle dashTimer = FTimerHandle();
	
	// Timer ticking for dash cooldown, starts when the key is pressed on ground, can't redash while this is ticking
	UPROPERTY(VisibleAnywhere,Category = "State",BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		FTimerHandle timerCooldown = FTimerHandle();
	
	// Struct containing properties for the dash, init with the one in playerMovementComponent
	UPROPERTY(VisibleAnywhere, Category = "State",BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		FDashData dashData = FDashData();


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

	// Setter for canDash
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetCanDash(const bool _status) { bCanDash = _status;}
	
	// Called when pressing the key
	UFUNCTION(BlueprintCallable) void Dash();

private:
	//called when dashTimer ends
	UFUNCTION(BlueprintCallable) void EndDash();
	
	
	//called when timerCooldown ends
	UFUNCTION(BlueprintCallable) void DashCooldown();
	UFUNCTION(BlueprintCallable) void ResetDash();

#pragma endregion
};