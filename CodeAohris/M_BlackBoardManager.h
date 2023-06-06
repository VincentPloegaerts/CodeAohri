#pragma once

#include "CoreMinimal.h"
#include "STRUCT_PatternMovementData.h"
#include "GameFramework/Actor.h"
#include "M_BlackBoardManager.generated.h"

class AB_AI_AFKKiller;
class AB_AIBoss;
class UB_BossAnim;
class UB_BossMovement;
class UBT_D_BossMovement;
class UBT_TN_CastPattern;

UCLASS()
class INSIDEUS_API AM_BlackBoardManager : public AActor
{
	GENERATED_BODY()

public:
	AM_BlackBoardManager();
	
private:
	virtual void BeginPlay() override;

#pragma region BossMovement_OnReachGoal
private:
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnReachGoal") UBT_TN_CastPattern* onReachGoal_TaskNodeCastPattern = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnReachGoal")                bool onReachGoal_TaskNodeCastPattern_IsMoving = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnReachGoal") UBT_D_BossMovement* onReachGoal_DecoratorBossMovement = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Debug|OnReachGoal")    UB_BossMovement* onReachGoal_BossMovement = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnReachGoal")                bool onReachGoal_BossMovement_LookPlayer = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnReachGoal")        UB_BossAnim* onReachGoal_BossAnim = nullptr;
	
public:
	UFUNCTION() FORCEINLINE void Set_OnReachGoal_TaskNodeCastPattern(UBT_TN_CastPattern* _taskNode)	   { onReachGoal_TaskNodeCastPattern = _taskNode; }
	UFUNCTION() FORCEINLINE void Set_OnReachGoal_TaskNodeCastPattern_IsMoving(const bool& _state)      { onReachGoal_TaskNodeCastPattern_IsMoving = _state; }
	
	UFUNCTION() FORCEINLINE void Set_OnReachGoal_DecoratorBossMovement(UBT_D_BossMovement* _decorator) { onReachGoal_DecoratorBossMovement = _decorator; }
	
	UFUNCTION() FORCEINLINE void Set_OnReachGoal_BossMovement(UB_BossMovement* _bossMovement)          { onReachGoal_BossMovement = _bossMovement; }
	UFUNCTION() FORCEINLINE void Set_OnReachGoal_BossMovement_LookPlayer(const bool& _state)           { onReachGoal_BossMovement_LookPlayer = _state; }
	
	UFUNCTION() FORCEINLINE void Set_OnReachGoal_BossAnim(UB_BossAnim* _bossAnim)                      { onReachGoal_BossAnim = _bossAnim; }
	
	
	UFUNCTION() void Broadcast_BossMovement_OnReachGoal();
#pragma endregion BossMovement_OnReachGoal

#pragma region BossMovement_OnDislodgePlayer
private:
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnDislodgePlayer") UBT_D_BossMovement* onDislodgePlayer_DecoratorBossMovement = nullptr;

public:
	UFUNCTION() FORCEINLINE void Set_OnDislodgePlayer_DecoratorBossMovement(UBT_D_BossMovement* _decorator) { onDislodgePlayer_DecoratorBossMovement = _decorator; }


	UFUNCTION() void Broadcast_BossMovement_OnDislodgePlayer(const FPatternMovementData& _data);
#pragma endregion BossMovement_OnDislodgePlayer

#pragma region Boss_OnBossMidLifeAnimStart
private:
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossMidLifeAnimStart") UBT_TN_CastPattern* onBossMidLifeAnimStart_TaskNodeCastPattern = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossMidLifeAnimStart") UBT_TN_CastPattern* onBossMidLifeAnimStart_TaskNodeCastPattern_AFK = nullptr;
	
public:
	UFUNCTION() FORCEINLINE void Set_OnBossMidLifeAnimStart_TaskNodeCastPattern(UBT_TN_CastPattern* _taskNode)     { onBossMidLifeAnimStart_TaskNodeCastPattern = _taskNode; }
	UFUNCTION() FORCEINLINE void Set_OnBossMidLifeAnimStart_TaskNodeCastPattern_AFK(UBT_TN_CastPattern* _taskNode) { onBossMidLifeAnimStart_TaskNodeCastPattern_AFK = _taskNode; }


	UFUNCTION() void Broadcast_Boss_OnBossMidLifeAnimStart();
