#include "IU_PlayerMovementComponent.h"

#include "IU_Macros.h"
#include "IU_PlayerFSMBrain.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "STRUCT_CustomMath.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "UC_PlayerAttackComponent.h"
#include "Components/CapsuleComponent.h"

/// Engine Methods

UIU_PlayerMovementComponent::UIU_PlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UIU_PlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	world->GetTimerManager().SetTimer(playerMoveTimer, this,
	                                  &UIU_PlayerMovementComponent::CheckPlayerHasMoveWithoutJumping, fCheckRate, true,
	                                  fCheckRate);
	//world->GetTimerManager().SetTimer(animDirectionTimer, this, &UIU_PlayerMovementComponent::UpdateAnimDirectionVectors, 0.1, true, 0.1);

	onDashed.AddDynamic(this, &UIU_PlayerMovementComponent::ResetPlayerAFKTime);
	onDashed.AddDynamic(this, &UIU_PlayerMovementComponent::CallResetTakeDamageWithDelay);
	onJump.AddDynamic(this, &UIU_PlayerMovementComponent::SetIsJumpingTrue);
	onHitGround.AddDynamic(this, &UIU_PlayerMovementComponent::SetIsJumpingFalse);

	ownerPawn = Cast<AIU_Player>(GetOwner());
	if (ownerPawn)
	{
		vLastLocationWithoutJumping = ownerPawn->GetActorLocation();
		vLastLocationWithoutJumping.Z = 0;
		ownerCapsule = Cast<UCapsuleComponent>(ownerPawn->GetComponentByClass(UCapsuleComponent::StaticClass()));
		brain = Cast<UIU_PlayerFSMBrain>(ownerPawn->GetComponentByClass(UIU_PlayerFSMBrain::StaticClass()));
		playerAttackComponent = ownerPawn->GetCharacterAttackComponent();
		Event_Add(ownerPawn->OnIsInArena(), UIU_PlayerMovementComponent::SetIsInArena);
	}

	hasAlreadyStopped = true;
}

void UIU_PlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	onUpdate.Broadcast(DeltaTime);

	lastDirection = frameDirection;
	frameDirection = FVector::ZeroVector;

	CheckForStopAnimation();

	if (bIsKnockBack)
	{
		MovePlayerWithMomentum();
	}

	if (lastDirection.Size() > 0.2f)
	{
		currentRuningTime += GetWorld()->DeltaTimeSeconds;
		onPlayerStopWalking.Broadcast(false);
		//CheckAnimToPlay();
		hasAlreadyStopped = false;
	}

	if (lastDirection.Size() < 0.05f && currentRuningTime < runningTimeNeededToDeclenchStopAnimation)
	{
		currentRuningTime = 0.f;
	}
}
#pragma region NoAnim

#if WITH_EDITOR
void UIU_PlayerMovementComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (GetWorld() && GetWorld()->GetGameInstance())
	{
		//UE_LOG(LogTemp,Warning,TEXT("PlayerMovementComponent PostEditChangeProperty To remove on Ship later in dev"));
		if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(
			UIU_PlayerMovementComponent, movementStats) && brain)
		{
			brain->PassALlDatas(movementStats);
		}
	}
}

#endif


void UIU_PlayerMovementComponent::SetIsInArena(const bool _state)
{
	bIsInArena = _state;
	onUpdate.RemoveDynamic(this, &UIU_PlayerMovementComponent::IncreasePlayerAFKTime);
}

void UIU_PlayerMovementComponent::CheckPlayerHasMoveWithoutJumping()
{
	if (!bIsInArena)return;
	FVector _newLocation(ownerPawn->GetActorLocation());
	_newLocation.Z = 0;

	if (FVector::Dist(_newLocation, vLastLocationWithoutJumping) < fMinimalMoveDistance)
	{
		if (bIsMovingWithoutJumping)
			onUpdate.AddDynamic(this, &UIU_PlayerMovementComponent::IncreasePlayerAFKTime);
		bIsMovingWithoutJumping = false;
	}
	else
	{
		if (!bIsJumping)
		{
			ResetPlayerAFKTime();
			vLastLocationWithoutJumping = _newLocation;
		}
	}
}

