#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_ArenaBlock.generated.h"

class ALevelLoadingManager;
class AHUD_Game;
class UUI_PlayerWidget;
class AM_GameMode;
class AM_LevelStreamerDeluxeEdition;
class AGPE_SaveTrigger;
class UA_ArenaBlockAnimInstance;
class AIU_Player;
class UNiagaraSystem;
class UBoxComponent;
class AM_CinematicManager;
class UC_RespawnComponent;
class AM_BossManager;

UCLASS()
class INSIDEUS_API AA_ArenaBlock : public AActor
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlockTrigger);

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Arena block | Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UBillboardComponent* billboard = nullptr;		
#endif
	
	UPROPERTY(EditAnywhere, Category = "Arena block | Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UStaticMeshComponent* closedDoorStaticMesh = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Arena block | Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UStaticMeshComponent* otherDoorMesh = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Arena block | Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UBoxComponent* cinematicButtonCollider = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Arena block | Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	USceneComponent* niagaraSpawnLocation = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Arena block | Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	USceneComponent* playerTeleportLocation = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Arena block | Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	USceneComponent* otherDoorCloseLocation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Arena block | Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSoftObjectPtr<AGPE_SaveTrigger> saveTrigger;

	UPROPERTY(EditAnywhere, Category = "Arena block | Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TArray<TSoftObjectPtr<UWorld>> levelsToLoad;

	UPROPERTY(EditAnywhere, Category = "Arena block | Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TArray<TSoftObjectPtr<UWorld>> levelsToUnload;

	UPROPERTY(EditAnywhere, Category = "Arena block | Settings", meta = (UIMin = "0.0", UIMax = "60.0", ClampMin = "0.0", ClampMax = "60.0"), BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float fCloseDelay = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Arena block | Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UNiagaraSystem* niagaraComp = nullptr;
	
	UPROPERTY()	bool bHasSetCameraBoss = false;
	UPROPERTY()	bool bHasPlayCinematic = false;

	UPROPERTY() AIU_Player* player = nullptr;
	UPROPERTY() UC_RespawnComponent* playerRespawnComponent = nullptr;
	UPROPERTY() AM_CinematicManager* cinematicManager = nullptr;

	UPROPERTY() const AM_GameMode* gameMode = nullptr;
	UPROPERTY()	const UWorld* world = nullptr;
	UPROPERTY()	AHUD_Game* hud = nullptr;
	UPROPERTY()	UUI_PlayerWidget* playerWidget = nullptr;
	UPROPERTY()	ALevelLoadingManager* levelLoadingManager = nullptr;
	
	UPROPERTY()	FTimerHandle closeTimer = FTimerHandle();
	UPROPERTY()	FTimerHandle initTimer = FTimerHandle();
	UPROPERTY() AM_BossManager* bossManager = nullptr;
	
	UPROPERTY()	FOnBlockTrigger onBlockTrigger = FOnBlockTrigger();

public:
	FORCEINLINE FOnBlockTrigger& OnBlockTrigger() { return onBlockTrigger; }
	FORCEINLINE void SetCloseDelay(const float& _delay) { fCloseDelay = _delay; }
	
public:	
	AA_ArenaBlock();

protected:
	virtual void BeginPlay() override;
	UFUNCTION() void InitPlayer();
	UFUNCTION() void ActivateArenaBehavior();
	
public: //for bypass ActivateBoss with input
	UFUNCTION(BlueprintCallable) void BroadcastArenaBlockTrigger();
	UFUNCTION() void Interacted();
	UFUNCTION() void LaunchCinematic();

private:
	UFUNCTION() void StartPlayerCanInteract(UPrimitiveComponent* _mine, AActor* _otherActor, UPrimitiveComponent* _otherComponent,
								             int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult);
	UFUNCTION() void StopPlayerCanInteract(UPrimitiveComponent* _mine, AActor* _otherActor, UPrimitiveComponent* _otherComponent, int32 _otherBodyIndex);
	
	UFUNCTION() void EndCinematicBehavior();

	UFUNCTION() void TeleportWithoutCinematic();
};