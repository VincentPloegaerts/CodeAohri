#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BT_D_CheckPlayerAFK.generated.h"

UCLASS()
class INSIDEUS_API UBT_D_CheckPlayerAFK : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float fMinimumAFKTime = 10.0f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float fMaximumAFKTime = 10.0f;
	
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void PostInitProperties() override;
	virtual FString GetStaticDescription() const override;

	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};