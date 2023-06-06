#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_TN_RandomizeValue.generated.h"

class UBT_D_IsBetween;

UCLASS()
class INSIDEUS_API UBT_TN_RandomizeValue : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Randomize Value")
	FBlackboardKeySelector key;
	UPROPERTY(EditAnywhere, Category = "Randomize Value", meta = (ClampMin = "0", ClampMax = "99"))
	int minValue = 0;
	UPROPERTY(EditAnywhere, Category = "Randomize Value", meta = (ClampMin = "1", ClampMax = "100"))
	int maxValue = 100;

	UPROPERTY()	UBT_D_IsBetween* valuesToAvoid = nullptr;
	//UPROPERTY()	UBT_D_IsBetween* lastValuesToAvoid = nullptr;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void PostInitProperties() override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
