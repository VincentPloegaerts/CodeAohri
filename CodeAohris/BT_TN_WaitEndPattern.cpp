#include "BT_TN_WaitEndPattern.h"

#include "B_AIBoss.h"

#include "IU_Macros.h"
#include "P_PatternCaster.h"

EBTNodeResult::Type UBT_TN_WaitEndPattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	tree = &OwnerComp;
	if (const AB_AIBoss* _bossAI = Cast<AB_AIBoss>(tree->GetAIOwner()))
	{
		patternCaster = _bossAI->GetPatternCaster();
		if (!patternCaster)
		{
			LOG("UBT_TN_CastPattern::ExecuteTask -> Invalid pattern caster");
			return EBTNodeResult::Failed;			
		}
		
		if (patternCaster->GetIsPatternListEmpty())
			return EBTNodeResult::Succeeded;
		
		patternCaster->OnPatternListEmpty().AddDynamic(this, &UBT_TN_WaitEndPattern::OnPatternsEnd);
		return EBTNodeResult::InProgress;
	}
	
	LOG("UBT_TN_CastPattern::ExecuteTask -> Invalid AIController");
	return EBTNodeResult::Failed;
}

void UBT_TN_WaitEndPattern::OnPatternsEnd()
{
	patternCaster->OnPatternListEmpty().RemoveDynamic(this, &UBT_TN_WaitEndPattern::OnPatternsEnd);
	tree->OnTaskFinished(this, EBTNodeResult::Succeeded);
}

void UBT_TN_WaitEndPattern::PostInitProperties()
{
	Super::PostInitProperties();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	NodeName = FString("Wait End Pattern");
#endif
}
FString UBT_TN_WaitEndPattern::GetStaticDescription() const
{
	return FString("Wait the end of Patterns ") + LINE_TERMINATOR + FString("(Casted Patterns & Patterns that will be cast ");
}
