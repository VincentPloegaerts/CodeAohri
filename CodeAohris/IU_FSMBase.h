#pragma once

#include "CoreMinimal.h"
#include "IU_FSMBase.generated.h"

class UIU_PlayerFSMBrain;

UCLASS(Abstract)
class INSIDEUS_API UIU_FSMBase : public UObject
{
	GENERATED_BODY()
	
public:
	// Ref on FSMBrainComponent
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
		UIU_PlayerFSMBrain* brain = nullptr;

public:
	// Called once on begin play settings properties
	UFUNCTION(BlueprintCallable)virtual void Init(UIU_PlayerFSMBrain* _brain);

	// Called once when entering state
	UFUNCTION(BlueprintCallable)virtual void Enter();
	
	// Called once when exiting state
	UFUNCTION(BlueprintCallable)virtual void Exit();
};