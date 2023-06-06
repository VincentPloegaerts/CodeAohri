#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"

#include "ENUM_CameraType.h"
#include "ENUM_DifficultyType.h"

#include "STRUCT_PlayerDatas.h"

#include "STRUCT_NiagaraComponents.h"
#include "STRUCT_DifficultyData.h"
#include "STRUCT_ArenaEnterData.h"

#include "C_PlayerAnimInstance.h"

#include "IU_Player.generated.h"

class AHUD_Game;
class US_GameSave;
class UM_GameInstance;
class AM_GameMode;

class AIU_Camera;
class AC_Bow;

class UT_ProjectileDetectionSphere;

class UIU_HealthSystemComponent;
class UC_PlayerAttackComponent;
class UC_SlowMotionComponent;
class UC_IronModeComponent;
class UC_RespawnComponent;
class UC_RegenComponent;
class UC_CollectibleComponent;

class UUI_PlayerWidget;

class UC_PlayerAnimInstance;
class UIU_PlayerFSMBrain;

class AA_Arena;
class AB_Boss;

UCLASS()
class INSIDEUS_API AIU_Player : public APawn
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInterract);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsInArena, const bool, _isInArena);

	#pragma region Events
private:
	UPROPERTY()	FOnIsInArena onIsInArena = FOnIsInArena();
	UPROPERTY()	FOnInterract onInterract = FOnInterract();
	#pragma endregion Events

	#pragma region Components
	
protected:
	// Capsule component
	UPROPERTY(EditAnywhere, Category = "Player values | Components", BlueprintReadWrite)
		UCapsuleComponent* capsuleComponent = nullptr;

	// Attack component
	UPROPERTY(EditAnywhere, Category = "Player values | Components", BlueprintReadWrite)
		UC_PlayerAttackComponent* attackComp = nullptr ;
	
	// Skeletal mesh component
	UPROPERTY(EditAnywhere, Category = "Player values | Components", BlueprintReadWrite)
		USkeletalMeshComponent* skeletalMesh = nullptr;

	// Health Components
	UPROPERTY(EditAnywhere, Category = "Player values | Components", BlueprintReadWrite, meta = (AllowPrivateAccess=True))
		UIU_HealthSystemComponent* healthComponent = nullptr;

	// SlowMotion Component
	UPROPERTY(EditAnywhere, Category = "Player values | Components", BlueprintReadWrite, meta = (AllowPrivateAccess=True))
		UC_SlowMotionComponent* slowMoComponent = nullptr;
	
