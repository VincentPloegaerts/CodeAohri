// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE_ElevatorAnimInstance.h"

void UGPE_ElevatorAnimInstance::UpdateIsActivated(const bool _isActivated)
{
	isActivated = _isActivated;
}

void UGPE_ElevatorAnimInstance::InitAnimInstance(float _animPlayRateValue)
{
	fAnimCustomPlayRate = _animPlayRateValue;
}