void UIU_PlayerMovementComponent::ResetPlayerAFKTime()
{
	fAFKTime = 0.0f;
	if (!bIsMovingWithoutJumping)
		onUpdate.RemoveDynamic(this, &UIU_PlayerMovementComponent::IncreasePlayerAFKTime);
	bIsMovingWithoutJumping = true;
	bIsJumping = false;
}

void UIU_PlayerMovementComponent::ResetPlayerTakeDamage() const
{
	ownerPawn->HealthComponent()->SetCanTakeDamages(true);
}

void UIU_PlayerMovementComponent::CallResetTakeDamageWithDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(handleDashInvincibility);
	GetWorld()->GetTimerManager().SetTimer(handleDashInvincibility,this,&UIU_PlayerMovementComponent::ResetPlayerTakeDamage,dashImmunityDuration);
}

void UIU_PlayerMovementComponent::IncreasePlayerAFKTime(const float& _deltaTime)
{
	fAFKTime += _deltaTime;
}

void UIU_PlayerMovementComponent::SetIsJumpingTrue()
{
	bIsJumping = true;
}

void UIU_PlayerMovementComponent::SetIsJumpingFalse()
{
	bIsJumping = false;
}


/// Movements Methods

void UIU_PlayerMovementComponent::MoveForward(float _forwardValue)
{
	if (FMath::Abs(_forwardValue) < axisAcceptance)
	{
		frameInputs.X = _forwardValue;
		onVerticalValueChange.Broadcast(_forwardValue);
		return;
	}
	if (!canMove)return;

	// const float& _absInput = FMath::Abs(_forwardValue);
	// if (_absInput < axisMaxSlowInput)
	// {
	// 	frameInputs.X = axisSlowValue * (_forwardValue < 0 ? -1.0f : 1.0f);
	// 	_forwardValue = frameInputs.X;
	// }
	// else if (_absInput < axisMinFastInput)
	// {
	// 	frameInputs.X = FCustomMath::SlowInputs(axisMaxSlowInput, axisSlowValue, _absInput) * (
	// 		_forwardValue < 0.0f ? -1.0f : 1.0f);
	// 	_forwardValue = frameInputs.X;
	// }
	// else
	// {
	// 	frameInputs.X = _forwardValue < 0.0f ? -1.0f : 1.0f;
	// 	_forwardValue = frameInputs.X;
	// }
	frameInputs.X = _forwardValue;
	onVerticalValueChange.Broadcast(_forwardValue);
	FVector _direction = GetCameraForwardVector() * _forwardValue;
	_direction.Z = 0.0f;
	//_direction.Normalize();
	frameDirection += _direction;
}

void UIU_PlayerMovementComponent::MoveRight(float _rightValue)
{
	if (FMath::Abs(_rightValue) < axisAcceptance)
	{
		frameInputs.Y = _rightValue;
		onHorizontalValueChange.Broadcast(_rightValue);
		return;
	}
	if (!canMove)return;
	// const float& _absInput = FMath::Abs(_rightValue);
	// if (_absInput < axisMaxSlowInput)
	// {
	// 	frameInputs.Y = axisSlowValue * (_rightValue < 0 ? -1.0f : 1.0f);
	// 	_rightValue = frameInputs.Y;
	// }
	// else if (_absInput < axisMinFastInput)
	// {
	// 	frameInputs.Y = FCustomMath::SlowInputs(axisMaxSlowInput, axisSlowValue, _absInput) * (
	// 		_rightValue < 0.0f ? -1.0f : 1.0f);
	// 	_rightValue = frameInputs.Y;
	// }
	// else
	// {
	// 	frameInputs.Y = _rightValue < 0.0f ? -1.0f : 1.0f;
	// 	_rightValue = frameInputs.Y;
	// }
	frameInputs.Y = _rightValue;
	onHorizontalValueChange.Broadcast(_rightValue);
	FVector _direction = GetCameraRightVector() * _rightValue;
	_direction.Z = 0.0f;
	//_direction.Normalize();
	frameDirection += _direction;
}

