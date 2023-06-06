#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_TN_Immune.generated.h"

class AB_Boss;

UCLASS(Abstract)
class INSIDEUS_API UBT_TN_Immune : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	AB_Boss* boss;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	virtual void ImmuneTask();
};
