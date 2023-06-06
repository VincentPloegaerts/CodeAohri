#pragma once

#include "CoreMinimal.h"
#include "ENUM_AnimState.h"
#include "Animation/AnimInstance.h"
#include "B_BossAnimatedStoneAnim.generated.h"

UCLASS()
class INSIDEUS_API UB_BossAnimatedStoneAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartAnimEnd);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndAnimEnd);
	
private:
	UPROPERTY()
	AB_BossAnimatedStone* bossStone = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequenceBase* currentAnim = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<EAnimState> animState = EAnimState::AnimState_None;
	
	UPROPERTY(BlueprintReadWrite, BlueprintCallable) FOnStartAnimEnd onStartAnimEnd = FOnStartAnimEnd();
	UPROPERTY(BlueprintReadWrite, BlueprintCallable) FOnEndAnimEnd onEndAnimEnd     = FOnEndAnimEnd();

public:	
	FORCEINLINE FOnStartAnimEnd& OnStartAnimEnd() { return onStartAnimEnd; }
	FORCEINLINE FOnEndAnimEnd& OnEndAnimEnd() { return onEndAnimEnd; }
	
private:
	virtual void NativeBeginPlay() override;
	UFUNCTION() void Init(AB_BossAnimatedStone* _bossStone);

public:
	UFUNCTION() void SetAnimState(const EAnimState& _animState, UAnimSequenceBase* _anim);
	
};