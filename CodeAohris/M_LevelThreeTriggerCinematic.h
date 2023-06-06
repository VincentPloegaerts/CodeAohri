// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "M_LevelThreeTriggerCinematic.generated.h"

UCLASS()
class INSIDEUS_API AM_LevelThreeTriggerCinematic : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* boxCollider = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Settings | Reference")
		TSoftObjectPtr<AActor> toCloseRight = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings | Reference")
		TSoftObjectPtr<AActor> toCloseLeft = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Settings | Values")
		float rotationValuesRight = 80;
	UPROPERTY(EditAnywhere, Category = "Settings | Values")
		float rotationValuesLeft = -80;

	UPROPERTY()
	bool canBeClosed = false;
	UPROPERTY()
	bool hasAlreadyClosed = false;
	
public:	
	// Sets default values for this actor's properties
	AM_LevelThreeTriggerCinematic();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable) void OnActorOverlap(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp,
		int32 _index, bool _sweep, const FHitResult& _result);

public:
	void RotateDoor();

};
