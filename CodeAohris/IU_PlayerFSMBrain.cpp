#include "IU_PlayerFSMBrain.h"

UIU_PlayerFSMBrain::UIU_PlayerFSMBrain()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UIU_PlayerFSMBrain::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void UIU_PlayerFSMBrain::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsSwitchingState())
	{
		if (bufferDash)
		{
			SwitchCurrentState(dashingState);
			bufferDash = false;
			return;
		}
		if (bufferJump)
		{
			SwitchCurrentState(jumpingState);
			bufferJump = false;
		}
		else
			currentState->Update(DeltaTime);
	}
	else
	{
		currentState = nextState;
	}
}

void UIU_PlayerFSMBrain::Init()
{
	playerOwner = GetOwner<AIU_Player>();
	if (movingGroundStateRef)
	{
		movingOnGroundState = NewObject<UIU_MovingOnGroundState>(this, movingGroundStateRef);
		movingOnGroundState->Init(this);
		currentState = movingOnGroundState;
	}

	if (jumpingStateRef)
	{
		jumpingState = NewObject<UIU_JumpingState>(this, jumpingStateRef);
		jumpingState->Init(this);
	}

	if (fallingStateRef)
	{
		fallingState = NewObject<UIU_FallingState>(this, fallingStateRef);
		fallingState->Init(this);
	}

	if (dashingStateRef)
	{
		dashingState = NewObject<UIU_DashingState>(this, dashingStateRef);
		dashingState->Init(this);
	}
	nextState = currentState;
}
#if WITH_EDITOR
void UIU_PlayerFSMBrain::PassALlDatas(const FMovementData& _datas) const
{
	if (movingOnGroundState)
		movingOnGroundState->PassData(_datas);
	if (jumpingState)
		jumpingState->PassData(_datas);
	if (fallingState)
		fallingState->PassData(_datas);
	if (dashingState)
		dashingState->PassData(_datas);
}
#endif
void UIU_PlayerFSMBrain::SwitchCurrentState(TEnumAsByte<EStateType> _newState)
{
	switch (_newState)
	{
	case Dashing:
		SwitchCurrentState(dashingState);
		return;

	case Jumping:
		SwitchCurrentState(jumpingState);
		break;

	case Falling:
		SwitchCurrentState(fallingState);
		break;

	case Moving:
		SwitchCurrentState(movingOnGroundState);
		break;

	default:
		break;
	}
}

void UIU_PlayerFSMBrain::SwitchCurrentState(UIU_State* _newState)
{
	if (!currentState || !_newState) return;
	currentState->Exit();
	_newState->Enter();
	nextState = _newState;
}
