#include "M_FeedbackManager.h"

#include "B_Boss.h"
#include "B_BossAnim.h"
#include "FMODAudioComponent.h"
#include "FMODBlueprintStatics.h"
#include "IU_Macros.h"
#include "M_BossManager.h"
#include "M_GameMode.h"
#include "M_WorldTimeManager.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "P_PatternBase.h"
#include "P_PatternCaster.h"
#include "STRUCT_SongFeedBackData.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

AM_FeedbackManager::AM_FeedbackManager()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
}

void AM_FeedbackManager::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	
	timerManager = &world->GetTimerManager();
	FTimerHandle _timer;
	timerManager->SetTimer(_timer, this, &AM_FeedbackManager::InitBossAnim, 0.1f);
}

void AM_FeedbackManager::InitBossAnim()
{
	const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>();
	ROG_ERROR(!_gameMode, "AM_FeedbackManager::InitBossAnim -> Invalid gamemode");

	AM_BossManager* _bossManager = _gameMode->GetBossManager();
	ROG_ERROR(!_bossManager, "AM_FeedbackManager::InitBossAnim -> Invalid bossManager");

	if (AM_WorldTimeManager* _worldTimeManager = _gameMode->GetWorldTimeManager())
		Event_AddUnique(_worldTimeManager->OnWorldSpeedChange(), AM_FeedbackManager::ChangeWorldTimeSpeed);
	
	Event_Add(_bossManager->OnBossChanged(), AM_FeedbackManager::ChangeBossFeedBack);
	boss = _bossManager->GetCurrentBoss();
	if (boss)
	{
		bossAnimInstance = boss->GetBossAnim();
		bossPatternCaster = boss->GetPatternCaster();
		
		Event_AddUnique(bossPatternCaster->OnPatternComplete(), AM_FeedbackManager::CheckEndLoopingSong);    //
		
		Event_AddUnique(boss->OnBossStunned(), AM_FeedbackManager::ResetFeedBack);
		Event_AddUnique(boss->OnBossMidLifeAnimStart(), AM_FeedbackManager::ResetFeedBack);
		Event_AddUnique(boss->OnBossDead(), AM_FeedbackManager::ResetFeedBack);
		Event_AddUnique(boss->OnBossReset(), AM_FeedbackManager::ResetFeedBack);
		
		Event_AddUnique(boss->OnBossDead(), AM_FeedbackManager::DestroyFeedBack);
		Event_AddUnique(boss->OnBossReset(), AM_FeedbackManager::DestroyFeedBack);
		ROG_ERROR(!bossAnimInstance || !bossPatternCaster, "AM_FeedbackManager::InitBossAnim -> Invalid Anim or pattern caster on boss : %s", *boss->GetName());
	}
}

void AM_FeedbackManager::ChangeBossFeedBack(AB_Boss* _boss)
{
	ROG(!_boss, "AM_FeedbackManager::ChangeBossFeedBack -> invalid boss");
	
	Event_Remove(bossPatternCaster->OnPatternComplete(), AM_FeedbackManager::CheckEndLoopingSong);    //
	
	Event_Remove(boss->OnBossStunned(), AM_FeedbackManager::ResetFeedBack);
	Event_Remove(boss->OnBossMidLifeAnimStart(), AM_FeedbackManager::ResetFeedBack);
	Event_Remove(boss->OnBossDead(), AM_FeedbackManager::ResetFeedBack);
	Event_Remove(boss->OnBossReset(), AM_FeedbackManager::ResetFeedBack);
	
	Event_Remove(boss->OnBossDead(), AM_FeedbackManager::DestroyFeedBack);
	Event_Remove(boss->OnBossReset(), AM_FeedbackManager::DestroyFeedBack);
	
	boss = _boss;

	if (UP_PatternCaster* _patternCaster = boss->GetPatternCaster())
		Event_AddUnique(_patternCaster->OnPatternComplete(), AM_FeedbackManager::CheckEndLoopingSong);    //
	
	Event_AddUnique(boss->OnBossStunned(), AM_FeedbackManager::ResetFeedBack);
	Event_AddUnique(boss->OnBossMidLifeAnimStart(), AM_FeedbackManager::ResetFeedBack);
	Event_AddUnique(boss->OnBossDead(), AM_FeedbackManager::ResetFeedBack);
	Event_AddUnique(boss->OnBossReset(), AM_FeedbackManager::ResetFeedBack);
	
	Event_AddUnique(boss->OnBossDead(), AM_FeedbackManager::DestroyFeedBack);
	Event_AddUnique(boss->OnBossReset(), AM_FeedbackManager::DestroyFeedBack);
	
	bossAnimInstance = _boss->GetBossAnim();
	bossPatternCaster = _boss->GetPatternCaster();
	
	ROG_ERROR(!bossAnimInstance || !bossPatternCaster, "AM_FeedbackManager::ChangeBossFeedBack -> Invalid Anim or pattern caster on boss : %s", *_boss->GetName());
}

