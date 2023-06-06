#pragma once

#include "CoreMinimal.h"
#include "ENUM_BossPhase.h"
#include "ENUM_BossSpecialAnimState.h"
#include "STRUCT_ProjectileSettings.h"
#include "GameFramework/Pawn.h"
#include "B_Boss.generated.h"

class AM_CinematicManager;
class AGPE_SaveTrigger;
class AM_WorldTimeManager;
class UB_BossSpecialAnim;
class UC_RespawnComponent;
class UB_BossMovement;
class UIU_HealthSystemComponent;
class UP_PatternCaster;
class AIU_PunchingBall;
class AB_AIBoss;
class AM_GameMode;
class AA_Arena;
class AIU_Player;
class UM_DifficultyManager;
class UCapsuleComponent;
class UB_BossAnim;
class UB_DT_CritPointDataTable;
class AB_BossCritPoint;
class AM_BlackBoardManager;

UCLASS()
class INSIDEUS_API AB_Boss : public APawn
{
	GENERATED_BODY()

	friend class AM_BlackBoardManager;
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossMiddleLifeAnimStart);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossMiddleLifeAnimEnd);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossStunned);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossStunnedEnd);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossCanBeStun);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossActivated);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossReset);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDead);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossDeadCinematic, const EBossPhase&, _bossPhase);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossInvincibilityStart);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossInvincibilityEnd);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInGround);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutGround);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartDash);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndDash);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickAnim, const float&, _deltaTime);

protected:
	UPROPERTY(EditAnywhere, Category = "Boss values", BlueprintReadWrite)
	USkeletalMeshComponent* skeletalMeshComponent = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = "Boss values")
	UIU_HealthSystemComponent* healthComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "Boss values")
	UP_PatternCaster* patternCaster = nullptr;
	UPROPERTY(EditAnywhere, Category = "Boss values")
	UB_BossMovement* bossMovement = nullptr;
	UPROPERTY(EditAnywhere, Category = "Boss values")
	UCapsuleComponent* capsuleCollider = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Boss values")
	UB_BossAnim* bossAnimInstance = nullptr;
	UPROPERTY(EditAnywhere, Category = "Boss values")
	UB_BossSpecialAnim* bossSpecialAnim = nullptr;
	UPROPERTY(EditAnywhere, Category = "Boss values")
	UB_DT_CritPointDataTable* critPointDataTable = nullptr;
	UPROPERTY(EditAnywhere, Category = "Boss values")
	TSoftObjectPtr<AGPE_SaveTrigger> saveTriggerRefEndBoss;
	
	UPROPERTY(EditAnywhere, Category = "Boss values")
	TEnumAsByte<EBossPhase> bossPhase = EBossPhase::UN;
	
	UPROPERTY(EditAnywhere, Category = "Boss values")
	bool bCanResetHealth = true;
	
	UPROPERTY(EditAnywhere, Category = "Boss values", BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	float cameraDelayDeath = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "Boss values", meta = (ClampMin = "0.01"))
	float animDeathDelay = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Boss values", meta = (ClampMin = "0.01"))
	float cinematicDeathDelay = 1.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Boss values")
	bool bIsMidLifeAnimPlaying = false;
	UPROPERTY(EditAnywhere, Category = "Boss values", BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	bool bIsMidLife = false;
	
	
	UPROPERTY(VisibleAnywhere, Category = "Boss values| Stun")
	bool bCanBeStun = true;
	UPROPERTY(VisibleAnywhere, Category = "Boss values| Stun")
	bool bIsStunned = false;
	UPROPERTY(VisibleAnywhere, Category = "Boss values| Stun")
	int iStackNeededToStun = 2;
	UPROPERTY(VisibleAnywhere, Category = "Boss values| Stun")
	int iCurrentHarpoonStack = 0;
	UPROPERTY(VisibleAnywhere, Category = "Boss values| Stun")
	float fHarpoonStunDuration = 3.0f;

	
	UPROPERTY(VisibleAnywhere, Category = "Boss values| Stun")
	float fWorldSpeed = 1.f;
	

	UPROPERTY(EditAnywhere, Category = "Boss values")
	FKnockbackSettings knockbackSettings = FKnockbackSettings();
	
	
	UPROPERTY()	AB_AIBoss* aiBoss = nullptr;
	UPROPERTY()	AIU_Player* player = nullptr;
	UPROPERTY()	UC_RespawnComponent* playerRespawnComponent = nullptr;
	UPROPERTY()	UWorld* world = nullptr;
	UPROPERTY()	AM_GameMode* gameMode = nullptr;
	UPROPERTY()	UM_DifficultyManager* difficultyManager = nullptr;
	UPROPERTY()	AM_BlackBoardManager* blackBoardManager = nullptr;
	UPROPERTY() AM_WorldTimeManager* worldTimeManager = nullptr;
	UPROPERTY() AM_CinematicManager* cinematicManager = nullptr;
	UPROPERTY()	TArray<AB_BossCritPoint*> critPoints = { };

	FTimerManager* timerManager = nullptr;

	UPROPERTY()	FTimerHandle deadTimer          = FTimerHandle();
	UPROPERTY()	FTimerHandle cinematicTimer     = FTimerHandle();
	UPROPERTY()	FTimerHandle stunTimer          = FTimerHandle();
	UPROPERTY()	FTimerHandle animInGroundTimer  = FTimerHandle();
	UPROPERTY()	FTimerHandle animOutGroundTimer = FTimerHandle();
	UPROPERTY()	FTimerHandle invincibleTimer    = FTimerHandle();
	UPROPERTY()	FTimerHandle playerInitTimer    = FTimerHandle();
	UPROPERTY()	FTimerHandle knockBackTimer     = FTimerHandle();
	
protected:
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnBossMiddleLifeAnimStart onBossMidLifeAnimStart  = FOnBossMiddleLifeAnimStart();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnBossMiddleLifeAnimEnd onBossMidLifeAnimEnd      = FOnBossMiddleLifeAnimEnd();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnBossStunned onBossStunned                       = FOnBossStunned();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnBossStunnedEnd onBossStunnedEnd                 = FOnBossStunnedEnd();
	
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnBossActivated onBossActivated                   = FOnBossActivated();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnBossReset onBossReset                           = FOnBossReset();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnBossDead onBossDead                             = FOnBossDead();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnBossDeadCinematic onBossDeadCinematic           = FOnBossDeadCinematic();
	
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnBossInvincibilityStart onBossInvincibilityStart = FOnBossInvincibilityStart();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnBossInvincibilityEnd onBossInvincibilityEnd     = FOnBossInvincibilityEnd();

	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnInGround onInGround                             = FOnInGround();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnOutGround onOutGround                           = FOnOutGround();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnStartDash onStartDash                           = FOnStartDash();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable) FOnEndDash onEndDash                               = FOnEndDash();

