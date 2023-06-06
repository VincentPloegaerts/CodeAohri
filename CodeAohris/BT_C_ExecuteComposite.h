#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BT_C_ExecuteComposite.generated.h"

UCLASS()
class INSIDEUS_API UBT_C_ExecuteComposite : public UBTCompositeNode
{
	GENERATED_BODY()

	UBT_C_ExecuteComposite(const FObjectInitializer& ObjectInitializer);
	
	virtual int32 GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
	
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif
};