#pragma once

#include "CoreMinimal.h"
#include "ENUM_BossPhase.h"
#include "STRUCT_BossPhase.h"
#include "GameFramework/Actor.h"
#include "M_BossManager.generated.h"

class US_GameSave;
class AM_ArenaManager;
class AM_GameMode;
class AA_Arena;
class AB_Boss;

UCLASS()
class INSIDEUS_API AM_BossManager : public AActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArenaChanged, AA_Arena*, _arena);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossChanged, AB_Boss*, _boss);

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBossPhase> currentBossPhase = EBossPhase::UN;

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EBossPhase>, FBossPhase> bossPhaseSettings;
	
	UPROPERTY(VisibleAnywhere)
	AA_Arena* currentArena = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	AB_Boss* currentBoss = nullptr;
	
	UPROPERTY()
	AM_GameMode* gameMode = nullptr;
	
	UPROPERTY()
	AM_ArenaManager* arenaManager = nullptr;

	UPROPERTY()
	US_GameSave* gameSave = nullptr;

protected:
	UPROPERTY(BlueprintCallable, BlueprintReadWrite) FOnArenaChanged onArenaChanged = FOnArenaChanged();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite) FOnBossChanged onBossChanged = FOnBossChanged();

public:
	FORCEINLINE AA_Arena* GetCurrentArena() const { return currentArena; }
	FORCEINLINE AB_Boss* GetCurrentBoss() const { return currentBoss; }
	FORCEINLINE const TEnumAsByte<EBossPhase>& GetCurrentBossPhase() const { return currentBossPhase; }
	
	FORCEINLINE FOnArenaChanged& OnArenaChanged() { return onArenaChanged; }
	FORCEINLINE FOnBossChanged& OnBossChanged() { return onBossChanged; }
	
public:
	AM_BossManager();
	
private:
	virtual void BeginPlay() override;

public:
	UFUNCTION() void SetArena(AA_Arena* _arena, const EBossPhase& _currentBossPhase);
	UFUNCTION() void SetBoss(AB_Boss* _boss, const EBossPhase& _bossPhase);

private:
	UFUNCTION() void IncrementBossPhase();
};