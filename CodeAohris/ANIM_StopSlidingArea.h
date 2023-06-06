// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ANIM_StopSlidingArea.generated.h"

UCLASS()
class INSIDEUS_API AANIM_StopSlidingArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AANIM_StopSlidingArea();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* boxCollider = nullptr;
	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult &SweepResult);

};
