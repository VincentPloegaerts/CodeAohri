#pragma once

#include "CoreMinimal.h"
#include "ENUM_AnimState.h"
#include "ENUM_BossMovementAnimState.h"
#include "ENUM_BossSpecialAnimState.h"
#include "Animation/AnimInstance.h"
#include "B_BossAnim.generated.h"

class UP_DT_BaseDataTable;
class AB_Boss;
class UB_BossMovement;
class AM_BlackBoardManager;

UCLASS()
class INSIDEUS_API UB_BossAnim : public UAnimInstance
{
	GENERATED_BODY()

	friend class AM_BlackBoardManager;
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationEnd);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimNotifyStateEnd, const TEnumAsByte<EAnimState>&, _animState);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimMoveEnd, const TEnumAsByte<EBossMovementAnimState>&, _animMoveState);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimSpecialEnd, const TEnumAsByte<EBossSpecialAnimState>&, _animSpecialState);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimFX);

private:
	UPROPERTY()	AB_Boss* boss = nullptr;
	UPROPERTY()	UB_BossMovement* bossMovement = nullptr;	
	UPROPERTY()	AM_BlackBoardManager* blackBoardManager = nullptr;

protected:
	UPROPERTY()
	const UAnimSequenceBase* resetIdleAnim = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	const UAnimSequenceBase* currentIdleAnim = nullptr;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsAnnouncing = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequenceBase* currentAnnounce = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<EAnimState> animState = AnimState_None;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsMoving = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EBossMovementAnimState> bossMovementAnimState = EBossMovementAnimState::NONE_;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDoingSpecialAnim = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EBossSpecialAnimState> bossSpecialAnimState = EBossSpecialAnimState::NONE__;
	
	
	UPROPERTY(BlueprintReadWrite, BlueprintCallable) FOnAnimationEnd   onAnimationEnd                = FOnAnimationEnd();
	UPROPERTY(BlueprintReadWrite, BlueprintCallable) FOnAnimMoveEnd    onAnimMoveEnd                 = FOnAnimMoveEnd();
	UPROPERTY(BlueprintReadWrite, BlueprintCallable) FOnAnimSpecialEnd onAnimSpecialEnd              = FOnAnimSpecialEnd();

	UPROPERTY(BlueprintReadWrite, BlueprintCallable) FOnAnimFX onAnimFX = FOnAnimFX();
	
	UPROPERTY(BlueprintReadWrite, BlueprintCallable) FOnAnimNotifyStateEnd onStartAnimNotifyStateEnd = FOnAnimNotifyStateEnd();
	UPROPERTY(BlueprintReadWrite, BlueprintCallable) FOnAnimNotifyStateEnd onLoopAnimNotifyStateEnd  = FOnAnimNotifyStateEnd();
	UPROPERTY(BlueprintReadWrite, BlueprintCallable) FOnAnimNotifyStateEnd onEndAnimNotifyStateEnd   = FOnAnimNotifyStateEnd();

public:
	FORCEINLINE FOnAnimationEnd& OnAnimationEnd() { return onAnimationEnd; }
	FORCEINLINE FOnAnimMoveEnd& OnAnimMoveEnd() { return onAnimMoveEnd; }
	FORCEINLINE FOnAnimSpecialEnd& OnAnimSpecialEnd() { return onAnimSpecialEnd; }
	
	FORCEINLINE FOnAnimFX& OnAnimFX() { return onAnimFX; }
	
	FORCEINLINE FOnAnimNotifyStateEnd& OnStartAnimNotifyStateEnd() { return onStartAnimNotifyStateEnd; }
	FORCEINLINE FOnAnimNotifyStateEnd& OnLoopAnimNotifyStateEnd() { return onLoopAnimNotifyStateEnd; }
	FORCEINLINE FOnAnimNotifyStateEnd& OnEndAnimNotifyStateEnd() { return onEndAnimNotifyStateEnd; }

	FORCEINLINE const UAnimSequenceBase* GetCurrentAnnounce() const { return currentAnnounce; }
	FORCEINLINE const UAnimSequenceBase* GetCurrentIdleAnim() const { return currentIdleAnim; }
	
	FORCEINLINE const TEnumAsByte<EAnimState>& GetAnimState() const { return animState; }
	
	FORCEINLINE const TEnumAsByte<EBossMovementAnimState>& GetBossMovementAnimState() const { return bossMovementAnimState; }
	FORCEINLINE const TEnumAsByte<EBossSpecialAnimState>& GetBossSpecialAnimState() const { return bossSpecialAnimState; }
	
	FORCEINLINE void SetCurrentIdleAnim(const UAnimSequenceBase* _newIdleAnim) { currentIdleAnim = _newIdleAnim; }
	
private:
	virtual void NativeBeginPlay() override;
	void Init();
	UFUNCTION() void InitAnim();
	UFUNCTION() void ResetAnim();
	UFUNCTION() void Broadcast_BlackBoard();

private:
	//Stop announce animation.
	UFUNCTION() void StopPlayAnnounce();
	//Stop movement animation when boss reach his goal.
	UFUNCTION() void StopPlayMovement();
	//Stop special animation.
	UFUNCTION() void StopPlaySpecialAnim();

	UFUNCTION() void NotifyStopMovement(const TEnumAsByte<EBossMovementAnimState>& _bossMoveState);
	UFUNCTION() void NotifyStopSpecialAnim(const TEnumAsByte<EBossSpecialAnimState>& _specialAnim);
	
public:
	/* Start playing given announce animation by M_FeedBack manager.
	 * Stop other animation.*/
	UFUNCTION() void PlayAnnounce(UAnimSequenceBase* _announce, const EAnimState& _animState);
private:
	/* Start play move animation taken in B_BossMovement.
	 * Stop other animation.*/
	UFUNCTION() void PlayMovement(const EBossMovementAnimState& _bossMovementAnimState);
public:
	/* Start playing given special anim.
	 * Stop other animation.*/
	UFUNCTION() void PlaySpecialAnim(const EBossSpecialAnimState& _bossSpecialAnimState);

private:
	//Debug showing when onAnimationEnd is called.
	UFUNCTION() void DEBUG_AnimFINISHED();
};