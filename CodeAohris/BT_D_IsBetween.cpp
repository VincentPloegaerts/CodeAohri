#include "BT_D_IsBetween.h"

#include "BehaviorTree/BlackboardComponent.h"

bool UBT_D_IsBetween::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (rangeMin > rangeMax)
		return false;
	
	const int& _value = OwnerComp.GetBlackboardComponent()->GetValueAsInt(key.SelectedKeyName);
	return _value >= rangeMin && _value <= rangeMax;
}

void UBT_D_IsBetween::PostInitProperties()
{
	Super::PostInitProperties();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	NodeName = FString("Is Between");
	key.AllowedTypes.Empty();
	key.AddIntFilter(this, FName());
#endif
}
FString UBT_D_IsBetween::GetStaticDescription() const
{
	const FString _baseStr = FString("Check if VALUE (") + key.SelectedKeyName.ToString() + FString(") is between ") + FString::FromInt(rangeMin) + FString(" and ") + FString::FromInt(rangeMax)+ FString(" ");
		
	return _baseStr;
}

#if WITH_EDITOR
void UBT_D_IsBetween::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == FName("rangeMin"))
	{
		if (rangeMin > rangeMax)
			rangeMax = rangeMin + 1;
	}
	if (PropertyChangedEvent.Property->GetFName() == FName("rangeMax"))
	{
		if (rangeMax < rangeMin )
			rangeMin = rangeMax - 1;
	}
}
#endif