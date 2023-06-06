#pragma once
#include "CoreMinimal.h"

#include "IU_State.h"

#include "IU_MovingOnGroundState.generated.h"

UCLASS()
class INSIDEUS_API UIU_MovingOnGroundState : public UIU_State
{
	GENERATED_BODY()
	
	// Raycast and collisions hit result update from what's under the player
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FHitResult groundResult = FHitResult();
	
	// Struct containing movement on ground properties init from playerMovementComponent
	UPROPERTY(VisibleAnywhere, Category = "State",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FMoveOnGroundData movingOnGroundStats = FMoveOnGroundData();
	
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

	#pragma endregion

	#pragma region Custom Methods
	
	// Called in update to move the player with inputs and collisions
	UFUNCTION(BlueprintCallable) void MovePlayerOnGround(float _deltaTime, const FHitResult& _groundResult) const;

	#pragma endregion
};