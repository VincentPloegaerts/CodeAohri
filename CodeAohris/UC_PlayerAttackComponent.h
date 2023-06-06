#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "C_PowerOrb.h"
#include "ENUM_PrimaryShootType.h"
#include "ENUM_DifficultyType.h"
#include "IU_PlayerArrow.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "UC_PlayerAttackComponent.generated.h"

class AIU_Camera;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UC_PlayerAttackComponent : public UActorComponent
{
	GENERATED_BODY()

	#pragma region Events
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPowerOrbNotUsed);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnPrimaryShoot"))
	FOnPowerOrbNotUsed onPowerOrbNotUsed;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPrimaryShoot);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnPrimaryShoot"))
		FOnPrimaryShoot onPrimaryShoot = FOnPrimaryShoot();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFXHarpoonShoot);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnFXHarpoonShoot"))
		FOnFXHarpoonShoot onFXHarpoonShoot = FOnFXHarpoonShoot();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFXThornShoot);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnFXThornShoot"))
		FOnFXThornShoot onFXThornShoot = FOnFXThornShoot();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartHarpoonLoadingShoot);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnStartHarpoonLoadingShoot"))
		FOnStartHarpoonLoadingShoot onStartHarpoonLoadingShoot = FOnStartHarpoonLoadingShoot();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartThornLoadingShoot);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,meta = (DisplayName = "FOnStartThornLoadingShoot"))
		FOnStartThornLoadingShoot onStartThornLoadingShoot = FOnStartThornLoadingShoot();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartLoadingShoot);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,meta = (DisplayName = "FOnStartLoadingShoot"))
		FOnStartLoadingShoot onStartLoadingShoot = FOnStartLoadingShoot();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPrimaryShootTimer, float, _current, float, _max);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnPrimaryShootTimer"))
		FOnPrimaryShootTimer onPrimaryShootTimer = FOnPrimaryShootTimer();
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnPrimaryShootAltTimer"))
		FOnPrimaryShootTimer onPrimaryShootAltTimer = FOnPrimaryShootTimer();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnOrbCountChange, float, _count, float, _max, float, _required);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnOrbCountChange"))
		FOnOrbCountChange onOrbCountChange = FOnOrbCountChange();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPowerOrbCanSpawn);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnPowerOrbCanSpawn"))
		FOnPowerOrbCanSpawn onPowerOrbCanSpawn = FOnPowerOrbCanSpawn();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrimaryShootChange, EPrimaryShootType, _type);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnPrimaryShootChange"))
		FOnPrimaryShootChange onPrimaryShootChange = FOnPrimaryShootChange();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerStopShooting);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnPlayerStopShooting"))
		FOnPlayerStopShooting onPlayerStopShooting = FOnPlayerStopShooting();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPowerOrbSpawn,FVector, _doubleGuidedOrb,FVector, _harpoonOrb);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnPowerOrbSpawn"))
		FOnPowerOrbSpawn onPowerOrbSpawn = FOnPowerOrbSpawn();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPowerOrbCollected);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnPowerOrbCollected"))
	FOnPowerOrbCollected onPowerOrbCollected = FOnPowerOrbCollected();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerShootFullyCharged);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnPlayerShootFullyCharged"))
	FOnPlayerShootFullyCharged onPlayerShootFullyCharged = FOnPlayerShootFullyCharged();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDetection, const bool, _enemyDetection);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Enemy Detection"))
	FOnEnemyDetection onEnemyDetection = FOnEnemyDetection();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAlternativeShotCharging, const float, _currentCharge, const float, _maxCharge);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Alternative Shot Charging"))
	FOnAlternativeShotCharging onAlternativeShotCharging = FOnAlternativeShotCharging();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlternativeShotCharged);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Alternative Shot Charged"))
	FOnAlternativeShotCharged onAlternativeShotCharged = FOnAlternativeShotCharged();

	#pragma region AnimationEvents
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSimpleShoot, bool, _isSimpleShooting);
		FOnSimpleShoot onSimpleShoot = FOnSimpleShoot();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThornShoot, bool, _isThornShooting);
		FOnThornShoot onThornShoot = FOnThornShoot();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHarpoonShoot, bool, _isHarpoonShooting);
		FOnHarpoonShoot onHarpoonShoot = FOnHarpoonShoot();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerChargingShoot, bool, _isChargingShoot);
		FOnPlayerChargingShoot onPlayerChargingShoot = FOnPlayerChargingShoot();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHasShootedAndChargingShoot, bool, _onHasShootedAndChargingShoot);
		FOnHasShootedAndChargingShoot onHasShootedAndChargingShoot = FOnHasShootedAndChargingShoot();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShooting, bool, _onShooting);
		FOnShooting onShooting = FOnShooting();

	#pragma endregion

	#pragma endregion

	#pragma region PrimaryShoot

	UPROPERTY(VisibleAnywhere, Category = "Attack values | Primary Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bCanShootAgain = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Attack values | Primary Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bHAsShootedOnce = false;
	
	//Damage
	UPROPERTY(EditAnywhere, Category = "Attack values | Primary Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fBaseDamage = 2.0f;
	
	//Time between two shoot
	UPROPERTY(EditAnywhere, Category = "Attack values | Primary Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fLoadingTime = 2.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Attack values | Primary Shoot", meta = (AllowPrivateAccess = True))
		float fBaseLoadingTime = 2.0f;

	UPROPERTY()
		AIU_Camera* playerCamera = nullptr;

	#pragma endregion

	#pragma region Harpoon

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Harpoon", meta = (AllowPrivateAccess = True))
		int iNumberOfHarpoonNeededToStunBoss = 2;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Harpoon", meta = (AllowPrivateAccess = True))
		float fHarpoonLoadingTime = 4.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Harpoon", meta = (AllowPrivateAccess = True))
		float fHarpoonDamage = 4.0f;

	//duration of the harpoon shoot
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Harpoon", meta = (AllowPrivateAccess = True))
		float fHarpoonShootDuration = 4.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Harpoon", meta = (AllowPrivateAccess = True))
		float fStunDuration = 5.0f;
	
	//Max duration time for the holding shoot in Harpoon Mode
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Harpoon", meta = (AllowPrivateAccess = True))
		float fMaxHarpoonShootTime = 5.0f;
	
	UPROPERTY()
		float fHarpoonTimer = 0.0f;

	#pragma endregion

	#pragma region Double Guided
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Double Guided", meta = (AllowPrivateAccess = True))
		float fDoubleGuidedLoadingTime = 1.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Double Guided", meta = (AllowPrivateAccess = True))
		float fDoubleGuidedDamage = 1.0f;
	
	//duration of the DoubleGuided shoot
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Double Guided", meta = (AllowPrivateAccess = True))
		float fDoubleGuidedDuration = 4.0f;
	
	//Max duration time for the holding shoot in Double Guided Mode
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Double Guided", meta = (AllowPrivateAccess = True))
		float fMaxDoubleGuidedShootTime = 2.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Double Guided", meta = (AllowPrivateAccess = True))
		float fDelayBetweenArrows = 0.5f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Double Guided", meta = (AllowPrivateAccess = True))
		float fArrowRotationSpeed = 5.0f;

	//after this time, the Double Guided arrows no longer rotate to the target
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Double Guided", meta = (AllowPrivateAccess = True))
		float fTimeBeforeArrowRotationStop = 5.0f;

	//Max distance for the line trace to detect an enemy and shoot the arrow on him when double guided 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Attack values | Double Guided", meta = (AllowPrivateAccess = True))
		float fMaxDoubleGuidedEnemyDetectionDistance = 5000.0f;

	UPROPERTY()
		float fDoubleGuidedTimer = 0.0f;

	UPROPERTY()
		float fFirstGuidedArrowDamage = 0.0f;

protected:
	UPROPERTY()
		FTimerHandle doubleGuidedSecondArrowSpawnTimer = FTimerHandle();

	#pragma endregion 
		
	#pragma region Orb
	
	//Min Distance where the orb can drop
	UPROPERTY(EditAnywhere, Category = "Attack values | Orb", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fMinDropRadius = 100.0f;
	
	//Max Distance where the orb can drop
	UPROPERTY(EditAnywhere, Category = "Attack values | Orb", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fMaxDropRadius = 200.0f;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fBaseMaxDropRadius = 200.0f;

	//How much the maxDropRadius is increased with each orb
	UPROPERTY(EditAnywhere, Category = "Attack values | Orb", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fMaxDistanceDropRadiusIncrease = 50.0f;
	
	//Maximum amount of orb that can be carry by the player
	UPROPERTY(EditAnywhere, Category = "Attack values | Orb", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fMaxOrbStack = 10.0f;

#pragma region Power Orb

	//Maximum amount of orb that can be carry by the player
	UPROPERTY(EditAnywhere, Category = "Attack values | Power Orb", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float zSpawnOffset = 100;

#pragma endregion 

	#pragma endregion

	#pragma region Misc
	
	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bCanShoot = false;

	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bShootInput = true;
	
	//To know if the player is holding down the shoot button 
	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bIsShooting = false;

	//Is the player in the Harpoon Shoot State
	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool isInHarpoonShootState = false;
	
	//Is the player in the Thorn Rain State
	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool isInThornRainState = false;

	//Necessary to know if the player is dead so he cant shoot
	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bIsDead = false;

	//Bool to know if the shoot can apply power orb effects
	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bIsChargeSuperiorToPercent = false;

	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bIsPlayerChargingShoot = false;
	
	//To know the current amount of Slow Mo orb
	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		int iCurrentSlowMoOrbCount = 0;
	
	//Max amount of Slow Mo orb
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		int iMaxSlowMoOrbCount = 20;
	
	//To know the current amount of Shooting Mode Orb 
	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		int iCurrentShootingModeOrbCount = 0;
	
	//Orb amount needed to drop the shooting mode orb
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		int iOrbAmountNeededToDropShootingModeOrb = 5;
	
	//To know if the player can shoot again 
	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fCurrentTimer = 0.0f;

	//Minimum percent needed to shoot
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True,ClampMin = 0.0f,ClampMax = 1.0f))
		float fMinimumPercentNeededToShoot = 0.2f;
	
	//Minimum percent needed to apply power orb effect
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True,ClampMin = 0.0f,ClampMax = 1.0f))
		float fPercentNeededToApplyPowerOrbEffect = 0.9f;

	//Max duration time for the holding shoot 
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fMaxShootTime = 5.0f;

	//Minimum percent needed to allow arrow to drop power orb
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True,ClampMin = 0.0f,ClampMax = 1.0f))
		float fPercentNeededToDropEnhancementOrb = 1.0f;

	//Aditionnal %age of MaxShootTime added after the end of Max Shoot Time before shoot
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True,ClampMin = 0.0f,ClampMax = 1.0f))
		float fAdditionnalPercentageTimeAfterMaxShootTime = 0.2f;
	
	UPROPERTY()
		float fBaseMaxShootTime = 5.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fCurrentMultiplierTimer = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float orbSpawnOffset = 500.0f;
	
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float orbForwardSpawnOffset = 300.0f;
	
	//Ground Layer
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Layers", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<TEnumAsByte<EObjectTypeQuery>> groundLayer;

	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Layers", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<TEnumAsByte<EObjectTypeQuery>> enemyLayer;
	
	//Object Layers that player attack can't go through
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Layers", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<TEnumAsByte<EObjectTypeQuery>> blockingLayers;
	
	//Arrow fired by the player
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Arrow To Spawn", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSubclassOf<AIU_PlayerArrow> arrowToSpawn = nullptr;

	//Harpoon fired by the player
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Arrow To Spawn", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSubclassOf<AIU_PlayerArrow> harpoonToSpawn = nullptr;

	//Thorn fired by the player
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Arrow To Spawn", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSubclassOf<AIU_PlayerArrow> thornToSpawn = nullptr;
	
	//Thorn power orb spawned by the player
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Orb To Spawn", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSubclassOf<AActor> thornPowerOrb = nullptr;

	//Harpoon power orb spawned by the player
	UPROPERTY(EditAnywhere, Category = "Attack values | Misc | Orb To Spawn", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSubclassOf<AActor> harpoonPowerOrb = nullptr;

	//Useful for the tutorial
	UPROPERTY()
	bool bHasPlayerCompletelyChargedShot = false;

	UPROPERTY(VisibleAnywhere, Category = "Attack values | Misc | Is in Tuto", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool isInTuto = false;
	
	#pragma endregion 

	#pragma region FX
	
	//NIAGARA ATTRIBUTE
	UPROPERTY(EditAnywhere, Category = "Attack values | FX", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UNiagaraSystem* shootActivationNiagaraSys = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attack values | FX", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UNiagaraSystem* FXOnPlayerWhenHitSomething = nullptr;

	//Useful to debug animations
	UPROPERTY(EditAnywhere, Category = "Attack values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool stopRotationForDebug = false;
	
	UPROPERTY()
		bool bHasShootFXAlreadySpawned = false;
	
	UPROPERTY()
		bool bIsOpeningChest = false;				//to prevent the player from rotate in the camera forward when opening chest

	UPROPERTY()
		bool bIsAimingAtEnemy = false;				//to know if the player aim at an enemy, useful in tuto

	UPROPERTY()		//to know if orbs have been used or not by the player
		bool bOrbUsed = false;
	#pragma endregion 

	
#pragma region Tutorial

	// To know if the player can pass to the next tuto state 
	UPROPERTY(VisibleAnywhere, Category = "Player values | Tutorial", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	bool bIsTutoConditionValid = false;

	// In the tuto, some state needs a number condition to continue, this float is used to check that condition
	UPROPERTY(VisibleAnywhere, Category = "Player values | Tutorial", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fCurrentTutoIteration = 0;

#pragma endregion
public:
	#pragma region Getters

	FORCEINLINE bool IsPlayerChargingShoot() const { return bIsPlayerChargingShoot; }
	FORCEINLINE float GetMinDropRadius() const { return fMinDropRadius; }
	FORCEINLINE float GetMaxDropRadius() const { return fMaxDropRadius; }
	FORCEINLINE int GetCurrentSlowMoOrbCount() const { return iCurrentSlowMoOrbCount; }
	
	FORCEINLINE FOnPrimaryShoot& OnPrimaryShoot() { return onPrimaryShoot; }
	FORCEINLINE FOnPrimaryShootTimer& OnPrimaryShootTimer() { return onPrimaryShootTimer; }
	FORCEINLINE FOnPrimaryShootTimer& OnPrimaryShootAltTimer() { return onPrimaryShootAltTimer; }
	FORCEINLINE FOnOrbCountChange& OnOrbCountChange() { return onOrbCountChange; }
	FORCEINLINE FOnPowerOrbCanSpawn& OnPowerOrbCanSpawn() { return onPowerOrbCanSpawn; }
	FORCEINLINE FOnPrimaryShootChange& OnPrimaryShootChange() { return onPrimaryShootChange; }
	FORCEINLINE FOnStartLoadingShoot& OnStartLoadingShoot() { return onStartLoadingShoot; }
	FORCEINLINE FOnStartHarpoonLoadingShoot& OnStartHarpoonLoadingShoot() { return onStartHarpoonLoadingShoot; }
	FORCEINLINE FOnStartThornLoadingShoot& OnStartThornLoadingShoot() { return onStartThornLoadingShoot; }
	FORCEINLINE FOnPlayerChargingShoot& OnPlayerChargingShoot() { return onPlayerChargingShoot; }
	FORCEINLINE FOnPlayerStopShooting& OnPlayerStopShooting() { return onPlayerStopShooting; }
	FORCEINLINE FOnShooting& OnShooting() { return onShooting; }
	FORCEINLINE FOnPowerOrbSpawn& FOnPowerOrbSpawn() { return onPowerOrbSpawn; }
	FORCEINLINE FOnPowerOrbCollected& FOnPowerOrbCollected() { return onPowerOrbCollected; }
	FORCEINLINE FOnPlayerShootFullyCharged& OnPlayerShootFullyCharged() { return onPlayerShootFullyCharged; }
	FORCEINLINE FOnEnemyDetection& OnEnemyDetection() { return onEnemyDetection; }
	FORCEINLINE FOnAlternativeShotCharging& OnAlternativeShotCharging() { return onAlternativeShotCharging; }
	FORCEINLINE FOnAlternativeShotCharged& OnAlternativeShotCharged() { return onAlternativeShotCharged; }
	FORCEINLINE FOnPowerOrbNotUsed& OnPowerOrbNotUsed() { return onPowerOrbNotUsed; }
	
	FORCEINLINE bool GetIsInTuto() const { return isInTuto; }
	FORCEINLINE int GetMaxOrbCount() const { return iMaxSlowMoOrbCount; }
	
	FORCEINLINE bool GetIsTutoConditionValid() const { return bIsTutoConditionValid; }
	FORCEINLINE float GetCurrentTutoIterationNumber() const { return fCurrentTutoIteration; }
	FORCEINLINE void ResetTutoCondition() { bIsTutoConditionValid = false;fCurrentTutoIteration = 0; }
	
	FORCEINLINE void SetSlowMoCountToZero() { iCurrentSlowMoOrbCount = 0; }
	
	FORCEINLINE bool GetIsOpeningChest() const {return bIsOpeningChest; }
	FORCEINLINE bool GetIsAimingAtEnemy() const {return bIsAimingAtEnemy; }
	
	
	//Animation 
	FORCEINLINE FOnSimpleShoot& OnSimpleShoot() { return onSimpleShoot; }
	FORCEINLINE FOnHarpoonShoot& OnHarpoonShoot() { return onHarpoonShoot; }
	FORCEINLINE FOnThornShoot& OnThornShoot() { return onThornShoot; }
	FORCEINLINE FOnHasShootedAndChargingShoot& OnHasShootedAndChargingShoot() { return onHasShootedAndChargingShoot; }

	#pragma endregion

	#pragma region Setters

	FORCEINLINE void SetOrbUsed(bool _used) { bOrbUsed = _used; }
	FORCEINLINE void SetIsInTuto(bool _status) {isInTuto = _status; }
	FORCEINLINE void SetIsOpeningChest(bool _status) {bIsOpeningChest = _status; }
	FORCEINLINE void SetIsTutoConditionValid(bool _status) {bIsTutoConditionValid = _status; }
	FORCEINLINE void SetHarpoonShootState(bool _status)
	{
		isInHarpoonShootState = _status;
		if (_status)
		{
			onPrimaryShootChange.Broadcast(HARPOON);
		}
	}
	FORCEINLINE void SetThornRainState(bool _status)
	{
		isInThornRainState = _status;
		if (_status)
		{
			onPrimaryShootChange.Broadcast(THORN);
		}
	}
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetCanShoot(bool _status) { bCanShoot = _status; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetShootInput(bool _status) { bShootInput = _status; }
	FORCEINLINE void SetIsShooting(bool _status) { bIsShooting = _status; }
	FORCEINLINE void SetIsDead(bool _status) { bIsDead =_status; }
	FORCEINLINE void ResetHarpoonTimer() { fHarpoonTimer = 0.0f; }
	FORCEINLINE void ResetThornTimer() { fDoubleGuidedTimer = 0.0f; }
	FORCEINLINE void SetPlayerCamera(AIU_Camera* _cam) { playerCamera = _cam; }
	FORCEINLINE void SetBothShootDuration(float _newHarpoonDuration, float _newDoubleGuidedDuration)
	{
		fHarpoonShootDuration = _newHarpoonDuration,
		fDoubleGuidedDuration = _newDoubleGuidedDuration;
	}

	FORCEINLINE bool GetPlayerChargedShotStatus() {return bHasShootFXAlreadySpawned;}
	FORCEINLINE void IncreaseCurrentSlowMoOrbCount(int _valueToAdd) {iCurrentSlowMoOrbCount += _valueToAdd;}
	FORCEINLINE void ResetShootState()
	{
		isInHarpoonShootState = false;
		isInThornRainState = false;
		iCurrentShootingModeOrbCount = 0;
		onPrimaryShootChange.Broadcast(NORMAL);
		fMaxShootTime = fBaseMaxShootTime;
	}
	
	FORCEINLINE void SetSlowMoAtQuarter() { iCurrentSlowMoOrbCount = 10; }		//Only for the tuto, to give the player a little bit of slow mo when fighting tuto boss


	UFUNCTION(BlueprintCallable) FORCEINLINE void SetSlowMoCountToMax() {iCurrentSlowMoOrbCount = iMaxSlowMoOrbCount;}
	UFUNCTION(BlueprintCallable) FORCEINLINE void AddSlowMowMaxCount(int _value) {iMaxSlowMoOrbCount += _value;}
	UFUNCTION(BlueprintCallable) FORCEINLINE void ResetConditionIfTutoDeath() { bIsTutoConditionValid = false; fCurrentTutoIteration = 0;}

	#pragma endregion 

public:	
	UC_PlayerAttackComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) void CheckEnemyDetection();

	FVector GetSpawnPoint() const;
	FVector GetMaxDistancePoint(const TSubclassOf<AIU_PlayerArrow>& _arrowType) const;
	AIU_PlayerArrow* SpawnArrow(const TSubclassOf<AIU_PlayerArrow>& _arrowType) const;
	
public:
	UFUNCTION(BlueprintCallable) void IncreaseTimer();
	UFUNCTION(BlueprintCallable) void StopShooting();
	UFUNCTION(BlueprintCallable) void IncreasedCurrentSlowMoOrbCount();
	UFUNCTION(BlueprintCallable) void IncreasedCurrentShootingModeCount();
	UFUNCTION(BlueprintCallable) void Shoot();
	UFUNCTION(BlueprintCallable) void ShootHarpoon();
	UFUNCTION(BlueprintCallable) void ShootDoubleGuided();
	UFUNCTION(BlueprintCallable) void SpawnDoubleGuidedSecondArrow(bool _canSpawnOrb, AActor* _target, bool _isInTuto);
	UFUNCTION(BlueprintCallable) void SetCurrentShootingState();
	UFUNCTION(BlueprintCallable) void SpawnPowerOrb();
	UFUNCTION(BlueprintCallable) void DestroySecondPowerOrb(AC_PowerOrb* _otherOrb);
	UFUNCTION(BlueprintCallable) void IncreasePowerShootTimer();
	UFUNCTION(BlueprintCallable) bool GetValidSlowMoOrbCount(int _orbConsumed);
	UFUNCTION(BlueprintCallable) void IncreaseShootTimer();
	UFUNCTION(BlueprintCallable) void CheckHoldingTime();
	UFUNCTION(BlueprintCallable) void StartShooting();
	UFUNCTION(BlueprintCallable) void SetCurrentMaxShootTime();
	UFUNCTION(BlueprintCallable) void SetGoodFX();
	UFUNCTION(BlueprintCallable) void SetGoodAnimation();
	UFUNCTION(BlueprintCallable) void StopAnimation();
	UFUNCTION(BlueprintCallable) void ResetShoot();
	UFUNCTION(BlueprintCallable) void SetGoodRotation();
	UFUNCTION(BlueprintCallable) AActor* GetClosestCritPoint(AB_Boss* boss);
	UFUNCTION(BlueprintCallable) void PlayHitFXOnPlayer();
	UFUNCTION(BlueprintCallable) void IncreaseAndCheckTutoCondition(); // Useful for the DashCancelShoot tuto state
	UFUNCTION(BlueprintCallable) void OnOrbDestroyed(AActor* _orb);
};