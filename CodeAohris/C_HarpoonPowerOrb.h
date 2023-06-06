// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_PowerOrb.h"
#include "C_HarpoonPowerOrb.generated.h"

/**
 * 
 */
UCLASS()
class INSIDEUS_API AC_HarpoonPowerOrb : public AC_PowerOrb
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fLifeTime = 5;

	UPROPERTY()
	bool haveALifeSpan = true;

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void BeginPlay() override;
	void SetPowerOrbLifeSpan();

	public :
	FORCEINLINE void SetHaveALifeSpan(bool _status) {haveALifeSpan = _status;}
};