void AM_FeedbackManager::ResetFeedBack()
{
	UB_BossAnim::FOnAnimNotifyStateEnd& _startNotify = bossAnimInstance->OnStartAnimNotifyStateEnd();
	Event_Remove(_startNotify, AM_FeedbackManager::StartAnimEnd);
	Event_Remove(_startNotify, AM_FeedbackManager::DirectFinishingStartAnimEnd);

	UP_PatternCaster::FOnDataTableEnd& _dataTableEnd = bossPatternCaster->OnDataTableEnd();
	Event_Remove(_dataTableEnd, AM_FeedbackManager::SetStartAnimEndWillDirectFinish);
	Event_Remove(_dataTableEnd, AM_FeedbackManager::LoopAnimEnd);
	
	Event_Remove(bossAnimInstance->OnEndAnimNotifyStateEnd(), AM_FeedbackManager::EndAnimEnd);
	
	timerManager->ClearTimer(loopFeedBackTimer);
	timerManager->ClearTimer(endFeedBackTimer);
	timerManager->ClearTimer(subAnimTimer);
	
	loopFeedbackRef = nullptr;
	endFeedbackRef = nullptr;
}

void AM_FeedbackManager::ChangeFeedBackSpeed()
{
	const size_t& _max = allFXFeedback.Num();
	
	for (size_t i = 0; i < _max; ++i)
	{
		UNiagaraComponent* _niagara = allFXFeedback[i];
		if (!IsValid(_niagara))continue;
		_niagara->SetCustomTimeDilation(fWorldTimeSpeed);
		_niagara->AdvanceSimulation(1, fWorldTimeSpeed);
	}
}

void AM_FeedbackManager::DestroyFeedBack()
{
	const size_t& _max = allFXFeedback.Num();
	
	for (size_t i = 0; i < _max; ++i)
	{
		UNiagaraComponent* _niagara = allFXFeedback[i];
		if (!IsValid(_niagara))continue;
		_niagara->DestroyInstance();
	}
	
	allFXFeedback.Empty();
}

void AM_FeedbackManager::ChangeWorldTimeSpeed(const float& _worldSpeed)
{
	fWorldTimeSpeed = _worldSpeed;
	ChangeFeedBackSpeed();
}

void AM_FeedbackManager::CheckEndLoopingSong(const AP_PatternBase* _pattern)
{
	if (!_pattern)return;
	const unsigned& _index = _pattern->GetPatternIndex();

	size_t _max = allSongFeedBack.Num();
	for (size_t i = 0; i < _max; ++i)
	{
		FSongFeedBackIndex& _feedBack = allSongFeedBack[i];
		if (_feedBack.iPatternIndex == _index)
		{
			if (UFMODAudioComponent* _audio = _feedBack.audioComponent)
			{
				onAudioComponent.Broadcast(_audio);
			}

			if (FMOD::Studio::EventInstance* _event = _feedBack.eventInstance.Instance)
			{
				onEventInstance.Broadcast(_feedBack.eventInstance);
				_event->stop(FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT);
			}
			
			allSongFeedBack.RemoveAt(i);
			allSongFeedBack.Shrink();
			
			--_max;
			--i;
		}
	}
}

