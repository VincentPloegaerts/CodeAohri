#include "C_PlayerAnimInstance.h"
#include "IU_Player.h"
#include "IU_PlayerMovementComponent.h"
#include "UC_PlayerAttackComponent.h"
#include "IU_HealthSystemComponent.h"


void UC_PlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	if (AIU_Player* _player = Cast<AIU_Player>(GetOwningActor()))
	{
		InitEvent(_player);
	}
}

void UC_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UC_PlayerAnimInstance::InitEvent(const AIU_Player* _player)
{

	if (UIU_PlayerMovementComponent* _movement = _player->MovementComponent())
	{
		_movement->OnJumpEvent().AddDynamic(this, &UC_PlayerAnimInstance::UpdateOnJumping);
		_movement->OnDashEvent().AddDynamic(this, &UC_PlayerAnimInstance::UpdateOnDashing);
		_movement->OnFallingEvent().AddDynamic(this, &UC_PlayerAnimInstance::UpdateIsFalling);
		_movement->OnVerticalValueChange().AddDynamic(this, &UC_PlayerAnimInstance::UpdateVerticalWalkAnim);
		_movement->OnHorizontalValueChange().AddDynamic(this, &UC_PlayerAnimInstance::UpdateHorizontalWalkAnim);
		_movement->OnHittingGroundAfterFreeFall().AddDynamic(this, &UC_PlayerAnimInstance::UpdateIsHittingGroundAfterFreeFall);
		_movement->OnPlayerStopWalking().AddDynamic(this, &UC_PlayerAnimInstance::UpdateStopAnimation);
		
	}

	if(UC_PlayerAttackComponent* _attackComp = _player->GetCharacterAttackComponent() )
	{
		_attackComp->OnSimpleShoot().AddDynamic(this,&UC_PlayerAnimInstance::UC_PlayerAnimInstance::UpdateOnSimpleShooting);
		_attackComp->OnPlayerChargingShoot().AddDynamic(this,&UC_PlayerAnimInstance::UpdateOnPlayerChargingShoot);
		_attackComp->OnThornShoot().AddDynamic(this,&UC_PlayerAnimInstance::UpdateOnThornShooting);
		_attackComp->OnHarpoonShoot().AddDynamic(this,&UC_PlayerAnimInstance::UpdateOnHarpoonShooting);
		_attackComp->OnHasShootedAndChargingShoot().AddDynamic(this,&UC_PlayerAnimInstance::UpdateOnHasShootedAndChargingShoot);
		_attackComp->OnShooting().AddDynamic(this,&UC_PlayerAnimInstance::UpdateIsShooting);
	}

	if(UIU_HealthSystemComponent* _healthComp = _player->HealthComponent())
	{
		_healthComp->OnDeath()->AddDynamic(this, &UC_PlayerAnimInstance::SetDeadStatus);
		Event_AddUnique(_healthComp->OnTakingHit(), UC_PlayerAnimInstance::UpdateIsTakingHit);
	}
}

void UC_PlayerAnimInstance::UpdateOnStrafing(const bool _isStrafing)
{
	isStrafing = _isStrafing;
}

void UC_PlayerAnimInstance::UpdateOnJumping(const bool _isJumping)
{
	isJumping = _isJumping;
}

void UC_PlayerAnimInstance::UpdateOnPlayerChargingShoot(const bool _isChargingShoot)
{
	isPlayerChargingShoot = _isChargingShoot;
}

void UC_PlayerAnimInstance::UpdateOnHasShootedAndChargingShoot(const bool _hasShootedAndChargingShoot)
{
	hasShootedAndChargingShoot = _hasShootedAndChargingShoot;
}

void UC_PlayerAnimInstance::UpdateOnDashing(const bool _isDashing)
{
	isDashing = _isDashing;
}

void UC_PlayerAnimInstance::UpdateOnSimpleShooting(const bool _isSimpleShooting)
{
	isSimpleShooting = _isSimpleShooting;
}

