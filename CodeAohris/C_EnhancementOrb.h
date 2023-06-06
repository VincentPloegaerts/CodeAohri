// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ENUM_SpawnSettings.h"
#include "IU_Player.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "C_EnhancementOrb.generated.h"

class AIU_Player;

UCLASS()
class INSIDEUS_API AC_EnhancementOrb : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMagnetBegin,FVector, _orbLocation,FVector, _playerLocation);
	UPROPERTY(VisibleAnywhere,BlueprintAssignable,BlueprintCallable,meta = (DisplayName = "FOnMagnetBegin")) FOnMagnetBegin onMagnetBegin;

	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UStaticMeshComponent* staticMesh = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		USphereComponent* orbCollider = nullptr;
	
UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		USphereComponent* magnetOrbCollider = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fSpeed = 0.1;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsAtMaxHeight = false;

	UPROPERTY()
	int iIndex = 0;
	
	UPROPERTY()
	TArray<FVector> _pointsArray;

	protected :
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsAtLocation = false;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FVector vFinalLocation = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float fMaxHeight = 500;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float fDurationBeforeDestruction = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
    	UNiagaraSystem* orbPickedUpNiagaraComp = nullptr;
	
UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
    	UNiagaraSystem* orbBaseNiagaraComp = nullptr;

	UPROPERTY(EditAnywhere, Category = "FX Settings")
	float fFXSpawnLocationOffset = -100;
	
	UPROPERTY(EditAnywhere, Category = "FX Settings")
	float fFXBaseLocationOffset = -100;

	UPROPERTY(EditAnywhere, Category = "FX Settings")
	float fAdditionalHeight = 100;

	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int iNumberOfPoints = 20;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsOrbGrounded = false;
UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bisAtFinalLocation = false;

	UPROPERTY()
	float fDestructionTimer = 0;
	
	UPROPERTY()
	bool bCanBeginDestruction = false;;

	UPROPERTY(VisibleAnywhere,Category = "Collect Settings")
	bool bCanGoToPlayer = false;;
	
	UPROPERTY(VisibleAnywhere,Category = "Collect Settings")
	AIU_Player* playerOwner = nullptr;

	
	UPROPERTY(EditAnywhere, Category = "Collect Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fcollectRange = 50;
	UPROPERTY(EditAnywhere, Category = "Collect Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fMagnetSpeed = 100;
	UPROPERTY(EditAnywhere, Category = "Collect Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fMagnetSpeedIncrease = 30;

//
// public:
// 	FORCEINLINE void SetPlayerOwner(AIU_Player* _playerOwner) {playerOwner = _playerOwner;}
// 	FORCEINLINE FOnMagnetBegin& OnMagnetBegin() { return onMagnetBegin; }
	

public:	
	AC_EnhancementOrb();
	//
	// UFUNCTION(BlueprintImplementableEvent)
	// void OnOrbHitGround();
	//
	// UFUNCTION(BlueprintImplementableEvent)
	// void OnOrbPickedUp();
	//
	// UFUNCTION(BlueprintImplementableEvent)
	// void OnOrbDestroyed();
	//
	//
	// 	
	// void SetFinalLocation(FVector _newLocation);
	// void CreateBezierCurve();

	

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	
	// UFUNCTION() void GoToPlayer();
	// UFUNCTION() void IncreaseTimer();
	// UFUNCTION() void MagnetMoveToPlayer();
	// /*
	//  Useful for the Graph, so he can put a good height for his FX
	//  */
	// UFUNCTION() void SetHeight();
	//
	// UFUNCTION() void OnOverlap(UPrimitiveComponent* OverlappedComponent, 
	// AActor* OtherActor, 
	// UPrimitiveComponent* OtherComp, 
	// int32 OtherBodyIndex, 
	// bool bFromSweep, 
	// const FHitResult &SweepResult);
	//
	// UFUNCTION() void OnMagnetOverlap(UPrimitiveComponent* OverlappedComponent, 
	// AActor* OtherActor, 
	// UPrimitiveComponent* OtherComp, 
	// int32 OtherBodyIndex, 
	// bool bFromSweep, 
	// const FHitResult &SweepResult);

};