void AM_FeedbackManager::PlaySound(const FSongFeedBackData& _songFeedback, const AActor* _requestingActor, float _activationDelay)
{
	if (!_requestingActor)return;

	if (_activationDelay > 0.f)
	{
		FTimerHandle _handle;
		FTimerDelegate _delegate;
		_delegate.BindUFunction(this, "PlaySound",_songFeedback, _requestingActor, 0.f);
		timerManager->SetTimer(_handle, _delegate, _activationDelay, false);
		return;
	}
	
	const TArray<FSongFeedBack>& _soundFeedBack = _songFeedback.soundFeedBack;
	const size_t& _max = _soundFeedBack.Num();
	for (size_t i = 0; i < _max; ++i)
	{
		const FSongFeedBack& _songSettings = _soundFeedBack[i];
		if (UFMODEvent* _sound = _songSettings.soundFeedback)
		{
			if (_songSettings.bAttachToRequestingActor)
			{
				if (_songSettings.bIsLoopingSong)
				{	
					FSongFeedBackIndex _fb;
					_fb.audioComponent = UFMODBlueprintStatics::PlayEventAttached(_sound, _requestingActor->GetRootComponent(), NAME_None,
						                                                          _songSettings.vSoundOffset, EAttachLocation::KeepRelativeOffset,
						                                                          true, true, true);
					_fb.iPatternIndex = bossPatternCaster->GetPatternIndex();
					allSongFeedBack.Add(_fb);
				}
				else
				{
					UFMODBlueprintStatics::PlayEventAttached(_sound, _requestingActor->GetRootComponent(), NAME_None,
															 _songSettings.vSoundOffset, EAttachLocation::KeepRelativeOffset,
															 false, true, true);
				}
			}
			else
			{
				if (_songSettings.bIsLoopingSong)
				{
					FSongFeedBackIndex _fb;
					_fb.eventInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), _sound, _requestingActor->GetTransform(), true);
					_fb.iPatternIndex = bossPatternCaster->GetPatternIndex();
					allSongFeedBack.Add(_fb);
				}
				else
				{
					UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), _sound, _requestingActor->GetTransform(), true);
				}
			}
		}
	}
}

void AM_FeedbackManager::PlayFX(UNiagaraSystem* _fx, USceneComponent* _attachedTo, const FVector& _offsetPosition,
								const FRotator& _rotation, const bool& _attachFX, bool _animFX, bool _isGPEFX)
{
	if (!_fx || !_attachedTo)return;

	if (_animFX)
	{
		Event_AddUnique(bossAnimInstance->OnAnimFX(), AM_FeedbackManager::PlayAnimFX);
		animFX.fx = _fx;
		animFX.rotation = _rotation;
		animFX.attachedTo = _attachedTo;
		animFX.offsetPosition = _offsetPosition;
		animFX.attachFX = _attachFX;
		return;
	}
	
	UNiagaraComponent* _niagaraComponent;
	if (_attachFX)
	{
		_niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(_fx, _attachedTo, NAME_None, _offsetPosition,
																		 _rotation, EAttachLocation::Type::KeepRelativeOffset,
																		 true);
	}
	else
	{
		_niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), _fx,
		                                                                   _attachedTo->GetComponentLocation() +
		                                                                   _offsetPosition, _rotation);
	}

	if (_isGPEFX)return;
	
	if (!IsValid(_niagaraComponent))return;
	_niagaraComponent->SetCustomTimeDilation(fWorldTimeSpeed);
	_niagaraComponent->AdvanceSimulation(1, fWorldTimeSpeed);
	allFXFeedback.Add(_niagaraComponent);
}

