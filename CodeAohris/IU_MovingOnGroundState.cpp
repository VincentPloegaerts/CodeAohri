#include "IU_MovingOnGroundState.h"
#include "IU_PlayerFSMBrain.h"
#include "Components/CapsuleComponent.h"

/// FSM base methods ///

void UIU_MovingOnGroundState::Init(UIU_PlayerFSMBrain* _brain)
{
	Super::Init(_brain);
	stateType = Moving;	
}

void UIU_MovingOnGroundState::Update(const float _deltaTime)
{
	Super::Update(_deltaTime);
	
	if (playerMovementComponent)
	{
		if ((!playerMovementComponent->CheckIsGrounded(groundResult) || !playerMovementComponent->CheckAcceptableSlope(groundResult)) &&
			playerMovementComponent->CanSwitchState())
		{
			brain->SwitchCurrentState(Falling);
			return;
		}
		MovePlayerOnGround(_deltaTime, groundResult);
	}
	
	// UE_LOG(LogTemp, Warning, TEXT("x => %f & y => %f"), _frameInputs.X,_frameInputs.Y);
}

bool UIU_MovingOnGroundState::CanEnterState()
{
	if (brain && !brain->IsSwitchingState())
	{
		switch (brain->CurrentStateType())
		{
		case Falling:
			return true;
		
		case Dashing:
			return true;
			
		default:
			return false;
		}
	}
	
	return false;
}

void UIU_MovingOnGroundState::PassData(const FMovementData& _data)
{
	Super::PassData(_data);
	movingOnGroundStats = _data.movingOnGroundData;
}


/// State methods ///
 
void UIU_MovingOnGroundState::Enter()
{
	Super::Enter();
	if (playerMovementComponent)
	{
	    FVector _momentum = playerMovementComponent->Momentum();
		_momentum.Z =0;
	    playerMovementComponent->SetMomentum(_momentum);
	}

	//not used anymore left in case we turn it back on
	// if (brain && brain->PlayerOwner())
	// {
	// 	brain->PlayerOwner()->SetSimulatePhysics(false);
	// }
}


/// Custom methods ///

void UIU_MovingOnGroundState::MovePlayerOnGround(const float _deltaTime, const FHitResult& _groundResult) const
{
	if (!playerMovementComponent || !playerMovementComponent->CanMove() || !playerMovementComponent->UpdatedComponent
	  || playerMovementComponent->ShouldSkipUpdate(_deltaTime)) return;

	const FVector& _input = playerMovementComponent->LastInputsDirection().GetClampedToMaxSize(1.0f);
	
	FVector _movement = _input * _deltaTime * movingOnGroundStats.moveSpeed;
	
	if (_movement == FVector::ZeroVector)
	{
		FVector _momentum = playerMovementComponent->Momentum();
		_momentum /= movingOnGroundStats.groundFriction;
		playerMovementComponent->SetMomentum(_momentum);
		playerMovementComponent->MovePlayerWithMomentum();
		
		return;
	}
	
	// FVector _cameraForward = playerMovementComponent->GetPlayerCamera()->GetActorForwardVector(); //MIGHT REUSE Old to rotate Player
	// _cameraForward.Z = 0.0f;
	// _cameraForward.Normalize();
	// playerMovementComponent->RotateToDirection(_cameraForward,playerMovementComponent->RotateSpeed());

	//check ground ahead to see if it's going down/up to avoid going into fall when going too fast
	FHitResult _outHit;
	UCapsuleComponent* _capsuleComponent = brain->PlayerOwner()->CapsuleComponent();
	
	float _pawnRadius = 0.0f, _pawnHalfHeight = 0.0f;
	_capsuleComponent->GetScaledCapsuleSize(_pawnRadius, _pawnHalfHeight);
	const ECollisionChannel _traceChannel = playerMovementComponent->UpdatedComponent->GetCollisionObjectType();
	FVector _move = _movement;
	_move.Normalize();
	FVector _start = _capsuleComponent->GetComponentLocation() + _move * _pawnRadius * 4.0f;		
	GetWorld()->LineTraceSingleByChannel(_outHit,
									_start + movingOnGroundStats.stepMaxHeight * FVector::UpVector,
									_start +  _pawnHalfHeight * FVector::DownVector,
									_traceChannel);
	// DrawDebugLine(GetWorld(),_start + movingOnGroundStats.stepMaxHeight * FVector::UpVector,
	// 					_start +  _pawnHalfHeight * FVector::DownVector ,
	// 					FColor::Blue,
	// 					false,
	// 					-1.0f,0,2.0f);
	

	//move accordingly
	FVector2f _frameInputs = playerMovementComponent->FrameInputs();
	float _inputSizeScared = _frameInputs.SizeSquared();
	if (_inputSizeScared> 1.0f)
	{
		const float _scale = FMath::InvSqrt(_inputSizeScared);
		_frameInputs = FVector2f(_frameInputs.X * _scale, _frameInputs.Y * _scale);
	}
	playerMovementComponent->SetMomentum(_movement* playerMovementComponent->MaxVelocity() * _frameInputs.Size() / _movement.Size()); //keep speed but rotate fast
	//const bool _oui = _outHit.ImpactNormal.Dot(FVector::RightVector) == 0;
	//  DrawDebugLine(GetWorld(),
	//  				_oui ? _groundResult.ImpactPoint : _outHit.ImpactPoint,
	//  				_oui ? _groundResult.ImpactPoint + _groundResult.ImpactNormal * 100.0f : _outHit.ImpactPoint + _outHit.ImpactNormal *100.0f,
	//  				_oui ? FColor::Blue : FColor::Red,
	//  				false,
	//  				-1,0,5.0f);
	const float& _momentumSize = playerMovementComponent->Momentum().Size();
	_movement = playerMovementComponent->Momentum() - playerMovementComponent->Momentum().ProjectOnToNormal(_outHit.ImpactNormal.Dot(FVector::RightVector) == 0 ?  _groundResult.ImpactNormal : _outHit.ImpactNormal);
	_movement.Normalize();
	_movement *= _momentumSize;
	const float& _height =_outHit.ImpactPoint.Z - (_capsuleComponent->GetComponentLocation().Z - _pawnHalfHeight);
	const bool _rightHeight = _outHit.bBlockingHit &&  _height< movingOnGroundStats.stepMaxHeight && _height > movingOnGroundStats.groundHeightDetection;
	if (_rightHeight)
	{
		//DrawDebugSphere(GetWorld(),brain->PlayerOwner()->GetActorLocation(),20.0f,20,FColor::Yellow,false,-1,0,5.0f);
		//brain->PlayerOwner()->SetActorLocation(_outHit.ImpactPoint + FVector::UpVector * _pawnHalfHeight);
		_movement = _outHit.ImpactPoint - _capsuleComponent->GetComponentLocation() + _pawnHalfHeight * FVector::UpVector;
		_movement.Normalize();
		_movement *=playerMovementComponent->MaxVelocity();
		
		// FVector _dir = _outHit.ImpactPoint - (_capsuleComponent->GetComponentLocation()- _pawnHalfHeight * FVector::UpVector );
		// _movement = _movement.RotateAngleAxis(FMath::RadiansToDegrees(-FMath::FastAsin(_movement.Size()/_dir.Size())),brain->PlayerOwner()->GetActorRightVector());
	}

	playerMovementComponent->SetMomentum(_movement - FVector::UpVector * (_rightHeight ? 0.0f : _movement.Size() /2.0f));
	playerMovementComponent->MovePlayerWithMomentum();
}