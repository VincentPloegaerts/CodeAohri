#include "B_BossAnim.h"

#include "B_Boss.h"
#include "B_BossMovement.h"
#include "IU_Macros.h"
#include "M_GameMode.h"
#include "P_PatternCaster.h"
#include "M_BlackBoardManager.h"

void UB_BossAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	boss = static_cast<AB_Boss*>(GetOwningActor());
	Init();
}

void UB_BossAnim::Init()
{
	ROG_ERROR(!boss, "UB_BossAnim::Init -> invalid boss");

	bossMovement = boss->GetBossMovement();
	ROG_ERROR(!bossMovement, "UB_BossAnim::Init -> invalid boss Movement");
	
	const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>();
	ROG_ERROR(!_gameMode, "UB_BossAnim::Init -> Invalid gamemode");
	blackBoardManager = _gameMode->GetBlackBoardManager();
	ROG_ERROR(!blackBoardManager, "UB_BossAnim::Init ->  Invalid black board manager");

	Event_AddUnique(boss->OnBossActivated(), UB_BossAnim::InitAnim);
	Event_AddUnique(boss->OnBossReset(), UB_BossAnim::ResetAnim);
	Event_AddUnique(bossMovement->OnPlayMovementAnimState(), UB_BossAnim::PlayMovement);

	Event_AddUnique(onAnimMoveEnd, UB_BossAnim::NotifyStopMovement);
	Event_AddUnique(onAnimSpecialEnd, UB_BossAnim::NotifyStopSpecialAnim);
	Event_AddUnique(onAnimationEnd, UB_BossAnim::Broadcast_BlackBoard);
	
	resetIdleAnim = currentIdleAnim;

#if WITH_LOG_MOVEMENT_BOSS && WITH_LOG_DEBUG
	Event_Add(onAnimationEnd, UB_BossAnim::DEBUG_AnimFINISHED);
#endif
}

void UB_BossAnim::InitAnim()
{
	blackBoardManager->Set_OnReachGoal_BossAnim(this);
}

void UB_BossAnim::ResetAnim()
{
	bIsAnnouncing = false;
	bIsMoving = false;
	bIsDoingSpecialAnim = false;
	animState = AnimState_None;
	bossMovementAnimState = EBossMovementAnimState::NONE_;
	bossSpecialAnimState = EBossSpecialAnimState::NONE__;
	currentAnnounce = nullptr;
	currentIdleAnim = resetIdleAnim;
}

void UB_BossAnim::Broadcast_BlackBoard()
{
	blackBoardManager->Broadcast_BossAnim_OnAnimationEnd();
}

void UB_BossAnim::StopPlayAnnounce()
{
	bIsAnnouncing = false;
	currentAnnounce = nullptr;
	animState = AnimState_None;
}

void UB_BossAnim::StopPlayMovement()
{
	bIsMoving = false;
	bossMovementAnimState = EBossMovementAnimState::NONE_;

#if WITH_LOG_ANIM_BOSS
	LOG("Anim : STOP PLAY Movement");
	LOG(" ");
#endif
}

void UB_BossAnim::StopPlaySpecialAnim()
{
	bIsDoingSpecialAnim = false;
	bossSpecialAnimState = EBossSpecialAnimState::NONE__;

#if WITH_LOG_ANIM_BOSS
	LOG("Anim : STOP PLAY Special Anim");
	LOG(" ");
#endif
}

void UB_BossAnim::NotifyStopMovement(const TEnumAsByte<EBossMovementAnimState>& _bossMoveState)
{
#if WITH_LOG_ANIM_BOSS
	if (_bossMoveState)
		LOG("Notify : Movement %s end", *UEnum::GetValueAsString(_bossMoveState));
#endif
	
	if (bossMovementAnimState == _bossMoveState)
	{
		if (bossMovementAnimState != EBossMovementAnimState::MOVE_IN_GROUND_START &&
			bossMovementAnimState != EBossMovementAnimState::DASH_FWD_START &&
			bossMovementAnimState != EBossMovementAnimState::DASH_LEFT_START &&
			bossMovementAnimState != EBossMovementAnimState::DASH_RIGHT_START)
			StopPlayMovement();
		onAnimationEnd.Broadcast();
	}
}

void UB_BossAnim::NotifyStopSpecialAnim(const TEnumAsByte<EBossSpecialAnimState>& _specialAnim)
{
#if WITH_LOG_ANIM_BOSS
	if (_specialAnim)
		LOG("Notify : Special Anim %s end", *UEnum::GetValueAsString(_specialAnim));
#endif

	if (bossSpecialAnimState == _specialAnim)
	{
		StopPlaySpecialAnim();
		onAnimationEnd.Broadcast();
	}
}

void UB_BossAnim::PlayAnnounce(UAnimSequenceBase* _announce, const EAnimState& _animState)
{
	animState = _animState;
	
	if (animState == AnimState_None)
	{
#if WITH_LOG_ANIM_FEEDBACK
		LOG("Animation End");
#endif
		bIsAnnouncing = false;
		currentAnnounce = nullptr;
		onAnimationEnd.Broadcast();
		return;
	}
	
	bIsAnnouncing = true;
	currentAnnounce = _announce;
	
	StopPlayMovement();
	StopPlaySpecialAnim();
}

void UB_BossAnim::PlayMovement(const EBossMovementAnimState& _bossMovementAnimState)
{
	bIsMoving = _bossMovementAnimState != EBossMovementAnimState::NONE_;
	bossMovementAnimState = _bossMovementAnimState;

#if WITH_LOG_ANIM_BOSS
	LOG("Anim : Movement %s", *UEnum::GetValueAsString(bossMovementAnimState));
#endif
	
	StopPlayAnnounce();
	StopPlaySpecialAnim();
}

void UB_BossAnim::PlaySpecialAnim(const EBossSpecialAnimState& _bossSpecialAnimState)
{
	bIsDoingSpecialAnim = _bossSpecialAnimState != EBossSpecialAnimState::NONE__;
	bossSpecialAnimState = _bossSpecialAnimState;

#if WITH_LOG_ANIM_BOSS
	LOG("Anim : Special %s", *UEnum::GetValueAsString(bossSpecialAnimState));
#endif

	StopPlayAnnounce();
	StopPlayMovement();
}

void UB_BossAnim::DEBUG_AnimFINISHED()
{
	LOG("Anim: DEBUG Anim finished");
}