void UIU_PlayerMovementComponent::MovePlayer(FVector _direction)
{
	if (!_direction.IsNearlyZero())
	{
		const float& _deltaTime = world->DeltaTimeSeconds;

		FHitResult _hit;
		const float& _backwardValue = playerAttackComponent->IsPlayerChargingShoot() && frameInputs.X < 0
			                              ? (frameInputs.X / -1.0f)
			                              : 0.0f;
		const float& _speedPercent = FMath::Lerp(100.0f, backwardSpeedSlowPercent, _backwardValue);
		_direction = _direction * _deltaTime * 100.0f * fSlowFactor * (_speedPercent / 100.0f);
		SafeMoveUpdatedComponent(_direction, UpdatedComponent->GetComponentRotation(), true, _hit);
		// If we bumped into something, try to slide along it
		if (_hit.IsValidBlockingHit())
		{
			SlideAlongSurface(_direction, 1.0f - _hit.Time, _hit.Normal, _hit);
		}
		if (playerCamera)
		{
			playerCamera->UpdateLocation(_deltaTime);
			if (!brain->PlayerOwner()->GetCharacterAttackComponent()->IsPlayerChargingShoot() && canMove)
				RotateToDirection(_direction, playerRotateSpeed);
		}
	}
}

void UIU_PlayerMovementComponent::RotateToDirection(const FVector& _direction, const float _rotateSpeed) const
{
	//DrawDebugLine(GetWorld(),GetActorLocation()+ FVector::UpVector * 100.0f,GetActorLocation() + _direction *100.0f + FVector::UpVector * 100.0f,FColor::Red,false,-1,0,5);
	const FVector& _targetDirection = FVector(_direction.X, _direction.Y, 0.0f);
	const FRotator& _lookAt = _targetDirection.Rotation();
	const FRotator& _newRotation = UKismetMathLibrary::RInterpTo(OwnerRotation(), _lookAt, world->DeltaTimeSeconds,
	                                                             _rotateSpeed);
	ownerPawn->SetActorRotation(_newRotation);
}


//might need a maxVelocity later
void UIU_PlayerMovementComponent::SetMomentum(const FVector& _momentum, bool _useLimit)
{
	momentum = _momentum;
	if (_useLimit)
	{
		const FVector& _horizontalMovement = FVector(momentum.X, momentum.Y, 0);
		const FVector& _clampedMomentum = _horizontalMovement.GetClampedToMaxSize(maxVelocity);
		momentum = FVector(_clampedMomentum.X, _clampedMomentum.Y, momentum.Z);
	}
}

void UIU_PlayerMovementComponent::IncreaseMomentum(const FVector& _momentum, bool _useLimit)
{
	momentum += _momentum;
	if (_useLimit)
	{
		const FVector& _horizontalMovement = FVector(momentum.X, momentum.Y, 0);
		const FVector& _clampedMomentum = _horizontalMovement.GetClampedToMaxSize(maxVelocity);
		momentum = FVector(_clampedMomentum.X, _clampedMomentum.Y, momentum.Z);
	}
}

void UIU_PlayerMovementComponent::MovePlayerWithMomentum()
{
	MovePlayer(momentum);
}


/// Ground Methods

