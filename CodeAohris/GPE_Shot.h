#pragma once
#include "CoreMinimal.h"

#include "NiagaraComponent.h"
#include "Components/ArrowComponent.h"
#include "GPE_PatternCasterComponent.h"

#include "GPE_PlatformWithHealth.h"
#include "GPE_Shot.generated.h"

class AP_PoolingTriggerSphere;
class UBillboardComponent;
class UM_DifficultyManager;

UCLASS()
class INSIDEUS_API AGPE_Shot : public AGPE_PlatformWithHealth
{
	GENERATED_BODY()

#pragma region AnimationEvents
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShoot, bool, _onShooting);
	FOnShoot onShoot;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivate, bool, _onActivate);
	FOnActivate onActivate;

#pragma endregion

#pragma region Properties

#pragma region Shoot Properties
	
	//delay before the pattern is shot when player enters the range
	UPROPERTY(EditAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float firstShotDelay = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool firstDelayFinished = false;

	UPROPERTY(EditAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool waitDelayAfterFirst = false;
	
	//Bool to activate debugs, should be false if not preparing gpe shots,it's a heavy load on fps
	UPROPERTY(EditAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool showDebug = true;

	//Should the Range Detection be a sphere Range or A box one
	UPROPERTY(EditAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool useBox = false;

	//Defines this GPE range, can be seen by turning on showDebug
	UPROPERTY(EditAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True),
	meta=(EditCondition="!useBox",EditConditionHides))
		float range = 200.0f;

	//Defines this GPE range using a Box, can be seen by turning on showDebug
	UPROPERTY(EditAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True),
	meta=(EditCondition="useBox",EditConditionHides))
		FVector boxRange = FVector(200.0f);
	
	//If true this gpe will fire at 'targetPoint' when player enter its range
	UPROPERTY(EditAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool shootAtPoint = false;

	//If true this gpe will fire from 'shootStartPoint'
	UPROPERTY(EditAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool shootFromPoint = true;
	
	//Delay between 2 patterns, does not take account of the pattern fire rate
	UPROPERTY(EditAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float shootDelay = 0.5f;
	
	//Float increasing to reach shootDelay and shoot a new pattern when needed
	UPROPERTY(VisibleAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float shootingTime = 0.0f;
	
	//To add some height to the shot above its target, useful for projectiles with gravity
	UPROPERTY(EditAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float zOffsetShoot = 0.0f;
	
	//AActor to check for range and act in accordance, if set to none the player in M_Gamemode will be chosen
	UPROPERTY(EditAnywhere, Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		AActor* target = nullptr;
	
	//Layer that this GPE cannot detect the player through
	UPROPERTY(EditAnywhere,Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<TEnumAsByte<EObjectTypeQuery>> wallLayer = TArray<TEnumAsByte<EObjectTypeQuery>>();
	
	//used for the linetrace to not take account of self and not having to create a new array each tick
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<AActor*> toIgnoreActors = TArray<AActor*>();

#pragma endregion

#pragma region Pulling
	
	//range to add to the pulling zone compared to just 'range'
	UPROPERTY(EditAnywhere, Category = "GPE|Pulling",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float additionalRadiusForPulling = 100.0f;
	//Pooling Trigger to spawn
	UPROPERTY(EditAnywhere, Category = "GPE|Pulling",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		TSubclassOf<AP_PoolingTriggerSphere> poolingTriggerType;
	UPROPERTY(VisibleAnywhere, Category = "GPE|Pulling",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		AP_PoolingTriggerSphere* poolingTrigger = nullptr;

#pragma endregion

#pragma region Animation
	//delay before the pattern is shot when player enters the range
	UPROPERTY(EditAnywhere, Category = "GPE | Shoot| Animation", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FName headBoneName = "";
#pragma endregion

#pragma  region Components
	
	// Skeletal mesh component
	UPROPERTY(EditAnywhere, Category = "GPE|Components", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		USkeletalMeshComponent* skeletalMesh = nullptr;
	
	//Component that handle the shot in itself
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UGPE_PatternCasterComponent* patternCaster = nullptr;
	
	//The direction in which the pattern will be shot
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UArrowComponent* directionArrow = nullptr;
	
	//SceneComponent used to define where the turret will look at when not looking at the target
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		USceneComponent* targetPoint = nullptr;
	
	//Billboard to select the sceneComponent with ease
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UBillboardComponent* billBoard = nullptr;

	//SceneComponent used to define where the turret shots will start from
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		USceneComponent* shootPoint = nullptr;

	//Billboard to select the sceneComponent with ease
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UBillboardComponent* billboardShootPoint = nullptr;
	
	//Niagara that show where the turret is aiming
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UNiagaraComponent* niagaraAimLaser = nullptr;
	
#pragma endregion
	
	// Waiting for the pattern to finish
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GPE | Pattern", meta = (AllowPrivateAccess = True))
		bool wait = false;
	
	// Data table for the pattern shot every shootDelay
	UPROPERTY(EditAnywhere, Category = "GPE | Pattern", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UP_DT_BaseDataTable* patternDataTable = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UM_DifficultyManager* difficultyManager = nullptr;		

#pragma endregion 

public:
	FORCEINLINE UArrowComponent* DirectionArrow() const{return directionArrow;}
	FORCEINLINE FOnShoot& OnShoot() { return onShoot; }
	FORCEINLINE FOnActivate& OnActivate() { return onActivate; }
	FORCEINLINE FVector ShootStartPosition() const { return shootFromPoint ? shootPoint->GetComponentLocation() : GetActorLocation(); }
	
public:
	AGPE_Shot();

	virtual void EnableGPE() override;
	virtual void DisableGPE() override;
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
	
	UFUNCTION(BlueprintCallable) void SpawnPoolingTrigger();
	UFUNCTION(BlueprintCallable) void Init();
	UFUNCTION(BlueprintCallable) void Shoot();
	UFUNCTION(BlueprintCallable) void LoadShoot(const float& _deltaTime);
	UFUNCTION(BlueprintCallable) bool IsPlayerInRange(const AActor* _target);
	UFUNCTION(BlueprintCallable) void AimAt(const FVector& _position);
	UFUNCTION(BlueprintCallable) void TurretDeath();
	UFUNCTION(BlueprintCallable) void ChangeWait(AP_PatternBase* _pattern);

	UFUNCTION(BlueprintCallable) void PatternShootOnShootEvent();
protected:
	UFUNCTION(BlueprintCallable) virtual bool IsInRange(const FVector& _target,const FVector& _currentLocation,const bool& _useBox);
};