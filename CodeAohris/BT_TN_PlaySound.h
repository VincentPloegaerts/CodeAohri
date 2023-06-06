#pragma once

#include "CoreMinimal.h"
#include "STRUCT_SongFeedBackData.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_TN_PlaySound.generated.h"

UCLASS()
class INSIDEUS_API UBT_TN_PlaySound : public UBTTaskNode
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TArray<FSongFeedBack> soundFeedBack = TArray<FSongFeedBack>();

public:
	UBT_TN_PlaySound();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};