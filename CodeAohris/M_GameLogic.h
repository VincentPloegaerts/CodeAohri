#pragma once
#include "CoreMinimal.h"
#include "GPE_SaveTrigger.h"
#include "GameFramework/Actor.h"
#include "M_GameLogic.generated.h"

class AT_TutoLogic;
class AIU_Player;
class AB_Boss;
class AIU_CameraManager;
class AM_ArenaManager;
class AM_SpawnPointManager;
class AM_ProjectileManager;
class AM_BossRushLogic;

UCLASS()
class INSIDEUS_API AM_GameLogic : public AActor
{
	GENERATED_BODY()

	#pragma region Events

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRegistered, AIU_Player*, _player);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossRegistered, AB_Boss*, _boss);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckpointUpdate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckpoint, const bool, _saveProgression);
	
	UPROPERTY()
	FOnPlayerRegistered onPlayerRegistered = FOnPlayerRegistered();

	UPROPERTY()
	FOnBossRegistered onBossRegistered = FOnBossRegistered();
	
	UPROPERTY()
	FOnCheckpointUpdate onCheckpointUpdate;
	UPROPERTY()
	FOnCheckpoint onCheckpoint;

	#pragma endregion

	#pragma region Player
	
	UPROPERTY(EditAnywhere, Category = "GameLogic | Pointer", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSoftObjectPtr<AActor> playerStart = TSoftObjectPtr<AActor>();
	
	UPROPERTY(VisibleAnywhere, Category = "GameLogic | Pointer")
		AIU_Player* player = nullptr;;

	#pragma endregion

	#pragma region Boss
	
	UPROPERTY(VisibleAnywhere, Category = "GameLogic | Pointer")
		AB_Boss* boss = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "GameLogic | Manager | Type", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSubclassOf<AM_BossRushLogic> bossRushLogicType;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = True), Category = "GameLogic | Manager | Pointer")
		AM_BossRushLogic* bossRushLogic = nullptr;

	#pragma endregion

	#pragma region Camera
	
	UPROPERTY(EditAnywhere, Category = "GameLogic | Manager | Type")
		TSubclassOf<AIU_CameraManager> cameraManagerType = TSubclassOf<AIU_CameraManager>();
	
	UPROPERTY(VisibleAnywhere, Category = "GameLogic | Manager | Pointer")
		AIU_CameraManager* cameraManager = nullptr;

	#pragma endregion

	#pragma region Arena
	
	UPROPERTY(EditAnywhere, Category = "GameLogic | Manager | Type")
		TSubclassOf<AM_ArenaManager> arenaManagerType = TSubclassOf<AM_ArenaManager>();
	
	UPROPERTY(VisibleAnywhere, Category = "GameLogic | Manager | Pointer")
		AM_ArenaManager* arenaManager = nullptr;

	#pragma endregion

	#pragma region Projectile
	
	UPROPERTY(EditAnywhere, Category = "GameLogic | Manager | Type")
		TSubclassOf<AM_ProjectileManager> projectileManagerType = TSubclassOf<AM_ProjectileManager>();
	
	UPROPERTY(VisibleAnywhere, Category = "GameLogic | Manager | Pointer")
		AM_ProjectileManager* projectileManager = nullptr;

	#pragma endregion

	#pragma region SpawnPoint
	
	UPROPERTY(EditAnywhere, Category = "GameLogic | Manager | Type")
		TSubclassOf<AM_SpawnPointManager> spawnPointManagerType = TSubclassOf<AM_SpawnPointManager>();
	
	UPROPERTY(VisibleAnywhere, Category = "GameLogic | Manager | Pointer")
		AM_SpawnPointManager* spawnPointManager = nullptr;

	#pragma endregion

	#pragma region Respawn
	
	UPROPERTY(VisibleAnywhere, Category = "GameLogic | Respawn")
		FTransform fallRespawnPoint = FTransform();
	
	UPROPERTY(VisibleAnywhere, Category = "GameLogic | Respawn")
		FTransform deathRespawnPoint = FTransform();
	
	UPROPERTY(VisibleAnywhere, Category = "GameLogic | Respawn")
		AGPE_SaveTrigger* saveTrigger = nullptr;

	#pragma endregion
	
	#pragma region Tuto
	
	UPROPERTY(VisibleAnywhere, Category = "GameLogic | Pointer")
		AT_TutoLogic* tutoLogic = nullptr;

	#pragma endregion 
	
	UPROPERTY()
		UWorld* world = nullptr;

	#pragma region Get
	
