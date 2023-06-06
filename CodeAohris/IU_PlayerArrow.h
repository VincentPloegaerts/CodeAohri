// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IU_HealthSystemComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "ENUM_DifficultyType.h"
#include "M_UIManager.h"
#include "UI_DamageFloatingWidget.h"
#include "IU_PlayerArrow.generated.h"

class AIU_Player;

UENUM(BlueprintType)
enum arrowState { NONE,Simple,Harpoon,DoubleGuided };

UCLASS()
class INSIDEUS_API AIU_PlayerArrow : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArrowNearMaxRange);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnArrowNearMaxRange onArrowNearMaxRange = FOnArrowNearMaxRange();
	
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<arrowState> state;
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EDifficultyType> currentDifficulty = EDifficultyType::DIFFICULTY_BORN;

	//Orb dropped by the arrows
	UPROPERTY(EditAnywhere, Category = "Misc", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TMap<TEnumAsByte<EDifficultyType>,TSubclassOf<AActor>> orbBPArray;

	UPROPERTY(EditAnywhere, Category = "Misc", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TArray<TEnumAsByte<EObjectTypeQuery>> groundLayer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UIU_HealthSystemComponent> _tab;

	
	//Arrow Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float damage = 10;
	//Arrow Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float speed = 20;
	//Height of the spawning orb
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fHeightOfSpawningOrb = 50;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UStaticMeshComponent* staticMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UBoxComponent* mycollider = nullptr;

	UPROPERTY(EditAnywhere,Category = "Distance Travelled", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fMaxArrowDistance = 1000;
	
	UPROPERTY(EditAnywhere,Category = "Distance Travelled", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fMaxArrowDistanceIfInArena = 10000;

	UPROPERTY(VisibleAnywhere,Category = "Distance Travelled", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fCurrentDistanceTravelled = 0;
	

	UPROPERTY(VisibleAnywhere)
	AActor* currentTarget = nullptr;

	UPROPERTY(EditAnywhere,Category = "Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bShowDebugs = false;
	
	UPROPERTY()
	FVector vLastLocation = FVector::Zero();
	UPROPERTY()
	float fDistanceTravelledTimer = 0;
	
	UPROPERTY()
	bool bIsPrimaryShooting = false;
	UPROPERTY()
	bool bCanStun = false;
	
	UPROPERTY()
	bool bIsInArena = false;
	UPROPERTY()
	bool bhasAlreadyMadeEndRangeFX = false;

	

	UPROPERTY()
	float fHarpoonStunDuration = 0;
	
	UPROPERTY()
	int iStackNeededToStun = 0;
	
	UPROPERTY()
	bool bCanSpawnPowerOrb = false;

	UPROPERTY()
	float fRotationSpeed = 10;

	UPROPERTY()
	float fRotationTime = 0;

	UPROPERTY()
	FTimerHandle doubleGuidedRotationTimer;

	UPROPERTY()
	bool isInTuto = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UNiagaraSystem* niagaraSimpleShootComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UNiagaraSystem* niagaraSimpleShootHit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UNiagaraSystem* niagaraHarpoonShootComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UNiagaraSystem* niagaraHarpoonShootHit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UNiagaraSystem* niagaraThornShootComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UNiagaraSystem* niagaraThornShootHit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UNiagaraSystem* niagaraCritPointHit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UNiagaraSystem* niagaraHarpoonCritPointHit = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UNiagaraSystem* niagaraDoubleGuidedCritPointHit = nullptr;
	
	

	
public:
	FORCEINLINE const UBoxComponent* GetBoxComponent() const { return mycollider; }

	FORCEINLINE float GetDamage() const { return damage; }
	FORCEINLINE void SetPrimaryShootingState(bool _status) { bIsPrimaryShooting = _status; }
	FORCEINLINE void SetCanStun(bool _status) { bCanStun = _status; }
	FORCEINLINE void SetHarpoonProperty(int _iStackNeededToStun, float _fHarpoonStunDuration) { iStackNeededToStun = _iStackNeededToStun; fHarpoonStunDuration = _fHarpoonStunDuration; }
	FORCEINLINE void SetCanSpawnPowerOrb(bool _status) {bCanSpawnPowerOrb = _status;}
	FORCEINLINE void SetArrowState(arrowState _state) {state = _state;}
	FORCEINLINE void SetCurrentTarget(AActor* _target) {currentTarget = _target;}
	FORCEINLINE void SetRotationSpeedAndTime(float _speed,float _time) {fRotationSpeed = _speed;fRotationTime = _time;}
	FORCEINLINE void SetCurrentDifficulty(EDifficultyType _diffType){currentDifficulty = _diffType;}
	FORCEINLINE FOnArrowNearMaxRange* OnArrowNearMaxRange() { return &onArrowNearMaxRange; }
	
	FORCEINLINE const float& GetArrowRange() const { return  bIsInArena ? fMaxArrowDistanceIfInArena : fMaxArrowDistance; }
	FORCEINLINE void SetIsInArena(bool _status) { bIsInArena = _status; }
	FORCEINLINE void SetIsInTuto(bool _status) { isInTuto = _status; }
	
	UFUNCTION(BlueprintCallable) FORCEINLINE arrowState GetArrowState() { return state; }

	AIU_PlayerArrow();
	
	UFUNCTION(BlueprintImplementableEvent)
	void onArrowHit();


protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION() void OnProjectileOverlap(	UPrimitiveComponent* OverlappedComponent, 
											AActor* OtherActor, 
											UPrimitiveComponent* OtherComp, 
											int32 OtherBodyIndex, 
											bool bFromSweep, 
											const FHitResult &SweepResult);
	
	
	UFUNCTION(BlueprintCallable) void SetArrowFXHit(bool isCritPoint);

	
public:
	void DamageBoss(AB_Boss* _boss, const float _damage) const;
	
	UFUNCTION(BlueprintCallable) void SetArrowFX();
	UFUNCTION(BlueprintCallable) void SetDamage(float _damage);
	UFUNCTION(BlueprintCallable) void CalculateDistanceTravelled();
	//Normal behaviour of arrows
	UFUNCTION(BlueprintCallable) void MoveTo();
	//Behaviour of the double guided arrows
	UFUNCTION(BlueprintCallable) void MoveToTarget();
	
	UFUNCTION(BlueprintCallable) void StopArrowRotation();
	
	//Return a random range around the player, to set it as a final location for enhancement orb spawn 
	UFUNCTION(BlueprintCallable) FVector GetRandomRange(AIU_Player* _chara);
	UFUNCTION(BlueprintCallable) bool CheckRange(FVector _toCheck, FVector _playerLocation);

	

};