bool UIU_PlayerMovementComponent::CheckIsGrounded(FHitResult& _outHit) const
{
	if (!ownerCapsule) return false;
	const FVector& _start = ownerCapsule->GetComponentLocation();
	const FVector& _end = _start + FVector(0.0f, 0.0f, -movementStats.movingOnGroundData.groundHeightDetection);
	const ECollisionChannel& _traceChannel = UpdatedComponent->GetCollisionObjectType();
	const float& _radius = ownerCapsule->GetScaledCapsuleRadius();
	float _pawnRadius = 0.0f, _pawnHalfHeight = 0.0f;
	ownerCapsule->GetScaledCapsuleSize(_pawnRadius, _pawnHalfHeight);
	const float& _shrinkHeight = (_pawnHalfHeight - _pawnRadius) * 0.1f;
	const FCollisionShape& _capsuleShape = FCollisionShape::MakeCapsule(_radius, _pawnHalfHeight - _shrinkHeight);

	const FCollisionQueryParams _collisionParams;
	const FCollisionResponseParams _responseParams;

	// Test with a box that is enclosed by the capsule.
	const float& _capsuleRadius = _capsuleShape.GetCapsuleRadius() * 0.707f;
	const float& _capsuleHeight = _capsuleShape.GetCapsuleHalfHeight();
	const FCollisionShape& _boxShape =
		FCollisionShape::MakeBox(FVector(_capsuleRadius, _capsuleRadius, _capsuleHeight));

	// First test with the box rotated so the corners are along the major axes (ie rotated 45 degrees).
	bool _blockingHit = world->SweepSingleByChannel(_outHit, _start, _end, boxRotation, _traceChannel, _boxShape,
	                                                _collisionParams, _responseParams);

	//dk if needed
	if (!_blockingHit)
	{
		// Test again with the same box, not rotated.
		_outHit.Reset(1.f, false);
		_blockingHit = world->SweepSingleByChannel(_outHit, _start, _end, FQuat::Identity, _traceChannel, _boxShape,
		                                           _collisionParams, _responseParams);
	}

	if (_blockingHit && CheckAcceptableSlope(_outHit))
		return _blockingHit;
	// if (!_blockingHit)return _blockingHit;

	// Line trace
	const float& TraceDist = movementStats.movingOnGroundData.groundHeightDetection + _pawnHalfHeight;
	const FVector& _down = FVector(0.f, 0.f, -TraceDist);

	//FHitResult Hit; //default constructor set Time = 1.f and value never used idk
	const bool& _toReturn = world->LineTraceSingleByChannel(_outHit, _start, _start + _down, _traceChannel,
	                                                        _collisionParams, _responseParams);
	//GetWorld()->LineTraceSingleByChannel(_outHit, _start + momentum, _start + _down + _start + momentum, _traceChannel,_params,_responseParams);
	return _toReturn;
}

bool UIU_PlayerMovementComponent::CheckAcceptableSlope(const FHitResult& _result) const
{
	const float& _angle = FCustomMath::AngleBetweenVectors(_result.ImpactNormal, FVector::UpVector);
	return FMath::RadiansToDegrees(_angle) <= movementStats.movingOnGroundData.acceptableSlopeAngle;
}

#pragma endregion

// Animation Methods
void UIU_PlayerMovementComponent::CheckAnimToPlay()
{
	if (!canRotateAnim)return;

	AActor* _owner = GetOwner();
	if (!_owner)return;

	if (frameDirection == FVector::ZeroVector && lastDirection == FVector::ZeroVector)return;

	FHitResult _outHit;
	if (!CheckIsGrounded(_outHit))return;

	const FRotator& _rot = UKismetMathLibrary::FindLookAtRotation(lastDirection, frameDirection);
	const FVector& _angle = _rot.Euler();


	float _Zangle = FMath::Abs(_angle.Z - previousZAngle);


	if (_Zangle > 155 && _Zangle < 205)
	{
		UE_LOG(LogTemp, Warning, TEXT("JE ME RETOURNE AVEC ANGLE : %f"), _Zangle);
		//onPlayerTwist.Broadcast();
		canRotateAnim = false;
		FTimerHandle _timerRotationAnim;

		world->GetTimerManager().SetTimer(_timerRotationAnim, this,
		                                  &UIU_PlayerMovementComponent::SetCanAnimRotationToTrue, 1.2, false, 1.2);
		_owner->SetActorRotation(OwnerRotation());
		UE_LOG(LogTemp, Warning, TEXT("JE ME RETOURNE "));
		_Zangle = 0;
		return;
	}

	return;
}

