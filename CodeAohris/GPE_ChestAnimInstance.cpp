// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE_ChestAnimInstance.h"

void UGPE_ChestAnimInstance::UpdateIsOpen(const bool _isOpen)
{
	isOpen = _isOpen;
}

void UGPE_ChestAnimInstance::UpdateIsInFirstPhase(const bool _isInFirstPhase)
{
	isInFirstPhase = _isInFirstPhase;
}

void UGPE_ChestAnimInstance::UpdateIsInSecondPhase(const bool _isInSecondPhase)
{
	isInSecondPhase = _isInSecondPhase;
}