private:
	UPROPERTY() FOnTickAnim onTickAnim = FOnTickAnim();
	
public:
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() const { return skeletalMeshComponent; }	
	FORCEINLINE UCapsuleComponent* GetBossCapsuleCollider() const { return capsuleCollider; }
	FORCEINLINE UP_PatternCaster* GetPatternCaster() const { return patternCaster; }
	FORCEINLINE UIU_HealthSystemComponent* GetHealthComponent() const { return healthComponent; }
	FORCEINLINE UB_BossMovement* GetBossMovement() const { return bossMovement; }
	FORCEINLINE AB_AIBoss* GetAIBoss() const { return aiBoss; }
	FORCEINLINE UB_BossAnim* GetBossAnim() const { return bossAnimInstance; }
	FORCEINLINE UB_BossSpecialAnim* GetBossSpecialAnim() const { return bossSpecialAnim; }
	FORCEINLINE const TEnumAsByte<EBossPhase>& GetBossPhase() const { return bossPhase; }
	FORCEINLINE UB_DT_CritPointDataTable* GetCritPointDataTable() const { return critPointDataTable; }
	FORCEINLINE const TArray<AB_BossCritPoint*>& GetCritPoints() const { return critPoints; }
	
	FORCEINLINE FOnBossStunned& OnBossStunned() { return onBossStunned; }
	FORCEINLINE FOnBossStunnedEnd& OnBossStunnedEnd() { return onBossStunnedEnd; }
	FORCEINLINE const bool& GetCanBeStun() const { return bCanBeStun; }
	FORCEINLINE const bool& GetIsStunned() const { return bIsStunned; }
	FORCEINLINE void SetStackNeededToStun(int _number) { iStackNeededToStun = _number; }
	FORCEINLINE void ResetHarpoonStack() { iCurrentHarpoonStack = 0; }
	
	FORCEINLINE FOnBossMiddleLifeAnimStart& OnBossMidLifeAnimStart() { return onBossMidLifeAnimStart; }
	FORCEINLINE FOnBossMiddleLifeAnimEnd& OnBossMidLifeAnimEnd() { return onBossMidLifeAnimEnd; }
	
	FORCEINLINE FOnBossActivated& OnBossActivated() { return onBossActivated; }
	FORCEINLINE FOnBossReset& OnBossReset() { return onBossReset; }
	FORCEINLINE FOnBossDead& OnBossDead() { return onBossDead; }
	FORCEINLINE FOnBossDeadCinematic& OnBossDeadCinematic() { return onBossDeadCinematic; }
	
	FORCEINLINE FOnBossInvincibilityStart& OnBossInvincibilityStart() { return onBossInvincibilityStart; }
	FORCEINLINE FOnBossInvincibilityEnd& OnBossInvincibilityEnd() { return onBossInvincibilityEnd; }
	
	FORCEINLINE FOnInGround& OnInGround() { return onInGround; }
	FORCEINLINE FOnOutGround& OnOutGround() { return onOutGround; }
	FORCEINLINE FOnStartDash& OnStartDash() { return onStartDash; }
	FORCEINLINE FOnEndDash& OnEndDash() { return onEndDash; }

