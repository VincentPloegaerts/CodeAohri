#include "BT_TN_Feedback.h"

#include "B_Boss.h"
#include "B_BossAnim.h"
#include "IU_Macros.h"
#include "M_FeedbackManager.h"
#include "M_GameMode.h"

EBTNodeResult::Type UBT_TN_Feedback::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	tree = &OwnerComp;
	LOG_ERROR("UBT_TN_Feedback::ExecuteTask -> BT_TN_FeedBack IS DEPRECATED");
	
	if (const float& _delay = feedbackData.fBehaviorActivationDelay > 0.f)
	{
		FTimerHandle _handle;
		GetWorld()->GetTimerManager().SetTimer(_handle,this,&UBT_TN_Feedback::SpawnFeedbacks,_delay,false);
		return EBTNodeResult::InProgress;
	}
	SpawnFeedbacks();
	return EBTNodeResult::InProgress;
}

void UBT_TN_Feedback::SpawnFeedbacks()
{
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		AM_FeedbackManager* _fbManager = _gameMode->GetFeedbackManager();
		_fbManager->PlayFeedbacks(feedbackData, spawnPointFeedback.Get(), AnimState_Start);

		if (const AB_Boss* _boss = _gameMode->GetBoss())
		{
			bossAnim = _boss->GetBossAnim();
			if (bossAnim)
				Event_AddUnique(bossAnim->OnStartAnimNotifyStateEnd(), UBT_TN_Feedback::WhenBossAnimEndFinishTask);
		}
	}
}

void UBT_TN_Feedback::WhenBossAnimEndFinishTask(const TEnumAsByte<EAnimState>& _animState)
{
	ROG_ERROR(_animState != AnimState_Start, "UBT_TN_Feedback::WhenBossAnimEndFinishTask -> Invalid State");
	if (bossAnim->GetCurrentAnnounce() == feedbackData.animationFeedback)
		tree->OnTaskFinished(this, EBTNodeResult::Succeeded);
	else
		LOG_ERROR("UBT_TN_Feedback::WhenBossAnimEndFinishTask -> Current announce and animation feedback");
}

void UBT_TN_Feedback::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (bossAnim)
		Event_Remove(bossAnim->OnStartAnimNotifyStateEnd(), UBT_TN_Feedback::WhenBossAnimEndFinishTask);
}