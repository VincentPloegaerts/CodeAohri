#include "BT_TN_Immune.h"

#include "B_Boss.h"

#include "IU_Macros.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBT_TN_Immune::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UObject* _object = OwnerComp.GetBlackboardComponent()->GetValueAsObject("Boss"))
	{
		boss = Cast<AB_Boss>(_object);
		if (boss)
			ImmuneTask();
		else
		{
			LOG("UBT_TN_Immune::ExecuteTask -> Invalid KeyName");
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		LOG("UBT_TN_Immune::ExecuteTask -> Invalid KeyName, please add a 'Boss' key Object");
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Succeeded;
}

void UBT_TN_Immune::ImmuneTask()
{
	//Declaration in Child class
}