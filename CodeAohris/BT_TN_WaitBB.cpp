#include "BT_TN_WaitBB.h"

#include "M_BlackBoardManager.h"
#include "M_GameMode.h"
#include "BehaviorTree/BlackboardComponent.h"

UBT_TN_WaitBB::UBT_TN_WaitBB()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBT_TN_WaitBB::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (bSkipIfStun)
	{
		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsStun"))
			return EBTNodeResult::Succeeded;
	}

	fTimeToWait = FMath::RandRange(fWaitTime - fWaitTimeDeviation, fWaitTime + fWaitTimeDeviation);

	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		blackBoardManager = _gameMode->GetBlackBoardManager();
	}		
	
	return blackBoardManager ? EBTNodeResult::InProgress : EBTNodeResult::Failed;
}

void UBT_TN_WaitBB::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	blackBoardManager->UPDATE_DEBUG();
	
	fCurrentTime += DeltaSeconds;
	if (fCurrentTime >= fTimeToWait)
	{
		fCurrentTime = 0.f;
		OwnerComp.OnTaskFinished(this, EBTNodeResult::Succeeded);
	}
}

void UBT_TN_WaitBB::PostInitProperties()
{
	Super::PostInitProperties();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	NodeName = FString("Wait BB");
#endif
}

FString UBT_TN_WaitBB::GetStaticDescription() const
{
	return FString::SanitizeFloat(fWaitTime, 2) +
		   "+-" +
		   FString::SanitizeFloat(fWaitTimeDeviation, 2) +
		   FString(" sec -") + " Stun : " +
		   (bSkipIfStun ? "true" : "false");
}