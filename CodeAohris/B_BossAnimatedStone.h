#pragma once

#include "CoreMinimal.h"
#include "IU_Macros.h"
#include "ENUM_AnimState.h"
#include "STRUCT_FeedBackData.h"
#include "GameFramework/Actor.h"
#include "B_BossAnimatedStone.generated.h"

class UB_BossAnimatedStoneAnim;
class UP_DT_BaseDataTable;

UCLASS()
class INSIDEUS_API AB_BossAnimatedStone : public AActor
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndAnimation, AB_BossAnimatedStone*, _bossStone);
	
private:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* skeletalMesh = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UB_BossAnimatedStoneAnim* animInstance = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	unsigned iPatternIndex = 0;
	
	UPROPERTY(VisibleAnywhere)
	const UP_DT_BaseDataTable* datatable = nullptr;


	UPROPERTY() AM_FeedbackManager* feedBackManager = nullptr;
	
	UPROPERTY()	FOnEndAnimation onEndAnimation = FOnEndAnimation();
	UPROPERTY() FTimerHandle feedbackTimer = FTimerHandle();

	
public:
	FORCEINLINE const uint& GetPatternIndex() const { return iPatternIndex; }
	FORCEINLINE const UP_DT_BaseDataTable* GetDataTable() const { return datatable; }
	
	FORCEINLINE void SetPatternIndex(const uint& _patternIndex) { iPatternIndex = _patternIndex; }
	FORCEINLINE void SetDataTable(const UP_DT_BaseDataTable* _datatable) { datatable = _datatable; }

	FORCEINLINE FOnEndAnimation& OnEndAnimation() { return onEndAnimation; }
	
public:
	AB_BossAnimatedStone();
	
private:
	virtual void BeginPlay() override;
	UFUNCTION() void Init();

public:
	UFUNCTION() void PlayStartAnimation(AM_FeedbackManager* _feedbackManager);
private:
	UFUNCTION() void PlayLoopAnimation();
public:
	UFUNCTION() void PlayEndAnimation();
	
private:
	UFUNCTION() void PlayFeedBack(const EAnimState& _animState, UAnimSequenceBase* _anim, const FFeedBackData& _feedBack) const;
	UFUNCTION() void BroadcastEndAnimation();
	
};