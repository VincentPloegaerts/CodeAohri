#pragma once
#include "CoreMinimal.h"
#include "GPE_PatternCasterComponent.h"
#include "GPE_PlatformWithHealth.h"
#include "STRUCT_SpawnSettings.h"
#include "T_BossTutoAnimInstance.h"
#include "T_TutoLogic.h"
#include "T_BossTuto.generated.h"

class AP_PoolingTriggerSphere;
class UBillboardComponent;

UCLASS()
class INSIDEUS_API AT_BossTuto : public AGPE_PlatformWithHealth
{
	GENERATED_BODY()
#pragma region Properties
#pragma region Shoot
	//Delay between 2 patterns, does not take account of the pattern fire rate
	UPROPERTY(EditAnywhere, Category = "Tuto|Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float shootDelay = 0.5f;
	//Float increasing to reach shootDelay and shoot a new pattern when needed
	UPROPERTY(VisibleAnywhere, Category = "Tuto|Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float shootingTime = 0.0f;
	// Waiting for the pattern to finish
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tuto|Shoot", meta = (AllowPrivateAccess = True))
		bool wait = false;
	//AActor to check for range and act in accordance, if set to none the player in M_Gamemode will be chosen
	UPROPERTY(EditAnywhere, Category = "Tuto|Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		AActor* target = nullptr;
	//Vertical offset for the self guided projectile
	UPROPERTY(EditAnywhere, Category = "Tuto|Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fSelfGuidedOffset = 100;

	UPROPERTY()
		bool canShoot = true;
#pragma endregion 
	
#pragma region Pattern
	// Data table for the pattern shot every shootDelay
	UPROPERTY(EditAnywhere, Category = "Tuto|Pattern", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UP_DT_BaseDataTable* patternDataTable = nullptr;
	UPROPERTY(EditAnywhere, Category = "Tuto|Pattern", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UP_DT_BaseDataTable* patternWallDataTable = nullptr;
#pragma endregion 
	
#pragma region Pooling
	//Pooling Trigger to spawn
	UPROPERTY(EditAnywhere, Category = "Tuto|Pooling",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		TSubclassOf<AP_PoolingTriggerSphere> poolingTriggerType;
	UPROPERTY(VisibleAnywhere, Category = "Tuto|Pooling",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		AP_PoolingTriggerSphere* poolingTrigger = nullptr;
	UPROPERTY(EditAnywhere, Category = "Tuto|Pooling",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float rangePulling = 10000.0f;
	//Suppose time to wait for all projectile to be pooled. Use for PoolingTrigger Disable function
	UPROPERTY(EditAnywhere, Category = "Tuto|Pooling")
		float fWaitProjectileClear = 15.0f;

#pragma endregion

	UPROPERTY()
		UWorld* world;
	UPROPERTY()
		AM_PatternsTickManager* patternsTickManager = nullptr;
	UPROPERTY()
		AM_FeedbackManager* feedbackManager = nullptr;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
		AP_PatternBase* currentPattern = nullptr;
	UPROPERTY()
		FTimerHandle timerClear = FTimerHandle();
	UPROPERTY()
		AT_TutoLogic* tutoLogic = nullptr;
	UPROPERTY()
		float currentPlayerLife = 0;
	UPROPERTY(EditAnywhere, Category = "Attack values | FX", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UNiagaraSystem* stunNiagaraSys = nullptr;
	UPROPERTY(EditAnywhere, Category = "Attack values | FX", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UNiagaraSystem* normalShootNiagaraSys = nullptr;
	UPROPERTY(EditAnywhere, Category = "Attack values | FX", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UNiagaraSystem* wallShootNiagaraSys = nullptr;

	
	// In the tuto, some state needs a number condition to continue, this float is used to check that condition
	UPROPERTY(VisibleAnywhere, Category = "Tuto", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fCurrentConditionIteration = 0;


#pragma region Events
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShoot);
	UPROPERTY(BlueprintCallable,BlueprintAssignable,EditAnywhere,meta = (AllowPrivateAccess = True))
		FOnShoot onShoot;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileClearTuto);
	UPROPERTY(BlueprintCallable,BlueprintAssignable,EditAnywhere,meta = (AllowPrivateAccess = True))
		FOnProjectileClearTuto onProjectileClearTuto;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTutoBossStun);
	UPROPERTY(BlueprintCallable, BlueprintAssignable, EditAnywhere, meta = (AllowPrivateAccess = True))
		FOnTutoBossStun onTutoBossStun;

#pragma endregion

#pragma region Components

	//SceneComponent used to define where the turret shots will start from
	UPROPERTY(EditAnywhere,Category = "Tuto|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		USceneComponent* shootPoint = nullptr;
	//Billboard to select the sceneComponent with ease
	UPROPERTY(EditAnywhere,Category = "Tuto|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UBillboardComponent* billboardShootPoint = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		USkeletalMeshComponent* skeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UT_BossTutoAnimInstance* bossTutoAnimInstance = nullptr;

#pragma endregion
#pragma endregion
	
public:
	FORCEINLINE FOnShoot& OnShoot() {return onShoot;}
	FORCEINLINE FOnTutoBossStun& OnTutoBossStun() {return onTutoBossStun;}
	FORCEINLINE FOnProjectileClearTuto& OnProjectileClearTuto() {return onProjectileClearTuto;}
	FORCEINLINE float GetBossShootDelay() const {return shootDelay;}
	FORCEINLINE UT_BossTutoAnimInstance* GetTutoBossAnimInstance() const {return bossTutoAnimInstance;}
	FORCEINLINE UNiagaraSystem* GetStunNiagaraSystem() const {return stunNiagaraSys;}
	FORCEINLINE void SetBossShootDelay(float _value)  {shootDelay = _value;}
	FORCEINLINE void ResetCurrentTutoIteration() { fCurrentConditionIteration = 0; }
public:
	AT_BossTuto();
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable) void SpawnPoolingTrigger();
	UFUNCTION(BlueprintCallable) void ChangeWait(AP_PatternBase* _pattern);
	UFUNCTION(BlueprintCallable) void Init();
	UFUNCTION(BlueprintCallable)void CastPattern(UP_DT_BaseDataTable* _dataTable);
	UFUNCTION(BlueprintCallable) void PatternBehaviorCompleted(AP_PatternBase* _pattern);
	UFUNCTION(BlueprintCallable) void ProjectileClear();
	UFUNCTION(BlueprintCallable) void CheckPlayerLife();
	UFUNCTION(BlueprintCallable) void CheckForSlowMoUse();
	UFUNCTION(BlueprintCallable) void RotateToPlayer();

	void DuplicatePattern(const AP_PatternBase* _initialPattern, const FVector& _initialLocation,  UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack);
	void DuplicatePatternCenter(AP_PatternBase* _initialPattern, const FVector& _initialLocation, UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack);
	AActor* SpawnPatternDuplicata(const TSubclassOf<AP_PatternBase>& _patternType, const FVector& _location, const FRotator& _rotation, UP_DT_BaseDataTable* _dataTable, const FTripleDeluxeFeedBack* _overridedFeedBack);
	

public:
	UFUNCTION(BlueprintCallable) void Shoot();
	UFUNCTION(BlueprintCallable) void LoadShoot(const float& _deltaTime);
	UFUNCTION(BlueprintCallable) void LoadWallShoot(const float& _deltaTime);
	UFUNCTION(BlueprintCallable) void LoadNormalShoot(const float& _deltaTime);
	UFUNCTION(BlueprintCallable) void ShootSpecificPattern(UP_DT_BaseDataTable* patternToShoot);
	UFUNCTION(BlueprintCallable) void LoadShootSpecificPattern(const float& _deltaTime,UP_DT_BaseDataTable* patternToShoot);
	UFUNCTION(BlueprintCallable) void LoadRandomShootPattern(const float& _deltaTime);
	UFUNCTION(BlueprintCallable) void StopFX(UNiagaraComponent* niagaraComp);
};
