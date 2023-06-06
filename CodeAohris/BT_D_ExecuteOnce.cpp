#include "BT_D_ExecuteOnce.h"

#include "BehaviorTree/BlackboardComponent.h"

bool UBT_D_ExecuteOnce::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const bool& _keyState = OwnerComp.GetBlackboardComponent()->GetValueAsBool(key.SelectedKeyName);
	if (_keyState)
		return false;
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(key.SelectedKeyName, true);		
	return true;
}

void UBT_D_ExecuteOnce::PostInitProperties()
{
	Super::PostInitProperties();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	NodeName = FString("Execute Once");
	key.AllowedTypes.Empty();
	key.AddBoolFilter(this, FName());
#endif
	
}
FString UBT_D_ExecuteOnce::GetStaticDescription() const
{
	const FString& _baseStr = FString("Check if BT is already pass") + LINE_TERMINATOR + FString ("(Use ") + key.SelectedKeyName.ToString() + FString(") ");
	
	return _baseStr;
}