#pragma endregion Boss_OnBossMidLifeAnimStart

#pragma region Boss_OnBossMidLifeAnimEnd
private:
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossMidLifeAnimEnd") UBT_TN_CastPattern* onBossMidLifeAnimEnd_TaskNodeCastPattern = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossMidLifeAnimEnd") UBT_TN_CastPattern* onBossMidLifeAnimEnd_TaskNodeCastPattern_AFK = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossMidLifeAnimEnd") UBT_D_BossMovement* onBossMidLifeAnimEnd_DecoratorBossMovement = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossMidLifeAnimEnd")          AB_AIBoss* onBossMidLifeAnimEnd_AIBoss = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossMidLifeAnimEnd")    AB_AI_AFKKiller* onBossMidLifeAnimEnd_AIAFKKiller = nullptr;
	
public:
	UFUNCTION() FORCEINLINE void Set_OnBossMidLifeAnimEnd_TaskNodeCastPattern(UBT_TN_CastPattern* _taskNode)	 { onBossMidLifeAnimEnd_TaskNodeCastPattern = _taskNode; }
	UFUNCTION() FORCEINLINE void Set_OnBossMidLifeAnimEnd_TaskNodeCastPattern_AFK(UBT_TN_CastPattern* _taskNode) { onBossMidLifeAnimEnd_TaskNodeCastPattern_AFK = _taskNode; }
	
	UFUNCTION() FORCEINLINE void Set_OnBossMidLifeAnimEnd_DecoratorBossMovement(UBT_D_BossMovement* _decorator)  { onBossMidLifeAnimEnd_DecoratorBossMovement = _decorator; }

	UFUNCTION() FORCEINLINE void Set_OnBossMidLifeAnimEnd_AIBoss(AB_AIBoss* _aiBoss)                             { onBossMidLifeAnimEnd_AIBoss = _aiBoss; }

	UFUNCTION() FORCEINLINE void Set_OnBossMidLifeAnimEnd_AIAFKKIller(AB_AI_AFKKiller* _aiAFKKiller)             { onBossMidLifeAnimEnd_AIAFKKiller = _aiAFKKiller; }

	
	UFUNCTION() void Broadcast_Boss_OnBossMidLifeAnimEnd();
#pragma endregion Boss_OnBossMidLifeAnimEnd

#pragma region Boss_OnBossStunned
private:
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossStunned") UBT_TN_CastPattern* onBossStunned_TaskNodeCastPattern = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossStunned") UBT_TN_CastPattern* onBossStunned_TaskNodeCastPattern_AFK = nullptr;
	
public:
	UFUNCTION() FORCEINLINE void Set_OnBossStunned_TaskNodeCastPattern(UBT_TN_CastPattern* _taskNode)     { onBossStunned_TaskNodeCastPattern = _taskNode; }
	UFUNCTION() FORCEINLINE void Set_OnBossStunned_TaskNodeCastPattern_AFK(UBT_TN_CastPattern* _taskNode) { onBossStunned_TaskNodeCastPattern_AFK = _taskNode; }

	
	UFUNCTION() void Broadcast_Boss_OnBossStunned();
#pragma endregion Boss_OnBossStunned

#pragma region Boss_OnBossStunnedEnd
private:
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossStunnedEnd") UBT_TN_CastPattern* onBossStunnedEnd_TaskNodeCastPattern = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossStunnedEnd") UBT_TN_CastPattern* onBossStunnedEnd_TaskNodeCastPattern_AFK = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossStunnedEnd") UBT_D_BossMovement* onBossStunnedEnd_DecoratorBossMovement = nullptr;

public:
	UFUNCTION() FORCEINLINE void Set_OnBossStunnedEnd_TaskNodeCastPattern(UBT_TN_CastPattern* _taskNode)     { onBossStunnedEnd_TaskNodeCastPattern = _taskNode; }
	UFUNCTION() FORCEINLINE void Set_OnBossStunnedEnd_TaskNodeCastPattern_AFK(UBT_TN_CastPattern* _taskNode) { onBossStunnedEnd_TaskNodeCastPattern_AFK = _taskNode; }
	
	UFUNCTION() FORCEINLINE void Set_OnBossStunnedEnd_DecoratorBossMovement(UBT_D_BossMovement* _decorator)  { onBossStunnedEnd_DecoratorBossMovement = _decorator; }


	UFUNCTION() void Broadcast_Boss_OnBossStunnedEnd();
