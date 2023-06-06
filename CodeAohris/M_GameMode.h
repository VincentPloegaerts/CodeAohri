#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "ENUM_DifficultyType.h"
#include "M_GameLogic.h"

#include "M_GameMode.generated.h"

class ALevelLoadingManager;
class AM_BossRushLogic;
//In GameMode
class AM_GameLogic;
class AM_WorldTimeManager;
class AM_UIManager;
class AM_BossManager;
class AM_PatternsTickManager;
class AM_GPE_PatternTickManager;
class AM_PatternsTickManager;
class AM_GPEPatternsTickManager;
class AM_FeedbackManager;
class AM_CollectibleManager;
class AM_CinematicManager;
class AM_BlackBoardManager;
class AM_LeaderboardManager;

//In GameLogic
class AIU_Player;
class AB_Boss;
class AM_ArenaManager;
class AM_ProjectileManager;
class AM_SpawnPointManager;
class AIU_CameraManager;

class AM_Octree;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOctreeSetted, AM_Octree*, octree);

UCLASS()
class INSIDEUS_API AM_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "GameMode | Manager", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<AM_GameLogic> gameLogicType;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AM_GameLogic* gameLogic = nullptr;

	UPROPERTY(EditAnywhere, Category = "GameMode | Manager", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<AM_WorldTimeManager> worldTimeManagerType;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AM_WorldTimeManager* worldTimeManager = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "GameMode | Manager", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<AM_UIManager> uiManagerType = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AM_UIManager* uiManager = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "GameMode | Manager", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<AM_BossManager> bossManagerType;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AM_BossManager* bossManager = nullptr; 
	
	UPROPERTY(EditAnywhere, Category = "GameMode | Manager", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<AM_PatternsTickManager> patternsTickManagerType;	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AM_PatternsTickManager* patternsTickManager = nullptr;

	UPROPERTY(EditAnywhere, Category = "GameMode | Manager", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<AM_GPE_PatternTickManager> gpePatternTickManagerType;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AM_GPE_PatternTickManager* gpePatternTickManager = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "GameMode | Manager", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<AM_FeedbackManager> feedBackManagerType;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AM_FeedbackManager* feedBackManager = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "GameMode | Manager", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<AM_CollectibleManager> collectibleManagerType;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AM_CollectibleManager* collectibleManager = nullptr;

	UPROPERTY(EditAnywhere, Category = "GameMode | Manager", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<AM_CinematicManager> cinematicManagerType;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AM_CinematicManager* cinematicManager = nullptr;

	UPROPERTY(EditAnywhere, Category = "GameMode | Manager", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<AM_BlackBoardManager> blackBoardManagerType;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AM_BlackBoardManager* blackBoardManager = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "GameMode | Manager", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<AM_LeaderboardManager> leaderboardManagerType;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AM_LeaderboardManager* leaderboardManager = nullptr;

	UPROPERTY(EditAnywhere, Category = "GameMode | Manager", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TSubclassOf<ALevelLoadingManager> levelLoadingManagerType;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	ALevelLoadingManager* levelLoadingManager = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	AM_Octree* octree = nullptr;

	UPROPERTY()
	UWorld* world = nullptr;
	
	UPROPERTY()
		FOnOctreeSetted onOctreeSetted = FOnOctreeSetted();

	UPROPERTY(VisibleAnywhere, Category = "GameMode | Pointer")
		TEnumAsByte<EDifficultyType> currentDifficulty = TEnumAsByte<EDifficultyType>();
	
public:

#pragma region GameMode Get
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_UIManager* GetUIManager() const { return uiManager; }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_WorldTimeManager* GetWorldTimeManager() const { return worldTimeManager; }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_PatternsTickManager* GetPatternsTickManager() const { return patternsTickManager; }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_GPE_PatternTickManager* GetGPEPatternTickManager() const { return gpePatternTickManager; }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_FeedbackManager* GetFeedbackManager() const { return feedBackManager; }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_CollectibleManager* GetCollectibleManager() const { return collectibleManager; }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_BossManager* GetBossManager() const { return bossManager; }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_CinematicManager* GetCinematicManager() const { return cinematicManager; }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_BlackBoardManager* GetBlackBoardManager() const { return blackBoardManager; }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_LeaderboardManager* GetLeaderboardManager() const { return leaderboardManager; }
	UFUNCTION(BlueprintCallable) FORCEINLINE ALevelLoadingManager* GetLevelLoadingManager() const { return levelLoadingManager; }
	
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_Octree* GetOctree() const { return octree; }
	FORCEINLINE FOnOctreeSetted* OnOctreeSetted() { return &onOctreeSetted; }
	
#pragma endregion

#pragma region Octree Set
	
	FORCEINLINE void SetOctree(AM_Octree* _octree)
	{
		octree = _octree;
		onOctreeSetted.Broadcast(octree);
	}

#pragma  endregion

#pragma region GameLogic Get
	
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_GameLogic* GetGameLogic() const { return gameLogic; }
	
	UFUNCTION(BlueprintCallable) FORCEINLINE AT_TutoLogic* GetTutoLogic() const { return gameLogic->GetTutoLogic(); }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_BossRushLogic* GetBossRushLogic() const { return gameLogic->GetBossRushLogic(); }
	
	UFUNCTION(BlueprintCallable) FORCEINLINE AIU_Player* GetPlayer() const { return gameLogic->GetPlayer(); }
	UFUNCTION(BlueprintCallable) FORCEINLINE AB_Boss* GetBoss() const { return gameLogic->GetBoss(); }
	
	UFUNCTION(BlueprintCallable) FORCEINLINE AIU_CameraManager* GetCameraManager() const { return gameLogic->GetCameraManager(); }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_ArenaManager* GetArenaManager() const { return gameLogic->GetArenaManager(); }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_ProjectileManager* GetProjectileManager() const { return gameLogic->GetProjectileManager(); }
	UFUNCTION(BlueprintCallable) FORCEINLINE AM_SpawnPointManager* GetSpawnPointManager() const { return gameLogic->GetSpawnPointManager(); }

	UFUNCTION(BlueprintCallable) FORCEINLINE FVector GetRespawnLocation(const bool _isSaveTriggerDeath) const { return gameLogic->GetRespawnLocation(_isSaveTriggerDeath); }
	UFUNCTION(BlueprintCallable) FORCEINLINE FRotator GetRespawnRotation(const bool _isSaveTriggerDeath) const { return gameLogic->GetRespawnRotation(_isSaveTriggerDeath); }

#pragma endregion

#pragma region GameLogic Set
	
	FORCEINLINE void SetPlayer(AIU_Player* _player) const { gameLogic->SetPlayer(_player); }
	FORCEINLINE void SetBoss(AB_Boss* _boss) const { gameLogic->SetBoss(_boss); }
	FORCEINLINE void SetTutoLogic(AT_TutoLogic* _logic) const { gameLogic->SetTutoLogic(_logic); }
	
	FORCEINLINE void SetRespawnPoint(const bool _isDead, const FVector& _location, const FRotator& _rotation, AGPE_SaveTrigger* _saveTrigger = nullptr) const
	{
		gameLogic->SetRespawnLocationAndRotation(_isDead, _location, _rotation, _saveTrigger);
	}

#pragma  endregion
	
private:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitializeManagers();
	UFUNCTION() void FirstLoadFinished();

	template<class ClassType>
ClassType* CreateManager(const TSubclassOf<ClassType>& _type, const FString& _name)
	{
		if (!world || !_type.Get()) return nullptr;
		const FVector& _location = FVector(0);
		const FRotator& _rotation = FRotator(0);
		
		ClassType* _spawned = GetWorld()->SpawnActor<ClassType>(_type, _location, _rotation);
		_spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

#if WITH_EDITOR
		_spawned->SetActorLabel(_name);
#endif
		
		return _spawned;
	}
};