#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_TN_WaitBB.generated.h"

class AM_BlackBoardManager;
UCLASS()
class INSIDEUS_API UBT_TN_WaitBB : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	float fWaitTime = 0.f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	float fWaitTimeDeviation = 0.f;
	
	UPROPERTY()
	float fCurrentTime = 0.f;
	
	UPROPERTY()
	float fTimeToWait = 0.f;
	
	UPROPERTY(EditAnywhere)
	bool bSkipIfStun = false;

	UPROPERTY()
	AM_BlackBoardManager* blackBoardManager = nullptr;

public:
	UBT_TN_WaitBB();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void PostInitProperties() override;
	virtual FString GetStaticDescription() const override;
};