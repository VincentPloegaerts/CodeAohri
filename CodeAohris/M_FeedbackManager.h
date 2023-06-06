#pragma once
#include "CoreMinimal.h"
#include "ENUM_AnimState.h"
#include "STRUCT_AnimFX.h"
#include "STRUCT_FeedBackActor.h"

#include "STRUCT_FeedBackData.h"
#include "STRUCT_SpawnerFeedBack.h"
#include "STRUCT_TripleDeluxeFeedBack.h"
#include "STRUCT_SongFeedBackIndex.h"

#include "GameFramework/Actor.h"
#include "M_FeedbackManager.generated.h"

class UFMODAudioComponent;
class UP_DT_BaseDataTable;
class UAnimationAsset;
class AB_Boss;
class UB_BossAnim;
class UP_PatternCaster;
class UNiagaraComponent;

UCLASS()
class INSIDEUS_API AM_FeedbackManager : public AActor
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAudioComponent, UFMODAudioComponent*, _audioComponent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventInstance, FFMODEventInstance&, _eventInstance);
	
private:
	UPROPERTY(VisibleAnywhere) AB_Boss* boss = nullptr;
	UPROPERTY(VisibleAnywhere) UB_BossAnim* bossAnimInstance = nullptr;
	UPROPERTY(VisibleAnywhere) UP_PatternCaster* bossPatternCaster = nullptr;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1")) float fSubAnimDelay = 0.1f;
	

	UPROPERTY()	FFeedBackData loopFeedback = FFeedBackData();
	UPROPERTY()	FFeedBackData endFeedback = FFeedBackData();

	FFeedBackData* loopFeedbackRef = nullptr;
	FFeedBackData* endFeedbackRef = nullptr;

	UPROPERTY() bool bWaitEndOfPattern = true;
	UPROPERTY()	const AActor* requestingActor = nullptr;

	UPROPERTY() float fWorldTimeSpeed = 1.f;
	
	UPROPERTY(VisibleAnywhere) TArray<UNiagaraComponent*> allFXFeedback = TArray<UNiagaraComponent*>();
	UPROPERTY(VisibleAnywhere) mutable TArray<FSongFeedBackIndex> allSongFeedBack = TArray<FSongFeedBackIndex>();

	UPROPERTY() FAnimFX animFX = FAnimFX();

	UPROPERTY() TArray<FFeedBackActor> allGPEShotFeedBack = TArray<FFeedBackActor>();
	
	UPROPERTY()	const UWorld* world = nullptr;
	
	FTimerManager* timerManager = nullptr;

	UPROPERTY()	FTimerHandle loopFeedBackTimer = FTimerHandle();
	UPROPERTY()	FTimerHandle endFeedBackTimer = FTimerHandle();
	UPROPERTY()	FTimerHandle subAnimTimer = FTimerHandle();

protected:
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnAudioComponent onAudioComponent = FOnAudioComponent();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnEventInstance onEventInstance = FOnEventInstance();

public:
	FORCEINLINE FOnAudioComponent& OnAudioComponent() { return onAudioComponent; }
	FORCEINLINE FOnEventInstance& OnEventInstance() { return onEventInstance; }
	
public:	
	AM_FeedbackManager();

private:
	virtual void BeginPlay() override;
	UFUNCTION() void InitBossAnim();
	UFUNCTION() void ChangeBossFeedBack(AB_Boss* _boss);
	UFUNCTION() void ResetFeedBack();

	UFUNCTION() void ChangeFeedBackSpeed();
	UFUNCTION() void DestroyFeedBack();
	
	UFUNCTION() void ChangeWorldTimeSpeed(const float& _worldSpeed);

	UFUNCTION() void CheckEndLoopingSong(const AP_PatternBase* _pattern);

public:
	UFUNCTION() void PlaySound(const FSongFeedBackData& _songFeedback, const AActor* _requestingActor, float _activationDelay);
	
private:
	UFUNCTION() void PlayFX(UNiagaraSystem* _fx, USceneComponent* _attachedTo, const FVector& _offsetPosition, const FRotator& _rotation,
						    const bool& _attachFX, bool _animFX = false, bool _isGPEFX = false);
	void PlayAnim(UAnimSequenceBase* _anim, const EAnimState& _animState) const;

	UFUNCTION() void PlayAnimFX();
	UFUNCTION() void CalculateFXLocation(const FVector& _feedbackOffsetFX, const bool& _raycastFXOnGround, const TArray<TEnumAsByte<EObjectTypeQuery>>& _layer,
		                                 const float& _delayFX, UNiagaraSystem* _fxFeedback, const bool& _attachFX, const AActor* _requestingActor,
		                                 bool _animFX = false, bool _isGPEFX = false);

public:
	UFUNCTION() void RequestPlaySong(const FFeedBackData& _data, const AActor* _requestingActor);
	UFUNCTION() void RequestPlaySpawnerFeedback(const FSpawnerFeedBack& _data, const AActor* _requestingActor);
	UFUNCTION() void RequestPlayFX(const FFeedBackData& _data, const AActor* _requestingActor, bool _isGPEFX = false);
	UFUNCTION(BlueprintCallable) void PlayFeedbacks(const FFeedBackData& _data, const AActor* _requestingActor, const EAnimState& _animState);
	UFUNCTION() void RequestPlayTripleFeedbacks(const FTripleDeluxeFeedBack& _data, const AActor* _requestingActor);

private:
	UFUNCTION() void InstantFinishPattern(const UP_DT_BaseDataTable* _dataTable);
	
	UFUNCTION() void StartAnimEnd(const TEnumAsByte<EAnimState>& _animState);
	UFUNCTION() void SetStartAnimEndWillDirectFinish(const UP_DT_BaseDataTable* _dataTable);
	UFUNCTION() void DirectFinishingStartAnimEnd(const TEnumAsByte<EAnimState>& _animState);
	UFUNCTION() void LoopAnimEnd(const UP_DT_BaseDataTable* _dataTable);
	UFUNCTION() void EndAnimEnd(const TEnumAsByte<EAnimState>& _animState);

	UFUNCTION() void SubToStart();
	UFUNCTION() void SubToEnd();

public:
	UFUNCTION() void RequestPlayGPEShotFeedBack(const FTripleDeluxeFeedBack& _feedBack, const AActor* _requestingActor);

private:
	UFUNCTION() void PlayGPEShotEndFeedBack(const AActor* _requestingActor);
};