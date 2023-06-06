#pragma once

#include "CoreMinimal.h"
#include "STRUCT_SongFeedBackData.h"
#include "BehaviorTree/BTDecorator.h"
#include "BT_D_PlaySound.generated.h"

class UBT_D_BossMovement;

UCLASS()
class INSIDEUS_API UBT_D_PlaySound : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY()
	bool bTryGetNextBossMovement = false;

	UPROPERTY()
	unsigned iPatternExecutionIndex = 0;
	
	UPROPERTY(EditAnywhere)
	FSongFeedBackData sound = FSongFeedBackData();

	UPROPERTY()
	UBT_D_BossMovement* bossMovement = nullptr;
	
public:
	UBT_D_PlaySound();
	
private:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

	UFUNCTION() void PlaySound(const bool _isStun);
};