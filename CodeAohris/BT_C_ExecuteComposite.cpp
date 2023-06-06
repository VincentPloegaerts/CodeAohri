#include "BT_C_ExecuteComposite.h"

UBT_C_ExecuteComposite::UBT_C_ExecuteComposite(const FObjectInitializer& ObjectInitializer) : Super::UBTCompositeNode(ObjectInitializer)
{
	NodeName = "Never Loop";
}

int32 UBT_C_ExecuteComposite::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild,	EBTNodeResult::Type LastResult) const
{
	int32 _nextChildIndex = BTSpecialChild::ReturnToParent;

	if (PrevChild == BTSpecialChild::NotInitialized)
	{
		_nextChildIndex = 0;
	}
	else if (PrevChild + 1 < GetChildrenNum())
	{
		_nextChildIndex = PrevChild + 1;
	}
	
	return _nextChildIndex;
}
#if WITH_EDITOR
FName UBT_C_ExecuteComposite::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Selector.Icon");
}
#endif
