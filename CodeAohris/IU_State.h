#pragma once
#include "CoreMinimal.h"

#include "IU_FSMBase.h"

#include "IU_PlayerMovementComponent.h"
#include "IU_State.generated.h"

//Enum of every state possible to identify them
UENUM(BlueprintType)
enum EStateType
{
	Moving,
	Dashing,
	Jumping,
	Falling,
	None
};

UCLASS(Blueprintable, BlueprintType)
class INSIDEUS_API UIU_State : public UIU_FSMBase
{
	GENERATED_BODY()
	
protected:
	//EStateType identifying state
	UPROPERTY(VisibleAnywhere,Category = "State",BlueprintReadWrite)
		TEnumAsByte<EStateType> stateType = EStateType::None;
	
	//playerMovementComponent ref used all over the states
	UPROPERTY(BlueprintReadWrite)
		UIU_PlayerMovementComponent* playerMovementComponent = nullptr;
	
public:
	FORCEINLINE EStateType StateType() const {return stateType; }
	
public:
	// Called once on begin play settings properties
	virtual void Init(UIU_PlayerFSMBrain* _brain) override;
	
	// Called once every frame when state is the current one in the brain
	UFUNCTION(BlueprintCallable) virtual void Update(const float _deltaTime);
	
	// Override this to check conditions for entering state
	UFUNCTION(BlueprintCallable) virtual bool CanEnterState();
	
	// Called by playerMovementComponent to pass on its data
	UFUNCTION(BlueprintCallable)  virtual void PassData(const FMovementData& _data);
};
