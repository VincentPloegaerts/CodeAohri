// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "M_LevelTwoTriggerCinematic.generated.h"

UCLASS()
class INSIDEUS_API AM_LevelTwoTriggerCinematic : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* boxCollider = nullptr;
	
public:	
	AM_LevelTwoTriggerCinematic();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) void OnActorOverlap(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp,
									  int32 _index, bool _sweep, const FHitResult& _result);

};

 