private:
	// Player movement component
	UPROPERTY(EditAnywhere, Category = "Player values | Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		class UIU_PlayerMovementComponent* movementComponent = nullptr;

	// Player movement component
	UPROPERTY(EditAnywhere, Category = "Player values | Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UIU_PlayerFSMBrain* movementFsm = nullptr;

	// Respawn Component
	UPROPERTY(EditAnywhere, Category = "Player values | Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UC_RespawnComponent* respawnComp = nullptr;
		
	// Collectible Component
	UPROPERTY(EditAnywhere, Category = "Player values | Components", BlueprintReadWrite, meta = (AllowPrivateAccess=True))
		UC_CollectibleComponent* collectibleComponent = nullptr;

	// Anim instance
	UPROPERTY(EditAnywhere, Category = "Player values | Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UC_PlayerAnimInstance* animInstance = nullptr;

	// Mesh indicating where player's gonna land
	UPROPERTY(EditAnywhere, Category = "Player values | Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UStaticMeshComponent* jumpIndicatorMesh = nullptr;

	#pragma endregion

	#pragma region Datas

	// Struct wich contains all player's datas
	UPROPERTY(VisibleAnywhere, Category = "Player values | Datas")
		FPlayerDatas playerDatas = FPlayerDatas();

	// All player's niagara components
	UPROPERTY(EditAnywhere, Category = "Player values | Datas", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FNiagaraComps niagraComps = FNiagaraComps();

	UPROPERTY(VisibleAnywhere,Category = "Player values | Save")
		US_GameSave* gameSave = nullptr;

	#pragma endregion

	#pragma region Bow

	// Player bow type
	UPROPERTY(EditAnywhere, Category = "Player values | Bow", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSubclassOf<AC_Bow> bowToSpawn = nullptr;

	// Player bow pointer
	UPROPERTY(VisibleAnywhere, Category = "Player values | Bow", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		AC_Bow* currentBow = nullptr;

	#pragma endregion

	#pragma region Arrow

	// Player arrow type
	UPROPERTY(EditAnywhere, Category = "Player values | Bow", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<AActor> arrowToSpawn = nullptr;

	// Player arrow pointer
	UPROPERTY(VisibleAnywhere, Category = "Player values | Bow", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AActor* currentArrow = nullptr;
	#pragma endregion

	#pragma region Tutorial

	// Allow to skip the tutorial
	UPROPERTY(VisibleAnywhere, Category = "Player values | Tutorial", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bCanSkipTutorial = false;

	#pragma endregion

	#pragma region UI

	// Player widget
	UPROPERTY(VisibleAnywhere, Category = "Player values | UI")
		UUI_PlayerWidget* playerWidget = nullptr;

	#pragma endregion

	#pragma region Arena

	// Player is in arena status
	UPROPERTY(VisibleAnywhere, Category = "Player values | Arena")
		bool bIsInArena = false;

	UPROPERTY(EditAnywhere, Category = "Player values | Arena")
		TArray<FArenaEnterData> arenaEnters = TArray<FArenaEnterData>();

	//Only useful to detect the Arena Block actor
	UPROPERTY(EditAnywhere, Category = "Player values | Interaction", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TArray<TEnumAsByte<EObjectTypeQuery>> interactionLayer;
	UPROPERTY(EditAnywhere, Category = "Settings Values | Interaction", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float interactionRange = 250;
	#pragma endregion

	#pragma region Indicator

	// Range mesh indicator for the jump
	UPROPERTY(EditAnywhere, Category = "Player values | Indicator", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float meshIndicatorOffset = 5.0f;
	
	// Range mesh indicator for the jump
	UPROPERTY(EditAnywhere, Category = "Player values | Indicator", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FVector rangeMeshIndicator = FVector(0.0f);

	#pragma endregion

	#pragma region Difficulty

	// Allow player to not die at the difficulty init
	UPROPERTY(VisibleAnywhere, Category = "Player values | Difficulty")
		bool bAlreadyInit = false;
	
	// Current difficulty (for giving it to PlayerArrows)
	UPROPERTY(VisibleAnywhere, Category = "Player values | Difficulty")
		TEnumAsByte<EDifficultyType> currentDifficulty = DIFFICULTY_BORN;

	#pragma endregion

	#pragma region Debugs

	UPROPERTY(EditAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool calculateTravelledDistance = false;
	
	UPROPERTY(EditAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool useJumpMetricsDebug = false;

	UPROPERTY(VisibleAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool isMaxHeight = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool isStartVectorSaved = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool isEndvectorSaved = false;
	
	UPROPERTY(EditAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool isPersistent = true;

	UPROPERTY(VisibleAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool isStartTravelledVectorSaved = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool isEndTravelledVectorSaved = false;
	
	UPROPERTY(EditAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True, EditCondition = "!isPersistent", EditConditionHides))
		float debugStayTime = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True, EditCondition = "calculateTravelledDistance", EditConditionHides))
		float distancePrecision = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float jumpDistanceOffset = -50.0f;

	UPROPERTY(VisibleAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float totalTravelled = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float msTimer = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FVector maxHeightVector = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FVector startDistanceVector = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FVector endDistanceVector = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FVector startTravelledDistanceVector = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, Category = "Player values | Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FVector endTravelledDistanceVector = FVector::ZeroVector;

	#pragma endregion

	#pragma region Pointers
	
	UPROPERTY()
		UM_GameInstance* gameInstance = nullptr;
	
	UPROPERTY()
		UWorld* world = nullptr;
	
	
	FTimerManager* TimerManager = nullptr;

	UPROPERTY()
		AM_GameMode* gameMode = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		AIU_Camera* camera = nullptr;

	#pragma endregion

	#pragma region Animation
	
	// Time before the AFK animation starts playing
	UPROPERTY(EditAnywhere, Category = "Animation | Time before AFK animation", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float timeBeforeAfkAnimation = 10.0f;
	
	UPROPERTY()
	float currentTimeSinceLastInput = 0.0f;

	UPROPERTY()
	bool hasDoneAFKAnimation = false;

	UPROPERTY(EditAnywhere, Category = "Animation", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fNewAnimPlayRate = 1.0f;

	#pragma endregion

	//For footstep sound event when on water trigger
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool isInsideFTSPTrigger = false;

	UPROPERTY(EditAnywhere, Category = "Player | Settings")
	int iMaxHitAmount = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | Settings",meta = (AllowPrivateAccess = True))
	float fHitDecayTime = 5;
	UPROPERTY()
	int iHitAmount = 0;
	
	UPROPERTY()
	bool bInteracting = false;
	
	UPROPERTY()
	FTimerHandle TimerHit;
	UPROPERTY()
	AHUD_Game* hud = nullptr;
	UPROPERTY()
	class UUI_PauseMenuWidget* pauseWidget = nullptr;
	
public:
	AIU_Player();

	#pragma region Setters

public:
	UFUNCTION(BlueprintCallable) FORCEINLINE void ToggleFreePass() { bCanSkipTutorial = !bCanSkipTutorial; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetIsInsideFTSPTigger(bool _status) { isInsideFTSPTrigger = _status; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetIsIntoArena()
	{
		bIsInArena = true;
		onIsInArena.Broadcast(true);
	}
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetIsOutFromArena()
	{
		bIsInArena = false;
		onIsInArena.Broadcast(false);
	}
	UFUNCTION(BlueprintCallable) FORCEINLINE void TeleportInFrontOfArena(const int _arenaIndex)
	{
		if (arenaEnters.Num() < _arenaIndex) return;
		UE_LOG(LogTemp, Warning, TEXT("Go to arena : %d"), _arenaIndex);
		const FArenaEnterData& _enterData = arenaEnters[_arenaIndex - 1];
		SetActorLocationAndRotation(_enterData.enterOfArenaLocation, _enterData.enterOfArenaRotation);
	}
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetInteractingState(const bool& _interacting) { bInteracting = _interacting; }

	#pragma endregion

	#pragma region Getters
	
public:
	FORCEINLINE bool CanSkipTutorial() const { return bCanSkipTutorial; }
	FORCEINLINE bool IsInArena() const { return bIsInArena; }
	
	FORCEINLINE FOnIsInArena& OnIsInArena() { return onIsInArena; }
	FORCEINLINE FOnInterract& OnInterract() { return onInterract; }

	FORCEINLINE AIU_Camera* GetCamera() const { return camera; }
	FORCEINLINE const float& GetInteractionRange() const { return interactionRange; }
	FORCEINLINE const TEnumAsByte<EDifficultyType>& GetDifficulty() const { return currentDifficulty; }
	FORCEINLINE UCapsuleComponent* CapsuleComponent() const {return capsuleComponent;}
	FORCEINLINE UIU_PlayerMovementComponent* MovementComponent() const { return movementComponent; }
	FORCEINLINE UIU_PlayerFSMBrain* MovementFSM() const { return movementFsm; }
	FORCEINLINE UC_PlayerAttackComponent* GetCharacterAttackComponent() const { return attackComp; }
	FORCEINLINE UIU_HealthSystemComponent* HealthComponent() const { return healthComponent; }
	FORCEINLINE UC_RespawnComponent* RespawnComponent() const { return respawnComp; }
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const { return skeletalMesh; }
	FORCEINLINE UC_PlayerAnimInstance* GetPlayerAnimInstance() const { return animInstance; }
	UFUNCTION() FORCEINLINE UC_SlowMotionComponent* GetSlowMotionComponent() const { return slowMoComponent; }

	#pragma endregion
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION() void Hit(float _null);
	UFUNCTION() void ResetHit();
	UFUNCTION() void ForceResetHit();
protected:
	UFUNCTION(BlueprintImplementableEvent) void OnPlayerHit(int _hit);
private:

	#pragma region Init

	// Init pointers
	UFUNCTION() void Init();

	// Init player UI
	void InitUI();
	
	// Init event on his components
	void InitEvents();
	
	// Register to gameMode during begin play
	void RegisterToGameMode();

	// Init arena events
	UFUNCTION(BlueprintCallable) void InitBoss(AB_Boss* _boss);

	// On boss death, reset player life
	UFUNCTION(BlueprintCallable) void InitBossDeath(AB_Boss* _boss);

	// Spawn player bow
	UFUNCTION(BlueprintCallable) void SpawnBow();

	#pragma endregion
	
	#pragma region Debug
	
	UFUNCTION(BlueprintCallable) void JumpMetrics();
	UFUNCTION(BlueprintCallable) void TravelledDistanceMetrics();
	UFUNCTION(BlueprintCallable) void ShowMaxHeight(const FVector& maxHeight);
	UFUNCTION(BlueprintCallable) void ShowMaxLength();
	UFUNCTION(BlueprintCallable) void TravelledDistanceTimer();
	UFUNCTION(BlueprintCallable) void msDistanceTimer();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Player values | Debug") void ClearDebugs();
	UFUNCTION(BlueprintCallable) void StartBoss();
	UFUNCTION(BlueprintCallable) void KillBoss();

	#pragma endregion
	
	#pragma region Niagara
	
	UFUNCTION(BlueprintCallable) void NiagaraCharaHit(float _damage);
	UFUNCTION(BlueprintCallable) void NiagaraDeathFall(float _damage, float _actualHealth);

	#pragma endregion

	#pragma region Jump
	
	UFUNCTION(BlueprintCallable) void SetJumpIndicatorPosition();
	UFUNCTION(BlueprintCallable) void ChangeIndicatorState();

	#pragma endregion

	#pragma region Difficulty
	
	// Init difficulty changement event
	UFUNCTION(BlueprintCallable) void InitDifficulty();

	// Change behavior depending on the new difficulty
	UFUNCTION(BlueprintCallable) void UpdateOnDifficultyChanged(EDifficultyType _difficulty, const FDifficultyData& _difficultyData);

#pragma endregion

	#pragma region Status
	
public:
	void SetPlayerStatus(bool _status, bool _isDead);
	void SetPlayerInputs(bool _status) const;
	
private:
	void ResetPlayerStatus(const FVector& _direction) const;

	#pragma endregion

	#pragma region Tutorial
	
public:
	void ApplySlow(float _slowFactor) const;
	void ShowArrow();
	void HideArrow();
	void ShowBow();
	void HideBow();

	#pragma endregion

	#pragma region Public
	
public:
	UFUNCTION(BlueprintCallable) void AddExtraLife(float _extraLife);
	UFUNCTION(BlueprintCallable) void AddSlowMotion(int _extraSlowMo);
	
	
	// Set the properties from UE "Simulate physics"
	UFUNCTION(BlueprintCallable) void SetSimulatePhysics(bool _status) const;
	
	// Spawn and init camera from cameraManager and bind inputs
	UFUNCTION(BlueprintCallable) void InitCameraByType(const TEnumAsByte<ECameraType>& _cameraType);

	#pragma endregion

	#pragma region Animation

	UFUNCTION() void ResetAfkTimeForAnimation();

	#pragma endregion

	#pragma region Interaction
private:
	UFUNCTION(BlueprintCallable) void TryInteract();
public:
	UFUNCTION(BlueprintCallable) void StartInteraction();
	UFUNCTION(BlueprintCallable) void StopInteraction();
	
	UFUNCTION() void BroadcastInteract();

#pragma endregion Interraction
	
	UFUNCTION() void LoadLevelOnReloadSave();
	UFUNCTION() void SetPlayerAfterLoad();
};