void UC_PlayerAnimInstance::UpdateOnHarpoonShooting(const bool _isHarpoonShooting)
{
	isHarpoonShooting = _isHarpoonShooting;
}

void UC_PlayerAnimInstance::UpdateOnThornShooting(const bool _isThornShooting)
{
	isThornShooting = _isThornShooting;
}


void UC_PlayerAnimInstance::SetDeadStatus()
{
	//AIU_Player* _player = Cast<AIU_Player>(GetWorld()->GetFirstPlayerController());
	//if (!_player)return;

	//if (!_player->IsInArena())return;

	isDead = true;

	UE_LOG(LogTemp, Warning, TEXT("Is In Arena : %s"),isInArena ? TEXT("true") : TEXT("false"));

	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &UC_PlayerAnimInstance::ResetDeadStatus,
		5, false);

}

void UC_PlayerAnimInstance::UpdateIsShooting(const bool _isShooting)
{
	isShooting = _isShooting;
}

void UC_PlayerAnimInstance::UpdateIsFalling(const bool _isFalling)
{
	isFalling = _isFalling;
}

void UC_PlayerAnimInstance::UpdateIsTakingHit(const bool _isTakingHit)
{
	isTakingHit = _isTakingHit;
}

void UC_PlayerAnimInstance::UpdateIsHittingGroundAfterFreeFall(const bool _isHittingGroundAfterFreeFall)
{
	isHittingGroundAfterFreeFall = _isHittingGroundAfterFreeFall;
}

void UC_PlayerAnimInstance::UpdateStopAnimation(const bool _canDoStopAnimation)
{
	canDoStopAnimation = _canDoStopAnimation;
}

void UC_PlayerAnimInstance::StopIsActivatingEscalator()
{
	isActivatingElevator = false;
	
}

void UC_PlayerAnimInstance::UpdateOnAFK(const bool _isPlayerAFK)
{
	if(_isPlayerAFK == false)
		hasDoneAfkAnimation = false;
	
	isPlayerAFK = _isPlayerAFK;
	
}

void UC_PlayerAnimInstance::UpdateIsKnocked(const bool _isKnocked)
{
	isKnocked = _isKnocked;
}

void UC_PlayerAnimInstance::SetIsActivatingElevator()
{
	isActivatingElevator = true;
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &UC_PlayerAnimInstance::StopIsActivatingEscalator,
											   1, false);
	
}

void UC_PlayerAnimInstance::UpdateIsOpeningChest(const bool _isOpening)
{
	isOpeningChest = _isOpening;
}

void UC_PlayerAnimInstance::UpdateIsInChestFirstPhase(const bool _isInChestFirstPhase)
{
	isInChestFirstPhase = _isInChestFirstPhase;
}

void UC_PlayerAnimInstance::UpdateIsInChestSecondPhase(const bool _isInChestSecondPhase)
{
	isInChestSecondPhase = _isInChestSecondPhase;
}

void UC_PlayerAnimInstance::UpdateIsInArena(const bool _isInArena)
{
	isInArena = _isInArena;
}

void UC_PlayerAnimInstance::UpdateIsOpeningArena(const bool _isOpeningArena)
{
	isOpeningArena = _isOpeningArena;
}

void UC_PlayerAnimInstance::UpdateIsSliding(const bool _isSliding)
{
	isSliding = _isSliding;
}

void UC_PlayerAnimInstance::ResetDeadStatus()
{
	isDead = false;
}

void UC_PlayerAnimInstance::UpdatePlayerAnimOnWalking(const bool _state)
{
	isWalking = _state;
}

void UC_PlayerAnimInstance::UpdateVerticalWalkAnim(const float _axisValue)
{
	verticalAxis = _axisValue;
}

void UC_PlayerAnimInstance::UpdateHorizontalWalkAnim(const float _axisValue)
{
	horizontalAxis = _axisValue;
}