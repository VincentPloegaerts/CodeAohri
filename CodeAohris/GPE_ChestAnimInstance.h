// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GPE_ChestAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INSIDEUS_API UGPE_ChestAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	protected :
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool isOpen = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool isInFirstPhase = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool isInSecondPhase = false;

public :

	UFUNCTION(BlueprintCallable) void UpdateIsOpen(const bool _isOpen);
	UFUNCTION(BlueprintCallable) void UpdateIsInFirstPhase(const bool _isInFirstPhase);
	UFUNCTION(BlueprintCallable) void UpdateIsInSecondPhase(const bool _isInSecondPhase);
};
