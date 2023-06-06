#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_TN_AnimChanger.generated.h"

UCLASS()
class INSIDEUS_API UBT_TN_AnimChanger : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	const UAnimSequenceBase* newIdleAnim = nullptr;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};