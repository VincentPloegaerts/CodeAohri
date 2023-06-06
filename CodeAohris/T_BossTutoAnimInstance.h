// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "T_BossTutoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INSIDEUS_API UT_BossTutoAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isAttacking = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isStunned = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isShootingNormal = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		bool isShootingWall = false;



	
private:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;



public :
	
	UFUNCTION(BlueprintCallable) void UpdateOnAttacking(const bool _isAttacking);
	UFUNCTION(BlueprintCallable) void UpdateIsStunned(const bool _isStunned);
	UFUNCTION(BlueprintCallable) void ResetIsStunned() {isStunned = false; };
	UFUNCTION(BlueprintCallable) void UpdateIsShootingNormal(const bool _isShootingNormal);
	UFUNCTION(BlueprintCallable) void UpdateIsShootingWall(const bool _isShootingWall);
	
	UFUNCTION(BlueprintCallable) void ResetIsShootingWall();
	UFUNCTION(BlueprintCallable) void ResetIsShootingNormal();
	

};