void AM_FeedbackManager::PlayAnim(UAnimSequenceBase* _anim, const EAnimState& _animState) const
{
	if (!bossAnimInstance)return;
	bossAnimInstance->PlayAnnounce(_anim, _animState);
}

void AM_FeedbackManager::PlayAnimFX()
{
	Event_Remove(bossAnimInstance->OnAnimFX(), AM_FeedbackManager::PlayAnimFX);

	UNiagaraComponent* _niagaraComponent;
	if (animFX.attachFX)
	{
		_niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(animFX.fx, animFX.attachedTo, NAME_None, animFX.offsetPosition,
																		 animFX.rotation, EAttachLocation::Type::KeepRelativeOffset,
																		 true);
	}
	else
	{
		_niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), animFX.fx,
																		   animFX.attachedTo->GetComponentLocation() +
																		   animFX.offsetPosition, animFX.rotation);
	}
	
	if (!IsValid(_niagaraComponent))return;
	_niagaraComponent->SetCustomTimeDilation(fWorldTimeSpeed);
	_niagaraComponent->AdvanceSimulation(1, fWorldTimeSpeed);
	allFXFeedback.Add(_niagaraComponent);
}

void AM_FeedbackManager::CalculateFXLocation(const FVector& _feedbackOffsetFX, const bool& _raycastFXOnGround,
                                             const TArray<TEnumAsByte<EObjectTypeQuery>>& _layer, const float& _delayFX,
                                             UNiagaraSystem* _fxFeedback, const bool& _attachFX, const AActor* _requestingActor,
                                             bool _animFX, bool _isGPEFX)
{
	if (_requestingActor)
	{
		const FVector& _feedBackOffset = _feedbackOffsetFX;
		FVector _offset (_requestingActor->GetActorForwardVector() * _feedBackOffset.X +
						 _requestingActor->GetActorRightVector() * _feedBackOffset.Y +
						 _requestingActor->GetActorUpVector() * _feedBackOffset.Z);

		if (_raycastFXOnGround)
		{
			FHitResult _result;
			FVector _ZOffset(0.f, 0.f, 10000.f);
			
			ROG_ERROR(_layer.Num() < 1, "AM_FeedbackManager::CalculateFXLocation -> You are trying to hit ground with no layer");

			const FVector& _requestLocation = _requestingActor->GetActorLocation();
			const FVector& _rayCastLocation = _requestLocation + _offset;

			if (Get_LineTrace(world, _rayCastLocation + _ZOffset, _rayCastLocation - _ZOffset, _layer, _result))
			{
				_offset.Z = _result.Location.Z - _requestLocation.Z + _feedBackOffset.Z;
			}
			else
				LOG_ERROR("AM_FeedbackManager::CalculateFXLocation -> Probably wrong arena layer type");
		}
		
		if (_delayFX > 0.f)
		{
			TIMED_DELEGATE((*timerManager), "PlayFX", _delayFX, _fxFeedback, _requestingActor->GetRootComponent(), _offset, FRotator::ZeroRotator, _attachFX, _animFX, _isGPEFX);
		}
		else
			PlayFX(_fxFeedback,_requestingActor->GetRootComponent(), _offset,FRotator::ZeroRotator, _attachFX, _animFX, _isGPEFX);
	}
}

void AM_FeedbackManager::RequestPlaySong(const FFeedBackData& _data, const AActor* _requestingActor)
{
	PlaySound(_data.soundFeedbackData, _requestingActor, _data.soundFeedbackData.fActivationDelay);
}

void AM_FeedbackManager::RequestPlaySpawnerFeedback(const FSpawnerFeedBack& _data, const AActor* _requestingActor)
{
	CalculateFXLocation(_data.feedbackOffsetFX, _data.bRaycastFXOnGround, _data.hitGroundLayer, _data.fDelayFX, _data.fxFeedBack, _data.attachFX, _requestingActor);
	PlaySound(_data.soundFeedbackData, _requestingActor, _data.soundFeedbackData.fActivationDelay);
}

