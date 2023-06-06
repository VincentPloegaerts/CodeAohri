#include "IU_DashingState.h"

#include "ENUM_PhaseTuto.h"
#include "IU_HealthSystemComponent.h"
#include "IU_PlayerFSMBrain.h"
#include "M_GameMode.h"
#include "T_TutoLogic.h"
#include "UC_PlayerAttackComponent.h"
#include "Components/CapsuleComponent.h"

/// FSM base methods ///

void UIU_DashingState::Init(UIU_PlayerFSMBrain* _brain)
{
	Super::Init(_brain);
	stateType = Dashing;
	
	if (brain && brain->PlayerOwner())
	{
		brain->PlayerOwner()->InputComponent->BindAction(dashData.dashInputName, IE_Pressed, this, &UIU_DashingState::Dash);
	}
	
	if (playerMovementComponent)
	{
		playerMovementComponent->OnHitGround().AddDynamic(this, &UIU_DashingState::ResetDash);
	}
}

void UIU_DashingState::Enter()
{
	Super::Enter();

	GetWorld()->GetTimerManager().SetTimer(timerCooldown, this, &UIU_DashingState::DashCooldown, dashData.dashCooldown, false, -1);
	GetWorld()->GetTimerManager().SetTimer(dashTimer, this, &UIU_DashingState::EndDash, dashData.dashDuration, false, -1);
	playerMovementComponent->OnDashed().Broadcast();
	
	if (brain && brain->PlayerOwner() && brain->PlayerOwner()->HealthComponent())
	{
		brain->PlayerOwner()->HealthComponent()->SetCanTakeDamages(false);
		brain->PlayerOwner()->MovementComponent()->OnDashEvent().Broadcast(true);

		brain->PlayerOwner()->MovementComponent()->SetHasDashed(true);
		
		if(brain->PlayerOwner()->GetCharacterAttackComponent()->IsPlayerChargingShoot())
		{
			brain->PlayerOwner()->GetCharacterAttackComponent()->SetCurrentShootingState();

			
			if(	brain->PlayerOwner()->GetCharacterAttackComponent()->GetIsInTuto())
			{
			
				AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
				if(!_gm)return;
				
				AM_GameLogic* _gl = _gm->GetGameLogic();
				if(!_gl)return;
			
				if(_gl->GetTutoLogic()->GetCurrentPhase() == DashCancelShoot)
				{
					brain->PlayerOwner()->GetCharacterAttackComponent()->IncreaseAndCheckTutoCondition();
					_gl->GetTutoLogic()->GetCurrentCountElementWidget()->UpdateCurrentCount(brain->PlayerOwner()->GetCharacterAttackComponent()->GetCurrentTutoIterationNumber());
				}
			}
		}
		
		brain->PlayerOwner()->GetCharacterAttackComponent()->ResetShoot();

	}
	
	brain->PlayerOwner()->CapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6,ECollisionResponse::ECR_Ignore);
	
	if (!playerMovementComponent) return;
	dashDirection = playerMovementComponent->LastInputsDirection();
	if (dashDirection == FVector::ZeroVector)
	{
		if (const AIU_Camera* _camera =  playerMovementComponent->GetPlayerCamera())
		{
			dashDirection = _camera->GetActorForwardVector();
			dashDirection.Z = 0.0f;			
		}
	}
	
	dashDirection.Normalize();
	if (brain->CurrentStateType() == Moving)
	{		
		bIsGrounded = true;
	}
	else
	{
		bIsGrounded = false;
	}
	
	bDashIsInCooldown = true;
}

void UIU_DashingState::Update(const float _deltaTime)
{

	if (playerMovementComponent)
	{
		playerMovementComponent->MovePlayer(dashDirection * dashData.dashPower );
	}
}

void UIU_DashingState::Exit()
{
	Super::Exit();
	brain->PlayerOwner()->CapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6,ECollisionResponse::ECR_Overlap);
	brain->PlayerOwner()->MovementComponent()->OnDashEvent().Broadcast(false);
	playerMovementComponent->OnDashEnded().Broadcast();
}

bool UIU_DashingState::CanEnterState()
{
	if (brain && bCanDash && bIsGrounded && !brain->PlayerOwner()->HealthComponent()->IsDead() && !bDashIsInCooldown && !brain->PlayerOwner()->MovementComponent()->GetIsSliding())
	{
		switch (brain->CurrentStateType())
		{
		case Moving:
			return !brain->IsSwitchingState();
			
		case Falling:
			if (brain->IsSwitchingState())
			{
				brain->SetBufferDash(true);
				return false;
			}			
			return true;
			
		case Jumping:
			if (brain->IsSwitchingState())
			{
				brain->SetBufferDash(true);
				return false;
			}
			return true;
		default:
			return false;
		}
	}
	
	return false;
}

void UIU_DashingState::PassData(const FMovementData& _data)
{
	Super::PassData(_data);
	dashData = _data.dashData;
	fAirControl = _data.fallData.airControl;
}
 
void UIU_DashingState::Dash()
{
	if (!CanEnterState()) return;
	
	brain->SwitchCurrentState(this);
}

void UIU_DashingState::EndDash()
{

	if (brain)
	{
		brain->SwitchCurrentState(Falling);
		// if (brain->PlayerOwner() && brain->PlayerOwner()->HealthComponent())
		// {
		// 	brain->PlayerOwner()->HealthComponent()->SetCanTakeDamages(true);
		// 	
		// }
		FVector _momentum = playerMovementComponent->LastInputsDirection().GetClampedToMaxSize(1.0f) * fAirControl;
		if ( playerMovementComponent->LastInputsDirection().IsNearlyZero())
		{
			_momentum = dashDirection * dashData.dashPower * GetWorld()->DeltaTimeSeconds / 7.0f;
		}			
		playerMovementComponent->SetMomentum(_momentum);

	}
	
	//playerMovementComponent->OnDashEvent().Broadcast(false);
}

void UIU_DashingState::DashCooldown()
{	
	bDashIsInCooldown = false;
}

void UIU_DashingState::ResetDash()
{
	bIsGrounded = true;
}