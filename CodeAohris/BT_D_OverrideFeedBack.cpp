#include "BT_D_OverrideFeedBack.h"

#include "BT_TN_CastPattern.h"
#include "IU_Macros.h"
#include "BehaviorTree/BTCompositeNode.h"

UBT_D_OverrideFeedBack::UBT_D_OverrideFeedBack()
{
	bNotifyActivation = true;
}

void UBT_D_OverrideFeedBack::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);
	OverrideCasterFeedBack();
}

void UBT_D_OverrideFeedBack::OverrideCasterFeedBack() const
{
	if (UBTCompositeNode* _node = GetParentNode())
	{
		TArray<FBTCompositeChild>& _children = _node->Children;

		const uint& _max = _children.Num();					
		for (uint i(0); i < _max; ++i)
		{
			FBTCompositeChild& _compositeChild = _children[i];

			TArray<TObjectPtr<UBTDecorator>>& _decorator = _compositeChild.Decorators;
			const uint& _max2 = _decorator.Num();
			for (uint j(0); j < _max2; ++j)
			{
				if (const UBT_D_OverrideFeedBack* _checkBossLife = Cast<UBT_D_OverrideFeedBack>(_decorator[j]))
				{
					if (GetExecutionIndex() == _checkBossLife->GetExecutionIndex())
					{
						if (UBT_TN_CastPattern* _patternOwner = Cast<UBT_TN_CastPattern>(_compositeChild.ChildTask))
						{
#if WITH_LOG_OVERRIDE_FEEDBACK
							LOG_ERROR("Decorator : Override Triple Deluxe Feedback");
#endif
							_patternOwner->OverrideTripleFeedBack(&overrideTripleFeedBack);
						}
						return;
					}
				}
			}
		}
	}
}