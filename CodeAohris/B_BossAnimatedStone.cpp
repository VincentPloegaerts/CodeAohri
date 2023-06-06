#include "B_BossAnimatedStone.h"

#include "B_BossAnimatedStoneAnim.h"
#include "M_FeedbackManager.h"
#include "P_DT_BaseDataTable.h"

#define CHECK_ERROR(_functionName) \
	ROG_ERROR(!datatable, "AB_BossAnimatedStone::%s -> Invalid datatable", *FString(_functionName)); \
	ROG_ERROR(!animInstance, "AB_BossAnimatedStone::%s -> Invalid anim instance", *FString(_functionName));

AB_BossAnimatedStone::AB_BossAnimatedStone()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	skeletalMesh->SetupAttachment(RootComponent);
}

void AB_BossAnimatedStone::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void AB_BossAnimatedStone::Init()
{
	animInstance = Cast<UB_BossAnimatedStoneAnim>(skeletalMesh->GetAnimInstance());

	ROG_ERROR(!animInstance, "AB_BossAnimatedStone::Init -> Invalid anim instance");
}

void AB_BossAnimatedStone::PlayStartAnimation(AM_FeedbackManager* _feedbackManager)
{
	CHECK_ERROR("PlayStartAnimation");

	feedBackManager = _feedbackManager;
	
	Event_AddUnique(animInstance->OnStartAnimEnd(), AB_BossAnimatedStone::PlayLoopAnimation);

	const FFeedBackData& _feedBack = datatable->GetTripleDeluxeFeedBack().startStoneFeedback;
	UAnimSequenceBase* _anim = _feedBack.animationFeedback;
	
	if (!_anim)
	{
		BroadcastEndAnimation();
		LOG_ERROR("AB_BossAnimatedStone::PlayStartAnimation -> You activate boss stone anim without setting start animation");
		return;
	}
	
	const float& _delay = _feedBack.fBehaviorActivationDelay;
	if (_delay > 0.f)
	{
		FTimerDelegate _delegate;
		_delegate.BindUFunction(this, "PlayFeedBack", AnimState_Start, _anim, _feedBack);
		GetWorld()->GetTimerManager().SetTimer(feedbackTimer, _delegate, _delay, false);
	}
	else
		PlayFeedBack(AnimState_Start, _anim, _feedBack);
}

void AB_BossAnimatedStone::PlayLoopAnimation()
{
	CHECK_ERROR("PlayLoopAnimation");

	Event_Remove(animInstance->OnStartAnimEnd(), AB_BossAnimatedStone::PlayLoopAnimation);

	const FFeedBackData& _feedBack = datatable->GetTripleDeluxeFeedBack().loopStoneFeedback;
	UAnimSequenceBase* _anim = _feedBack.animationFeedback;

	if (!_anim)
	{
		BroadcastEndAnimation();
		return;
	}
	
	const float& _delay = _feedBack.fBehaviorActivationDelay;
	if (_delay > 0.f)
	{
		FTimerDelegate _delegate;
		_delegate.BindUFunction(this, "PlayFeedBack", AnimState_Loop, _anim, _feedBack);
		GetWorld()->GetTimerManager().SetTimer(feedbackTimer, _delegate, _delay, false);
	}
	else
		PlayFeedBack(AnimState_Loop, _anim, _feedBack);
}

void AB_BossAnimatedStone::PlayEndAnimation()
{
	CHECK_ERROR("PlayEndAnimation");

	Event_Remove(animInstance->OnStartAnimEnd(), AB_BossAnimatedStone::PlayLoopAnimation);

	if (!IsValid(this))
	{
		BroadcastEndAnimation();
		return;
	}
	
	Event_AddUnique(animInstance->OnEndAnimEnd(), AB_BossAnimatedStone::BroadcastEndAnimation);

	const FFeedBackData& _feedBack = datatable->GetTripleDeluxeFeedBack().endStoneFeedback;
	UAnimSequenceBase* _anim = _feedBack.animationFeedback;

	if (!_anim)
	{
		BroadcastEndAnimation();
		return;
	}
	
	const float& _delay = _feedBack.fBehaviorActivationDelay;
	if (_delay > 0.f)
	{
		FTimerDelegate _delegate;
		_delegate.BindUFunction(this, "PlayFeedBack", AnimState_End, _anim, _feedBack);
		GetWorld()->GetTimerManager().SetTimer(feedbackTimer, _delegate, _delay, false);
	}
	else
		PlayFeedBack(AnimState_End, _anim, _feedBack);
}

void AB_BossAnimatedStone::PlayFeedBack(const EAnimState& _animState, UAnimSequenceBase* _anim, const FFeedBackData& _feedBack) const
{
	animInstance->SetAnimState(_animState, _anim);
	feedBackManager->RequestPlayFX(_feedBack, this);
	feedBackManager->RequestPlaySong(_feedBack, this);
}

void AB_BossAnimatedStone::BroadcastEndAnimation()
{
	Event_Remove(animInstance->OnStartAnimEnd(), AB_BossAnimatedStone::PlayLoopAnimation);
	Event_Remove(animInstance->OnEndAnimEnd(), AB_BossAnimatedStone::BroadcastEndAnimation);
	onEndAnimation.Broadcast(this);
}