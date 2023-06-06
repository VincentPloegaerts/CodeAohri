#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_TN_WaitEndPattern.generated.h"

class UP_PatternCaster;

UCLASS()
class INSIDEUS_API UBT_TN_WaitEndPattern : public UBTTaskNode
{
	GENERATED_BODY()

	/*The current tree witch this is part of.*/
	UPROPERTY()
	UBehaviorTreeComponent* tree = nullptr;
	
	/*The pattern caster on the boss.*/
	UPROPERTY()
	UP_PatternCaster* patternCaster = nullptr;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION() void OnPatternsEnd();

	virtual void PostInitProperties() override;
	virtual FString GetStaticDescription() const override;
};