public:
	FORCEINLINE AIU_Player* GetPlayer() const { return player; }
	FORCEINLINE AB_Boss* GetBoss() const { return boss; }

	FORCEINLINE AIU_CameraManager* GetCameraManager() const { return cameraManager; }
	FORCEINLINE AM_ArenaManager* GetArenaManager() const { return arenaManager; }
	FORCEINLINE AM_ProjectileManager* GetProjectileManager() const { return projectileManager; }
	FORCEINLINE AM_SpawnPointManager* GetSpawnPointManager() const { return spawnPointManager; }
	FORCEINLINE AT_TutoLogic* GetTutoLogic() const { return tutoLogic; }
	FORCEINLINE AM_BossRushLogic* GetBossRushLogic() const { return bossRushLogic; }
	
	FORCEINLINE FVector GetRespawnLocation(const bool _isSaveTriggerDeath) const
	{
		if(saveTrigger)
			saveTrigger->RollbackLevelStreamer();
		
		return (_isSaveTriggerDeath ? deathRespawnPoint : fallRespawnPoint).GetLocation();
	}
	FORCEINLINE FRotator GetRespawnRotation(const bool _isSaveTriggerDeath) const
	{
		return (_isSaveTriggerDeath ? deathRespawnPoint : fallRespawnPoint).Rotator();
	}

	FORCEINLINE FOnPlayerRegistered* OnPlayerRegistered() { return &onPlayerRegistered; }
	FORCEINLINE FOnBossRegistered* OnBossRegistered() { return &onBossRegistered; }
	
	FORCEINLINE FOnCheckpointUpdate& OnCheckpointUpdate() { return onCheckpointUpdate; }
	FORCEINLINE FOnCheckpoint& OnCheckpoint() { return onCheckpoint; }

	#pragma endregion

	#pragma region Set
	
	FORCEINLINE void SetPlayer(AIU_Player* _player)
	{
		player = _player;
		onPlayerRegistered.Broadcast(player);
	}
	FORCEINLINE void SetBoss(AB_Boss* _boss)
	{
		boss = _boss;
		onBossRegistered.Broadcast(boss);
	}
	
	FORCEINLINE void SetRespawnLocationAndRotation(const bool _isSaveTriggerDeath, const FVector& _location, const FRotator& _rotation, AGPE_SaveTrigger* _saveTrigger = nullptr)
	{
		saveTrigger = _saveTrigger;
		fallRespawnPoint.SetLocation(_location);
		fallRespawnPoint.SetRotation(FQuat(_rotation));
		
		if (_isSaveTriggerDeath)
		{
			deathRespawnPoint.SetLocation(_location);
			deathRespawnPoint.SetRotation(FQuat(_rotation));
		}
	}

	FORCEINLINE void SetTutoLogic(AT_TutoLogic* _logic)
	{
		tutoLogic = _logic;
	}
	#pragma endregion
	
public:	
	AM_GameLogic();
	void InitializeManagers();

	UFUNCTION() void Checkpoint(const bool _saveProgression); 
	
private:	
	template<class ClassType>
	ClassType* CreateManager(const TSubclassOf<ClassType>& _type, const FString& _name) 
	{
		if (!world) return nullptr;
		const FVector& _location = FVector(0);
		const FRotator& _rotation = FRotator(0);
		
		ClassType* _spawned = GetWorld()->SpawnActor<ClassType>(_type, _location, _rotation);
		if (_spawned)
		{
			_spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

#if WITH_EDITOR
			_spawned->SetActorLabel(_name);
#endif
		}
		
		return _spawned;
	}
};