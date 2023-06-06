// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GPE_ElevatorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INSIDEUS_API UGPE_ElevatorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected :
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool isActivated = false;

	UPROPERTY(EditAnywhere, Category = "GPE|Animation", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bUseAnimCustomPlayRate = false;
	UPROPERTY(EditAnywhere, Category = "GPE|Animation", BlueprintReadWrite, meta = (EditCondition = bUseCustomPlayRate, EditConditionHides, AllowPrivateAccess = True))
		float fAnimCustomPlayRate = 1.0f;


public :
	UFUNCTION(BlueprintCallable) void UpdateIsActivated(const bool _isActivated);
	UFUNCTION(BlueprintCallable) void InitAnimInstance(float _animPlayRateValue);
};
