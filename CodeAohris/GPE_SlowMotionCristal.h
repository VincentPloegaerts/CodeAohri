// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GPE_PlatformWithHealth.h"
#include "Components/BoxComponent.h"
#include "GPE_SlowMotionCristal.generated.h"

/**
 * 
 */
UCLASS()
class INSIDEUS_API AGPE_SlowMotionCristal : public AGPE_BaseGPE
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UBillboardComponent* billboard = nullptr;		
#endif
	
	UPROPERTY(EditAnywhere, Category = "Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UStaticMeshComponent* staticMesh = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UBoxComponent* boxCollider = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UIU_HealthSystemComponent* healthComponent = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Values", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float amountOfSlowMoPowerAddedWhenBroken = 2;

	UPROPERTY(EditAnywhere, Category = "Values", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float timeBeforeRespawn = 1;

	
public:	
	AGPE_SlowMotionCristal();

protected:
	virtual void BeginPlay() override;
	UFUNCTION() void DeathBehaviour();
	UFUNCTION() void RespawnBehaviour();
};
