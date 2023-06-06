#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BT_D_IsBetween.generated.h"

UCLASS()
class INSIDEUS_API UBT_D_IsBetween : public UBTDecorator
{
	GENERATED_BODY()

	friend class UBT_TN_RandomizeValue;

	UPROPERTY(EditAnywhere, Category = "Is Between")
	FBlackboardKeySelector key;
	UPROPERTY(EditAnywhere, Category = "Is Between", meta = (ClampMin = "0", ClampMax = "99"))
	int rangeMin = 0;
	UPROPERTY(EditAnywhere, Category = "Is Between", meta = (ClampMin = "1", ClampMax = "100"))
	int rangeMax = 100;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	virtual void PostInitProperties() override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
