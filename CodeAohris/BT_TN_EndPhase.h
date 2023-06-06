#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_TN_EndPhase.generated.h"

class AB_AIBoss;

UCLASS()
class INSIDEUS_API UBT_TN_EndPhase : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY()
	UBehaviorTreeComponent* tree = nullptr;
	UPROPERTY()
	AB_AIBoss* bossAI = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "End Phase", meta = (ClampMin = "0", ClampMax = "60"))
	float fEndTime = 0;
	UPROPERTY(EditAnywhere, Category = "End Phase")
	UBehaviorTree* nextTree = nullptr;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION() void PhaseEnd();

	virtual void PostInitProperties() override;
	virtual FString GetStaticDescription() const override;
};