void UIU_PlayerMovementComponent::UpdateAnimDirectionVectors()
{
	if (!canRotateAnim)return;

	const AActor* _owner = GetOwner();
	if (!_owner)return;

	previousAnimDirection = currentAnimDirection;
	currentAnimDirection = _owner->GetActorForwardVector();

	//CheckAnimToPlay();
}

void UIU_PlayerMovementComponent::SetCanAnimRotationToTrue()
{
	canRotateAnim = true;
}

void UIU_PlayerMovementComponent::CheckForStopAnimation()
{
	// if (hasAlreadyStopped)
	// {
	// 	if (hasAlreadyMovedWhenStop)return;
	//
	// 	moveWhenStoppedTimer += GetWorld()->DeltaTimeSeconds;
	//
	// 	if (AIU_Player* _player = brain->PlayerOwner())
	// 	{
	// 		FVector _toGo = _player->GetActorLocation() + _player->GetActorForwardVector() * 50;
	//
	// 		while (FVector::Dist(_toGo, _player->GetActorLocation()) > 0.2)
	//
	// 		{
	// 			FVector _goodPos = UKismetMathLibrary::VLerp(_toGo, _player->GetActorLocation(),
	// 			                                             GetWorld()->DeltaTimeSeconds);
	// 			_player->SetActorLocation(_goodPos);
	// 			UE_LOG(LogTemp, Warning, TEXT("JE BOUGE APRES LE STOP"));
	// 		}
	// 	}
	//
	// 	if (moveWhenStoppedTimer > 0.5)
	//
	// 	{
	// 		hasAlreadyMovedWhenStop = true;
	// 	}
	// }

	if (currentRuningTime < runningTimeNeededToDeclenchStopAnimation) return;
	//UE_LOG(LogTemp, Warning, TEXT("FRAMEDIRECTION SIZE : %f"), frameDirection.Size());

	if (lastDirection.Size() < 0.05)
	{
		if (hasAlreadyStopped)return;

		fNoInputForAnimationTimer += GetWorld()->DeltaTimeSeconds;

		if (fNoInputForAnimationTimer > timeBeforeStopAnimationPlay)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("JE ME SUIS STOOOOOOOOOOOP"));
			hasAlreadyStopped = true;
			hasAlreadyMovedWhenStop = false;
			onPlayerStopWalking.Broadcast(true);

			FTimerHandle _timer;
			GetWorld()->GetTimerManager().SetTimer(_timer, this, &UIU_PlayerMovementComponent::ResetStopAnimation, 0.5f);

			fNoInputForAnimationTimer = 0;
			currentRuningTime = 0;
		}
	}
}

UFUNCTION(BlueprintCallable) void UIU_PlayerMovementComponent::ResetStopAnimation()
{
	onPlayerStopWalking.Broadcast(false);
}

void UIU_PlayerMovementComponent::StartKnockBack(const FVector& _knockDirection, const float& _duration)
{
	if (bIsKnockBack)return;
	ROG_ERROR(_duration <= 0.f, "UIU_PlayerMovementComponent::StartKnockBack -> Can't knockBack for less or egal to 0s");
	
	world->GetTimerManager().SetTimer(knockBackTimer, this, &UIU_PlayerMovementComponent::StopKnockBack, _duration);
	
	bIsKnockBack = true;
	ownerPawn->SetPlayerInputs(false);
	ownerPawn->GetPlayerAnimInstance()->UpdateIsKnocked(true);
	SetMomentum(FVector(_knockDirection.X, _knockDirection.Y, -FMath::Abs(momentum.Z)), false);
	MovePlayerWithMomentum();
}

void UIU_PlayerMovementComponent::StopKnockBack()
{
	bIsKnockBack = false;
	ownerPawn->SetPlayerInputs(true);
	ownerPawn->GetPlayerAnimInstance()->UpdateIsKnocked(false);
	
	SetMomentum(FVector::ZeroVector, false);
}