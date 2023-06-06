#include "BT_TN_EndPhase.h"

#include "B_AIBoss.h"
#include "ENUM_SpawnSettings.h"
#include "BehaviorTree/BehaviorTree.h"

EBTNodeResult::Type UBT_TN_EndPhase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	tree = &OwnerComp;
	bossAI = Cast<AB_AIBoss>(OwnerComp.GetAIOwner());

	if (!bossAI)
		return EBTNodeResult::Failed;

	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &UBT_TN_EndPhase::PhaseEnd, fEndTime);
	
	return EBTNodeResult::InProgress;
}

void UBT_TN_EndPhase::PhaseEnd()
{
	if (!tree || !bossAI) return;
	bossAI->SetBehaviorTree(nextTree);
	tree->OnTaskFinished(this, EBTNodeResult::Succeeded);
}

void UBT_TN_EndPhase::PostInitProperties()
{
	Super::PostInitProperties();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	NodeName = FString("End Phase");
#endif
}
FString UBT_TN_EndPhase::GetStaticDescription() const
{
	FString _treeName = nextTree ? nextTree->GetName() : FString("NONE");
	return FString("Call at the end of the Phase") + LINE_TERMINATOR + FString("Set Behavior Tree (") + _treeName + FString(") to Boss ");
}
