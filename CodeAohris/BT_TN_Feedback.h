#pragma once

#include "CoreMinimal.h"
#include "ENUM_AnimState.h"
#include "STRUCT_FeedBackData.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_TN_Feedback.generated.h"

class UB_BossAnim;

UCLASS()
class INSIDEUS_API UBT_TN_Feedback : public UBTTaskNode
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,Category = "Feedback")
	FFeedBackData feedbackData = FFeedBackData();
	
	UPROPERTY(EditAnywhere,Category = "Feedback")
	TSoftObjectPtr<AActor> spawnPointFeedback;
	
	UPROPERTY(VisibleAnywhere, Category = "Values")
	UBehaviorTreeComponent* tree = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Values")
	UB_BossAnim* bossAnim = nullptr;
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UFUNCTION(BlueprintCallable) void SpawnFeedbacks();

	UFUNCTION() void WhenBossAnimEndFinishTask(const TEnumAsByte<EAnimState>& _animState);
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};