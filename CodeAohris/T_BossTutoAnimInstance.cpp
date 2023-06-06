// Fill out your copyright notice in the Description page of Project Settings.


#include "T_BossTutoAnimInstance.h"

void UT_BossTutoAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UT_BossTutoAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UT_BossTutoAnimInstance::UpdateOnAttacking(const bool _isAttacking)
{
	isAttacking = _isAttacking;
}

void UT_BossTutoAnimInstance::UpdateIsStunned(const bool _isStunned)
{
	isStunned = _isStunned;
}

void UT_BossTutoAnimInstance::UpdateIsShootingNormal(const bool _isShootingNormal)
{
	isShootingNormal = _isShootingNormal;
}

void UT_BossTutoAnimInstance::UpdateIsShootingWall(const bool _isShootingWall)
{
	isShootingWall = _isShootingWall;
}

void UT_BossTutoAnimInstance::ResetIsShootingWall()
{
	isShootingWall = false;
}

void UT_BossTutoAnimInstance::ResetIsShootingNormal()
{
	isShootingNormal = false;
}