void AM_FeedbackManager::RequestPlayFX(const FFeedBackData& _data, const AActor* _requestingActor, bool _isGPEFX)
{
	CalculateFXLocation(_data.feedbackOffsetFX, _data.bRaycastFXOnGround, _data.hitGroundLayer, _data.fDelayFX, _data.fxFeedBack,
					    _data.attachFX, _requestingActor, _data.bLinkFXToAnimation, _isGPEFX);
}

void AM_FeedbackManager::PlayFeedbacks(const FFeedBackData& _data, const AActor* _requestingActor, const EAnimState& _animState)
{
	PlaySound(_data.soundFeedbackData, _requestingActor, _data.soundFeedbackData.fActivationDelay);
	RequestPlayFX(_data, _requestingActor);
	PlayAnim(_data.animationFeedback, _animState);
}

void AM_FeedbackManager::RequestPlayTripleFeedbacks(const FTripleDeluxeFeedBack& _data, const AActor* _requestingActor)
{
	if(!_data.startFeedBack.animationFeedback)return;

	requestingActor = _requestingActor;
	bWaitEndOfPattern = _data.bWaitEndOfPattern;

#if WITH_LOG_ANIM_FEEDBACK
	LOG_ERROR(" ");
	LOG_ERROR("Start Anim : %s", *_data.startFeedBack.animationFeedback.GetName());
	LOG_ERROR("Wait end of pattern %s", bWaitEndOfPattern ? *FString("true") : *FString("false"))
#endif

	timerManager->ClearTimer(loopFeedBackTimer);
	timerManager->ClearTimer(endFeedBackTimer);
	
	PlayFeedbacks(_data.startFeedBack, _requestingActor, AnimState_Start);

	Event_AddUnique(bossPatternCaster->OnDataTableEnd(), AM_FeedbackManager::InstantFinishPattern);
	
	if (bWaitEndOfPattern)
		SubToStart();
	else
		timerManager->SetTimer(subAnimTimer, this, &AM_FeedbackManager::SubToStart, fSubAnimDelay);
	
	if (!_data.loopFeedBack.animationFeedback)
	{
		loopFeedbackRef = nullptr;
	}
	else
	{
		loopFeedback = _data.loopFeedBack;
		loopFeedbackRef = &loopFeedback;
	}
	
	if (!_data.endFeedBack.animationFeedback)
	{
		endFeedbackRef = nullptr;
	}
	else
	{
		endFeedback = _data.endFeedBack;
		endFeedbackRef = &endFeedback;
	}
}

void AM_FeedbackManager::InstantFinishPattern(const UP_DT_BaseDataTable* _dataTable)
{
#if WITH_LOG_ANIM_FEEDBACK
	LOG_ERROR("Feedback -> INSTANT finish");
#endif
	loopFeedbackRef = nullptr;
}

