// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GPE_ShotAnimInstance.generated.h"


class AGPE_Shot;
/**
 * 
 */
UCLASS()
class INSIDEUS_API UGPE_ShotAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected :
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool isShooting = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool isTakingHit = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool isActivated = false;


	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void InitEvent(AGPE_Shot* _shot);


	UFUNCTION(BlueprintCallable) void UpdateIsTakingHit(const bool _isTakingHit);
	UFUNCTION(BlueprintCallable) void UpdateIsShooting(const bool _isShooting);
	UFUNCTION(BlueprintCallable) void UpdateIsActivated(const bool _isActivated);
	/* Needed for anim because the GPE_Shot shoot with a pattern*/
	UFUNCTION(BlueprintCallable) void UpdateIsShootingToFalse();
};
