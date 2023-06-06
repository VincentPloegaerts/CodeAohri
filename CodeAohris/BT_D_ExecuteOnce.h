#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BT_D_ExecuteOnce.generated.h"

UCLASS()
class INSIDEUS_API UBT_D_ExecuteOnce : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Key")
	FBlackboardKeySelector key;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void PostInitProperties() override;
	virtual FString GetStaticDescription() const override;
};