void AM_FeedbackManager::StartAnimEnd(const TEnumAsByte<EAnimState>& _animState)
{
	ROG_ERROR(_animState != AnimState_Start, "AM_FeedbackManager::StartAnimEnd -> Probably wrong animation or AnimState notify");
	Event_Remove(bossAnimInstance->OnStartAnimNotifyStateEnd(), AM_FeedbackManager::StartAnimEnd);
	Event_Remove(bossPatternCaster->OnDataTableEnd(), AM_FeedbackManager::SetStartAnimEndWillDirectFinish);
	Event_Remove(bossPatternCaster->OnDataTableEnd(), AM_FeedbackManager::InstantFinishPattern);
	
	if (loopFeedbackRef)
	{
#if WITH_LOG_ANIM_FEEDBACK
		LOG_ERROR("loop ref");
#endif
		
		const float& _delay = loopFeedback.fBehaviorActivationDelay;
		if (_delay > 0.f)
		{
			FTimerDelegate _delegate;
			_delegate.BindUFunction(this, "PlayFeedbacks", loopFeedback, requestingActor, AnimState_Loop);
			timerManager->SetTimer(loopFeedBackTimer, _delegate, _delay, false);
		}
		else
			PlayFeedbacks(loopFeedback, requestingActor, AnimState_Loop);
		Event_AddUnique(bossPatternCaster->OnDataTableEnd(), AM_FeedbackManager::LoopAnimEnd);
	}
	
	else if (endFeedbackRef)
	{
#if WITH_LOG_ANIM_FEEDBACK
		LOG_ERROR("end ref");
#endif
		
		const float& _delay = endFeedback.fBehaviorActivationDelay;
		if (_delay > 0.f)
		{
			FTimerDelegate _delegate;
			_delegate.BindUFunction(this, "PlayFeedbacks", endFeedback, requestingActor, AnimState_End);
			timerManager->SetTimer(endFeedBackTimer, _delegate, _delay, false);
		}
		else
			PlayFeedbacks(endFeedback, requestingActor, AnimState_End);
		Event_AddUnique(bossAnimInstance->OnEndAnimNotifyStateEnd(), AM_FeedbackManager::EndAnimEnd);
	}
	
	else
	{
#if WITH_LOG_ANIM_FEEDBACK
		LOG_ERROR("no ref");
#endif
		PlayAnim(nullptr, AnimState_None);
	}
}

void AM_FeedbackManager::SetStartAnimEndWillDirectFinish(const UP_DT_BaseDataTable* _dataTable)
{
	Event_Remove(bossAnimInstance->OnStartAnimNotifyStateEnd(), AM_FeedbackManager::StartAnimEnd);
	Event_Remove(bossPatternCaster->OnDataTableEnd(), AM_FeedbackManager::SetStartAnimEndWillDirectFinish);
	Event_AddUnique(bossAnimInstance->OnStartAnimNotifyStateEnd(), AM_FeedbackManager::DirectFinishingStartAnimEnd);
}

void AM_FeedbackManager::DirectFinishingStartAnimEnd(const TEnumAsByte<EAnimState>& _animState)
{
	ROG_ERROR(_animState != AnimState_Start, "AM_FeedbackManager::DirectFinishingStartAnimEnd -> Probably wrong animation or AnimState notify");
	Event_Remove(bossAnimInstance->OnStartAnimNotifyStateEnd(), AM_FeedbackManager::DirectFinishingStartAnimEnd);
	Event_Remove(bossPatternCaster->OnDataTableEnd(), AM_FeedbackManager::InstantFinishPattern);
	
	if (endFeedbackRef)
	{
#if WITH_LOG_ANIM_FEEDBACK
		LOG_ERROR("end ref Direct");
#endif
		
		const float& _delay = endFeedback.fBehaviorActivationDelay;
		if (_delay > 0.f)
		{
			FTimerDelegate _delegate;
			_delegate.BindUFunction(this, "PlayFeedbacks", endFeedback, requestingActor, AnimState_End);
			timerManager->SetTimer(endFeedBackTimer, _delegate, _delay, false);
		}
		else
			PlayFeedbacks(endFeedback, requestingActor, AnimState_End);
		Event_AddUnique(bossAnimInstance->OnEndAnimNotifyStateEnd(), AM_FeedbackManager::EndAnimEnd);
	}	
	else
	{
#if WITH_LOG_ANIM_FEEDBACK
		LOG_ERROR("no ref Direct");
#endif
		PlayAnim(nullptr, AnimState_None);
	}
}