#pragma endregion Boss_OnBossStunnedEnd

#pragma region Boss_OnBossReset
private:
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnBossReset") UBT_D_BossMovement* onBossReset_DecoratorBossMovement = nullptr;

public:
	UFUNCTION() FORCEINLINE void Set_OnBossReset_DecoratorBossMovement(UBT_D_BossMovement* _decorator) { onBossReset_DecoratorBossMovement = _decorator; }


	UFUNCTION() void Broadcast_Boss_OnBossReset();
#pragma endregion Boss_OnBossReset

#pragma region PatternCaster_OnPatternComplete
private:
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnPatternComplete") UBT_TN_CastPattern* onPatternComplete_TaskNodeCastPattern = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnPatternComplete") UBT_TN_CastPattern* onPatternComplete_TaskNodeCastPattern_AFK = nullptr;

public:
	UFUNCTION() FORCEINLINE void Set_OnPatternComplete_TaskNodeCastPattern(UBT_TN_CastPattern* _taskNode)     { onPatternComplete_TaskNodeCastPattern = _taskNode; }
	UFUNCTION() FORCEINLINE void Set_OnPatternComplete_TaskNodeCastPattern_AFK(UBT_TN_CastPattern* _taskNode) { onPatternComplete_TaskNodeCastPattern_AFK = _taskNode; }


	UFUNCTION() void Broadcast_PatternCaster_OnPatternComplete();
#pragma endregion PatternCaster_OnPatternComplete

#pragma region BossAnim_OnAnimationEnd
private:
	UPROPERTY(VisibleAnywhere, Category = "Debug|OnAnimationEnd") UBT_TN_CastPattern* onAnimationEnd_TaskNodeCastPattern = nullptr;

public:
	UFUNCTION() FORCEINLINE void Set_OnAnimationEnd_TaskNodeCastPattern(UBT_TN_CastPattern* _taskNode) { onAnimationEnd_TaskNodeCastPattern = _taskNode; }
	
	
	UFUNCTION() void Broadcast_BossAnim_OnAnimationEnd();
#pragma endregion BossAnim_OnAnimationEnd

#pragma region DEBUG_BossLock
private:
	UPROPERTY(VisibleAnywhere, Category = "Debug|DEBUG_Stun") UBT_TN_CastPattern* debugStun_TaskNodeCastPattern = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Debug|DEBUG_Stun") UBT_TN_CastPattern* debugStun_TaskNodeCastPattern_AFK = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Debug|DEBUG_Stun") UBT_D_BossMovement* debugStun_DecoratorBossMovement = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Debug|DEBUG_Stun") bool bShowDebugLog = true;
	UPROPERTY(EditAnywhere, Category = "Debug|DEBUG_Stun", meta = (ClampMin = "1", ClampMax = "20")) float fDebugBossStunTime = 3.f;

	UPROPERTY(EditAnywhere, Category = "Debug|DEBUG_Stun") bool bIsBossActivated = false;
	
	UPROPERTY()	FTimerHandle debugTimer = FTimerHandle();

	FTimerManager* timerManager = nullptr;

public:
	UFUNCTION() FORCEINLINE void Set_DEBUG_Stun_TaskNodeCastPattern(UBT_TN_CastPattern* _taskNode)     { debugStun_TaskNodeCastPattern = _taskNode; }
	UFUNCTION() FORCEINLINE void Set_DEBUG_Stun_TaskNodeCastPattern_AFK(UBT_TN_CastPattern* _taskNode) { debugStun_TaskNodeCastPattern_AFK = _taskNode; }
	
	UFUNCTION() FORCEINLINE void Set_DEBUG_Stun_DecoratorBossMovement(UBT_D_BossMovement* _decorator)  { debugStun_DecoratorBossMovement = _decorator; }

	UFUNCTION() void UPDATE_DEBUG();
	UFUNCTION() void STOP_DEBUG();
	
private:
	UFUNCTION() void DEBUG_BossLock();

public:
	UFUNCTION() void SetBossIsActivated();
#pragma endregion DEBUG_BossLock
};