#include "B_BossAnimatedStoneAnim.h"

#include "B_BossAnimatedStone.h"
#include "IU_Macros.h"

void UB_BossAnimatedStoneAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	Init(Cast<AB_BossAnimatedStone>(GetOwningActor()));
}

void UB_BossAnimatedStoneAnim::Init(AB_BossAnimatedStone* _bossStone)
{
	bossStone = _bossStone;
	ROG_ERROR(!bossStone, "UB_BossAnimatedStoneAnim::Init -> invalid boss stone");
}

void UB_BossAnimatedStoneAnim::SetAnimState(const EAnimState& _animState, UAnimSequenceBase* _anim)
{
	animState = _animState;
	currentAnim = _anim;
}