void AM_FeedbackManager::LoopAnimEnd(const UP_DT_BaseDataTable* _dataTable)
{
	Event_Remove(bossPatternCaster->OnDataTableEnd(), AM_FeedbackManager::LoopAnimEnd);
	
	if (endFeedbackRef)
	{
#if WITH_LOG_ANIM_FEEDBACK
		LOG_ERROR("end ref 2");
#endif
		
		const float& _delay = endFeedback.fBehaviorActivationDelay;
		if (_delay > 0.f)
		{
			FTimerDelegate _delegate;
			_delegate.BindUFunction(this, "PlayFeedbacks", endFeedback, requestingActor, AnimState_End);
			timerManager->SetTimer(endFeedBackTimer, _delegate, _delay, false);
		}
		else
			PlayFeedbacks(endFeedback, requestingActor, AnimState_End);

		if (bWaitEndOfPattern)
			SubToEnd();
		else
			timerManager->SetTimer(subAnimTimer, this, &AM_FeedbackManager::SubToEnd, fSubAnimDelay);
	}
	else
	{
#if WITH_LOG_ANIM_FEEDBACK
		LOG_ERROR("no ref 2");
#endif
		PlayAnim(nullptr, AnimState_None);
	}
}

void AM_FeedbackManager::EndAnimEnd(const TEnumAsByte<EAnimState>& _animState)
{
	ROG_ERROR(_animState != AnimState_End, "AM_FeedbackManager::EndAnimEnd -> Probably wrong animation or AnimState notify");
	Event_Remove(bossAnimInstance->OnEndAnimNotifyStateEnd(), AM_FeedbackManager::EndAnimEnd);

#if WITH_LOG_ANIM_FEEDBACK
	LOG_ERROR("no ref 3");
#endif
	PlayAnim(nullptr, AnimState_None);
}

void AM_FeedbackManager::SubToStart()
{
#if WITH_LOG_ANIM_FEEDBACK
	LOG_ERROR("Sub to start");
#endif
	Event_AddUnique(bossAnimInstance->OnStartAnimNotifyStateEnd(), AM_FeedbackManager::StartAnimEnd);
	Event_AddUnique(bossPatternCaster->OnDataTableEnd(), AM_FeedbackManager::SetStartAnimEndWillDirectFinish);
	
	Event_Remove(bossPatternCaster->OnDataTableEnd(), AM_FeedbackManager::InstantFinishPattern);
}

void AM_FeedbackManager::SubToEnd()
{
#if WITH_LOG_ANIM_FEEDBACK
	LOG_ERROR("Sub to end");
#endif
	Event_AddUnique(bossAnimInstance->OnEndAnimNotifyStateEnd(), AM_FeedbackManager::EndAnimEnd);
}

void AM_FeedbackManager::RequestPlayGPEShotFeedBack(const FTripleDeluxeFeedBack& _feedBack, const AActor* _requestingActor)
{
	const FFeedBackData& _startFeedbackData = _feedBack.startFeedBack;
	if (const UAnimSequenceBase* _anim = _startFeedbackData.animationFeedback)
	{
		const float& _animLenght = _anim->GetPlayLength();
		PlaySound(_startFeedbackData.soundFeedbackData, _requestingActor, _startFeedbackData.soundFeedbackData.fActivationDelay);
		RequestPlayFX(_startFeedbackData, _requestingActor, true);

		TIMED_DELEGATE((*timerManager), "PlayGPEShotEndFeedBack", _animLenght, _requestingActor);
	}
}

void AM_FeedbackManager::PlayGPEShotEndFeedBack(const AActor* _requestingActor)
{
	const size_t& _max = allGPEShotFeedBack.Num();
	for (size_t i = 0; i < _max; ++i)
	{
		const FFeedBackActor& _feedBackActor = allGPEShotFeedBack[i];
		if (_requestingActor == _feedBackActor.requestingActor)
		{
			const FFeedBackData& _endFeedBack = _feedBackActor.feedBack;
			PlaySound(_endFeedBack.soundFeedbackData, _requestingActor, _endFeedBack.soundFeedbackData.fActivationDelay);
			RequestPlayFX(_endFeedBack, _requestingActor, true);
			
			allGPEShotFeedBack.RemoveAt(i);
			return;
		}
	}
}