public:
	AB_Boss();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	//Boss Initialized by M_BossManager.
	UFUNCTION() void Init();
	
private:
	/* Register directly to the arena.
	 * If he can't, will sub to on arenaChange from M_BossManager.*/
	UFUNCTION() void RegisterToArena();
	// Called when arena change from M_BossManager.
	UFUNCTION() void PostRegisterToArena(AA_Arena* _arena);

#pragma region CritPoint
	//Create Crit Point from DataTable
	UFUNCTION() void InitializeCritPoint();
	UFUNCTION() void ActivateCritPointCollision();
	UFUNCTION() void UnactivateCritPointCollision();
#pragma endregion CritPoint
	
	//Reset boss when player dead.
	UFUNCTION() void RegisterToPlayerDeath();
	//Deactivate boss, change player camera.
	UFUNCTION() void BossDeath();
	//Broadcast event to play the cinematic
	UFUNCTION() void BroadcastCinematic();
	
	//Change player camera to PLATFORMING.
	UFUNCTION() void ChangeCam();

	UFUNCTION() void MakeBossVisible();
	

private:
	//Link to Arena_Block close event.
	UFUNCTION() void ActivateBoss();
	//Link to Player respawn event.
	UFUNCTION() void ResetBoss();
	//Load next levels
	UFUNCTION() void LoadLevelsAfterDeath() const;

#pragma region KnockBack
	/*Take effect only if capsuleCollider overlap Player layer.*/
	UFUNCTION() void StartKnockBackPlayer(AActor* _mine, AActor* _other);
	UFUNCTION() void KnockBackPlayer();
	UFUNCTION() void EndKnockBackPlayer(AActor* _mine, AActor* _other);
#pragma endregion KnockBack

#pragma region Stun
public:
	//Set stun harpoon data.
	UFUNCTION() void SetHarpoonData(int _iStackNeededToStun, float _fHarpoonStunDuration);
	//Check if the boss can be Stunned.
	UFUNCTION() void CheckIfStunned();
	
private:
	//Stun the boss, set stun anim and call onBossStunned.
	UFUNCTION() void StunBoss(const float& _stunDuration);
	//Set the stun end animation.
	UFUNCTION() void StunBossEnd();
	//Call onBossStunnedEnd.
	UFUNCTION() void StunBossAnimationEnd(const TEnumAsByte<EBossSpecialAnimState>& _specialAnim);

	UFUNCTION() void WhenBossIsStunStopMove();

public:
	UFUNCTION() void SetInvincibleStart();
	UFUNCTION() void SetInvincibleTimeStart(const float& _time);
	UFUNCTION() void SetInvincibleEnd();

private:
	UFUNCTION() void UnStunLockAfterTime();
#pragma endregion Stun

#pragma region Movement
public:
	//Wait animation end to do internal in ground behavior.
	UFUNCTION() void SetInGround();
	//Wait animation end to do internal out ground behavior.
	UFUNCTION() void SetOutGround();
	//Wait animation end to do internal start dash behavior.
	UFUNCTION() void SetStartDash();
	//Wait animation end to do internal end dash behavior.
	UFUNCTION() void SetEndDash();

private:
	//Disable collision and visibility.
	UFUNCTION() void Internal_SetInGround();
	//Enable collision and visibility.
	UFUNCTION() void Internal_SetOutGround();
	//Wait end of start dash animation.
	UFUNCTION() void Internal_SetStartDash();
	//Wait end of end dash animation.
	UFUNCTION() void Internal_SetEndDash();
#pragma endregion Movement

#pragma region Anim
private:
	UFUNCTION() void CheckIsMidLife(float _actualLife, float _maxLife);
	UFUNCTION() void StartMidLifeAnim();
	UFUNCTION() void EndMidLifeAnim(const TEnumAsByte<EBossSpecialAnimState>& _specialAnim);
	UFUNCTION() void PlayDeathAnimation();

	UFUNCTION() void ChangeAnimWorldSpeed(const float& _worldSpeed);
	UFUNCTION() void UpdateSkeletalAnim(const float& _deltaTime);
#pragma endregion Anim
};