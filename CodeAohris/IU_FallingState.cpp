#include "IU_FallingState.h"

#include "C_RespawnComponent.h"
#include "IU_PlayerFSMBrain.h"

/// FSM base methods ///

void UIU_FallingState::Init(UIU_PlayerFSMBrain* _brain)
{
	Super::Init(_brain);
	stateType = Falling;
	player = _brain->PlayerOwner();
}

void UIU_FallingState::Update(const float _deltaTime)
{
	Super::Update(_deltaTime);

	fallAnimationTimer += GetWorld()->DeltaTimeSeconds;
	if(fallAnimationTimer > timeBeforePlayerIsConsideredInFall)
	{
		if(playerMovementComponent)
		{
			playerMovementComponent->OnFallingEvent().Broadcast(true);
		
			
		}
	}
		
	
	if (playerMovementComponent && brain && playerMovementComponent->CheckIsGrounded(groundResult))
	{
			
		
		if (playerMovementComponent->CheckAcceptableSlope(groundResult))
		{
			playerMovementComponent->OnHitGround().Broadcast();
			playerMovementComponent->OnHitGroundWithResult().Broadcast(groundResult);
			playerMovementComponent->OnHittingGroundAfterFreeFall().Broadcast(true);

			if(fallAnimationTimer > timeBeforePlayerIsConsideredInFall)
			{
				if(playerMovementComponent)
				{
					playerMovementComponent->SetCanMove(false);
					GetWorld()->GetTimerManager().SetTimer(SetCanMoveTimer,this,&UIU_FallingState::SetCanMoveAgain,durationOfStopMouvementAfterFalling);
				}
			}
			
			GetWorld()->GetTimerManager().SetTimer(freeFallTimer,this,&UIU_FallingState::ResetHittingGroundAfterFreeFall,0.1);
			brain->SwitchCurrentState(EStateType::Moving);
			return;
		}
	}
		
	AirControlPlayer(_deltaTime);
	UpdateStuckTime(_deltaTime);
}

bool UIU_FallingState::CanEnterState()
{
	if (brain && !brain->IsSwitchingState()) 
	{
		switch (brain->CurrentStateType())
		{
		case Moving:
			return true;
			
		case Dashing:
			return true;
			
		case Jumping:
			return true;
			
		default:
			return false;
		}
	}
	
	return false;
}

void UIU_FallingState::PassData(const FMovementData& _data)
{
	Super::PassData(_data);
	fallData = _data.fallData;
	groundFriction = _data.movingOnGroundData.groundFriction;
}


/// State methods ///

void UIU_FallingState::Enter()
{
	Super::Enter();
	
	if (brain)
	{
		firstPosition = brain->PlayerOwner()->GetActorLocation();
		if (brain->CurrentStateType() == EStateType::Moving)
		{
			canCoyoteJump = true;
			playerMovementComponent->OnLeaveGround().Broadcast();
			GetWorld()->GetTimerManager().SetTimer(coyoteTimer,this,&UIU_FallingState::ResetCoyote,fallData.coyoteTime);
		}
		
		else if (brain->CurrentStateType() == EStateType::Dashing)
		{
			amplifyingAirControl = true;
			GetWorld()->GetTimerManager().SetTimer(coyoteTimer,this,&UIU_FallingState::ResetAirControl,fallData.amplificationTime);
		}
		
		// brain->PlayerOwner()->SetSimulatePhysics(true);
	}
}

void UIU_FallingState::Exit()
{
	Super::Exit();
	
	canCoyoteJump = false;
	progressAntiStuck = 0.0f;
	unstuckOnce = false;
	fallAnimationTimer = 0.0f;
	
	if (playerMovementComponent)
	{
		playerMovementComponent->OnFallingEvent().Broadcast(false);
		//playerMovementComponent->OnHittingGroundAfterFreeFall().Broadcast(false);
	}
}

/// Custom methods ///

void UIU_FallingState::AirControlPlayer(const float _deltaTime) const
{
	if (!playerMovementComponent || !playerMovementComponent->CanMove() || !playerMovementComponent->UpdatedComponent || playerMovementComponent->ShouldSkipUpdate(_deltaTime) || !brain || !brain->PlayerOwner())
	{
		return;
	}
	FVector _movement = playerMovementComponent->LastInputsDirection().GetClampedToMaxSize(1.0f) * _deltaTime * fallData.airControl * (amplifyingAirControl ? fallData.airControlAmplifier : 1.0f)  + FVector::DownVector * fallData.gravityScale * fallData.gravityMultiplier * _deltaTime;
	//to remove the bouncing effect after slopes that can't be climbed
	if (groundResult.bBlockingHit)
	{
		_movement = _movement - _movement.ProjectOnToNormal(groundResult.ImpactNormal);

		//slope friction avoid player going too easily up the slope with jump + dash
		// if (playerMovementComponent->Momentum().Z >0)
		// 	_movement*=3;
	}
	
	playerMovementComponent->IncreaseMomentum(_movement);
	playerMovementComponent->MovePlayerWithMomentum();
}

void UIU_FallingState::ResetCoyote()
{
	canCoyoteJump = false;
}

void UIU_FallingState::ResetAirControl()
{
	amplifyingAirControl = false;
}

void UIU_FallingState::ResetHittingGroundAfterFreeFall()
{
	
	if (playerMovementComponent)
	{
		playerMovementComponent->OnHittingGroundAfterFreeFall().Broadcast(false);
	}
}

void UIU_FallingState::SetCanMoveAgain()
{
	if(playerMovementComponent)
		playerMovementComponent->SetCanMove(true);
}

void UIU_FallingState::UpdateStuckTime(const float& _deltaTime)
{
	const float _zPos = player->GetActorLocation().Z;
	if (FMath::Abs(_zPos - zFallPosition) > 0.1f)
	{
		zFallPosition = _zPos;
		progressAntiStuck = 0.0f;
		return;
	}
	progressAntiStuck += _deltaTime;
	if (progressAntiStuck >= fallData.antiStuckTime)
	{
		if (unstuckOnce)
		{
			brain->PlayerOwner()->RespawnComponent()->Death(false);
		}
		
		else
		{
			FVector _dir = firstPosition - brain->PlayerOwner()->GetActorLocation();
			_dir.Normalize();
			_dir *= playerMovementComponent->MaxVelocity();
            _dir.Z = playerMovementComponent->MaxVelocity();
            playerMovementComponent->SetMomentum(_dir);
			unstuckOnce = true;
		}
		progressAntiStuck = 0.0f;
	}
}
