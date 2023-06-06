#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_PlayerAnimInstance.generated.h"

class AIU_Player;

UCLASS()
class INSIDEUS_API UC_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isWalking = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isStrafing = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isDashing = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isJumping = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isDead = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isSimpleShooting = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isHarpoonShooting = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isThornShooting = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isPlayerChargingShoot = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool hasShootedAndChargingShoot = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isShooting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isFalling = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isTakingHit = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isSliding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isHittingGroundAfterFreeFall = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isInArena = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isOpeningArena = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool canDoStopAnimation = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isPlayerAFK = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool hasDoneAfkAnimation = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isKnocked = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isActivatingElevator = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool isOpeningChest = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool isInChestFirstPhase = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool isInChestSecondPhase = false;


	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		float verticalAxis = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		float horizontalAxis = 0.0f;

private:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void InitEvent(const AIU_Player* _player);
	
	UFUNCTION(BlueprintCallable) void UpdateOnStrafing(const bool _isStrafing);
	UFUNCTION(BlueprintCallable) void UpdateOnDashing(const bool _isDashing);
	UFUNCTION(BlueprintCallable) void UpdateOnSimpleShooting(const bool _isSimpleShooting);
	UFUNCTION(BlueprintCallable) void UpdateOnHarpoonShooting(const bool _isHarpoonShooting);
	UFUNCTION(BlueprintCallable) void UpdateOnThornShooting(const bool _isThornShooting);
	UFUNCTION(BlueprintCallable) void UpdateOnJumping(const bool _isJumping);
	UFUNCTION(BlueprintCallable) void UpdateOnPlayerChargingShoot(const bool _isChargingShoot);
	UFUNCTION(BlueprintCallable) void UpdateOnHasShootedAndChargingShoot(const bool _hasShootedAndChargingShoot);
	UFUNCTION(BlueprintCallable) void UpdateIsShooting(const bool _isShooting);
	UFUNCTION(BlueprintCallable) void UpdateIsFalling(const bool _isFalling);
	UFUNCTION(BlueprintCallable) void UpdateIsTakingHit(const bool _isTakingHit);
	UFUNCTION(BlueprintCallable) void UpdateIsHittingGroundAfterFreeFall(const bool _isHittingGroundAfterFreeFall);
	UFUNCTION(BlueprintCallable) void UpdateStopAnimation(const bool _canDoStopAnimation);

public:
	UFUNCTION(BlueprintCallable) void SetDeadStatus();
	UFUNCTION(BlueprintCallable) void ResetDeadStatus();
	UFUNCTION(BlueprintCallable) void StopIsActivatingEscalator();
	UFUNCTION(BlueprintCallable) void UpdateIsSliding(const bool _isSliding);
	UFUNCTION(BlueprintCallable) void UpdateIsInArena(const bool _isInArena);
	UFUNCTION(BlueprintCallable) void UpdateIsOpeningArena(const bool _isOpeningArena);
	UFUNCTION(BlueprintCallable) void UpdateOnAFK(const bool _isPlayerAFK);
	UFUNCTION(BlueprintCallable) void UpdateIsKnocked(const bool _isKnocked);
	UFUNCTION(BlueprintCallable) void SetIsActivatingElevator();
	UFUNCTION(BlueprintCallable) void UpdateIsOpeningChest(const bool _isOpening);
	UFUNCTION(BlueprintCallable) void UpdateIsInChestFirstPhase(const bool _isInChestFirstPhase);
	UFUNCTION(BlueprintCallable) void UpdateIsInChestSecondPhase(const bool _isInChestSecondPhase);
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsPlayerOpeningChest() { return isOpeningChest; }

private:
	UFUNCTION(BlueprintCallable) void UpdatePlayerAnimOnWalking(const bool _state);
	UFUNCTION(BlueprintCallable) void UpdateVerticalWalkAnim(const float _axisValue);
	UFUNCTION(BlueprintCallable) void UpdateHorizontalWalkAnim(const float _axisValue);
};
