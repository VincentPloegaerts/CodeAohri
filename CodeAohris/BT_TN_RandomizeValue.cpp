#include "BT_TN_RandomizeValue.h"

#include "BT_D_IsBetween.h"
#include "IU_Macros.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTCompositeNode.h"

EBTNodeResult::Type UBT_TN_RandomizeValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TArray<UBT_D_IsBetween*> _allIsBetween;
	
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
				if (UBT_D_IsBetween* _isBetween = Cast<UBT_D_IsBetween>(_decorator[j]))
				{
					if (_isBetween != valuesToAvoid)
						_allIsBetween.Add(_isBetween);
				}
			}
		}
	}

	bool _isValid = false;
	const size_t& _max = _allIsBetween.Num();
	int _random = 0;

	for (size_t i = 0; i < PASTIS; ++i)
	{
		_random = FMath::RandRange(minValue, maxValue);

		for (size_t j = 0; j < _max; ++j)
		{
			UBT_D_IsBetween* _isBetween = _allIsBetween[j];
			if (_isBetween != valuesToAvoid)
			{
				if (!valuesToAvoid)
				{
					if (_random > _isBetween->rangeMin && _random < _isBetween->rangeMax)
					{
						//lastValuesToAvoid = valuesToAvoid;
						valuesToAvoid = _isBetween;
						_isValid = true;
						break;
					}
					continue;
				}
				//if (_random < valuesToAvoid->rangeMin || _random > valuesToAvoid->rangeMax)
				if (_random > _isBetween->rangeMin && _random < _isBetween->rangeMax)
				{
					//lastValuesToAvoid = valuesToAvoid;
					valuesToAvoid = _isBetween;
					_isValid = true;
					break;
				}
			}
		}

		if (_isValid)
			break;
	}

	//if (lastValuesToAvoid)
	//	LOG_ERROR("%s  new -> %s  last -> %s : %d", *GetName(), *valuesToAvoid->GetName(), *lastValuesToAvoid->GetName(), _random);

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(key.SelectedKeyName, _random);
	
	return EBTNodeResult::Succeeded;
}

void UBT_TN_RandomizeValue::PostInitProperties()
{
	Super::PostInitProperties();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	NodeName = FString("Randomize Value");
	key.AllowedTypes.Empty();
	key.AddIntFilter(this, FName());
#endif
}

FString UBT_TN_RandomizeValue::GetStaticDescription() const
{
	return FString("Randomize the value : ") + key.SelectedKeyName.ToString() + FString(" ");
}

#if WITH_EDITOR
void UBT_TN_RandomizeValue::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == FName("minValue"))
	{
		if (minValue > maxValue)
			maxValue = minValue + 1;
	}
	if (PropertyChangedEvent.Property->GetFName() == FName("maxValue"))
	{
		if (maxValue < minValue )
			minValue = maxValue - 1;
	}
}
#endif