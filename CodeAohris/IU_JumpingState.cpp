#include "IU_JumpingState.h"

#include "IU_Macros.h"
#include "IU_PlayerFSMBrain.h"
#include "Components/CapsuleComponent.h"

/// FSM base methods ///

void UIU_JumpingState::Init(UIU_PlayerFSMBrain* _brain)
{
	Super::Init(_brain);
	stateType = Jumping;
	
	if (brain && brain->PlayerOwner() &&  brain->PlayerOwner()->InputComponent)
	{
		brain->PlayerOwner()->InputComponent->BindAction(jumpData.jumpInputName, IE_Pressed, this, &UIU_JumpingState::Jump);
	}
}

void UIU_JumpingState::Enter()
{
	Super::Enter();
	playerMovementComponent->OnJumpEvent().Broadcast(true);
	GetWorld()->GetTimerManager().SetTimer(handle, this, &UIU_JumpingState::JumpFinished, jumpData.ascentDuration, false, -1);
	if (!brain || !brain->PlayerOwner() || !playerMovementComponent) return;
	const FVector _momentum = playerMovementComponent->Momentum();
	playerMovementComponent->SetMomentum(FVector(_momentum.X,_momentum.Y,jumpData.jumpVerticalPower));
	playerMovementComponent->MovePlayerWithMomentum();
	playerMovementComponent->OnJump().Broadcast();
}

void UIU_JumpingState::Exit()
{
	Super::Exit();
	GetWorld()->GetTimerManager().ClearTimer(handle);
	playerMovementComponent->OnJumpEvent().Broadcast(false);
}

void UIU_JumpingState::Update(const float _deltaTime)
{
	if (!playerMovementComponent || !playerMovementComponent->CanMove() || !playerMovementComponent->UpdatedComponent || playerMovementComponent->ShouldSkipUpdate(_deltaTime) || !brain || !brain->PlayerOwner())
	{
		return;
	}
	
	if (playerMovementComponent->Momentum().Z > 0.0f)
	{
		if (UCapsuleComponent* _capsule =  brain->PlayerOwner()->CapsuleComponent())
		{
			const FVector& _start = _capsule->GetComponentLocation();
			const ECollisionChannel& _traceChannel = _capsule->GetCollisionObjectType();
			const float& _radius = _capsule->GetScaledCapsuleRadius();
			float _pawnRadius = 0.0f, _pawnHalfHeight = 0.0f;
			_capsule->GetScaledCapsuleSize(_pawnRadius, _pawnHalfHeight);
			float _shrinkHeight = (_pawnHalfHeight - _pawnRadius) * 0.1f;
			const  FCollisionShape& _capsuleShape = FCollisionShape::MakeCapsule(_radius, _pawnHalfHeight - _shrinkHeight);
	
			const  FCollisionQueryParams _params;
			const  FCollisionResponseParams _responseParams;
			bool _blockingHit = false;	
	
			// Test with a box that is enclosed by the capsule.
			const float& _capsuleRadius = _capsuleShape.GetCapsuleRadius();
			const float& _capsuleHeight = _capsuleShape.GetCapsuleHalfHeight();
			const FCollisionShape& _boxShape = FCollisionShape::MakeBox(FVector(_capsuleRadius * 0.5f, _capsuleRadius * 0.5f, _capsuleHeight));

				
			const FVector& _end = _start + FVector(0.0f,0.0f,_capsuleHeight / 2.0f);
			FHitResult _outHit;
			// First test with the box rotated so the corners are along the major axes (ie rotated 45 degrees).
			_blockingHit = GetWorld()->SweepSingleByChannel(_outHit, _start, _end, FQuat(FVector(0.f, 0.f, -1.f), PI * 0.25f), _traceChannel, _boxShape, _params, _responseParams);
			if (_blockingHit)
			{
				FVector _newMomentum = playerMovementComponent->Momentum();
				_newMomentum.Z = 0.0f;
				playerMovementComponent->SetMomentum(_newMomentum);					
			}
		}
	}
	const FVector& _movement = playerMovementComponent->LastInputsDirection().GetClampedToMaxSize(1.0f) * _deltaTime * fAirControl + FVector::DownVector * fGravityScale * _deltaTime;
	playerMovementComponent->IncreaseMomentum(_movement);
	playerMovementComponent->MovePlayerWithMomentum();
}

bool UIU_JumpingState::CanEnterState()
{
	if (brain)
	{
		switch (brain->CurrentStateType())
		{
		case Moving:
			if (brain->IsSwitchingState())
			{
				if (brain->NextState()->StateType() != Dashing)
					brain->SetBufferJump(true);
				return false;
			}			
			return true;
			
		case Falling:
			if (brain && brain->FallingState() && brain->FallingState()->CanCoyoteJump())
			{
				if (brain->IsSwitchingState())
				{
					brain->SetBufferJump(true);
					return false;
				}			
				return true;
			}
			return false;
			
		default:
			return false;
		}
	}
	
	return false;
}

void UIU_JumpingState::PassData(const FMovementData& _data)
{
	Super::PassData(_data);
	jumpData = _data.jumpData;
	fGravityScale = _data.fallData.gravityScale;
	fAirControl = _data.fallData.airControl;
}

void UIU_JumpingState::Jump()
{
	if (!CanEnterState() || !bCanJump) return;
	brain->SwitchCurrentState(this);
}

void UIU_JumpingState::JumpFinished()
{
	brain->SwitchCurrentState(Falling);
}