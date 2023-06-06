#pragma once

#include "CoreMinimal.h"
#include "B_Boss.h"
#include "GameFramework/Actor.h"
#include "M_BossRushLogic.generated.h"

class UM_GameInstance;
class AM_CinematicManager;
class AA_ArenaBlock;
class AM_BossManager;
class AHUD_Game;

UCLASS()
class INSIDEUS_API AM_BossRushLogic : public AActor
{
	GENERATED_BODY()
	UPROPERTY()
		AM_BossManager* bossManager = nullptr;
	UPROPERTY()
		AIU_Player* player;
	UPROPERTY(EditAnywhere)
		TMap<TSoftObjectPtr<AB_Boss>,TSoftObjectPtr<AA_ArenaBlock>> positionToGoForEachBoss;
	UPROPERTY(EditAnywhere)
		float delayBeforeBossStart = 5.0f;
	UPROPERTY(EditAnywhere)
		FString changeBossConfirmBoxTitle = "Boss Rush";
	UPROPERTY(EditAnywhere)
		FString changeBossConfirmBoxText = "Teleport to next boss ?";
	UPROPERTY(EditAnywhere)
		FString goBackToMenuConfirmBoxTitle = "Boss Rush";
	UPROPERTY(EditAnywhere)
		FString goBackToMenuConfirmBoxText = "Go back to menu ?";
	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<AB_Boss> lastBoss;
	UPROPERTY()
		AHUD_Game* hud = nullptr;
	UPROPERTY()
		AA_ArenaBlock* nextTP = nullptr;
	UPROPERTY()
		APlayerController* playerController = nullptr;
	UPROPERTY()
		AM_GameMode* gameMode;
	UPROPERTY()
		AM_CinematicManager* cinematicManager;
	UPROPERTY()
		UM_GameInstance* gameInstance = nullptr;

public:	
	AM_BossRushLogic();
public:
	FORCEINLINE float GetDelayBeforeStartBoss() const {return delayBeforeBossStart;}
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable) void Init();
	UFUNCTION(BlueprintCallable) void BossFinished(AB_Boss* _boss);
	UFUNCTION(BlueprintCallable) void TeleportToNewBoss();
	UFUNCTION(BlueprintCallable) void TeleportPlayer();
	UFUNCTION(BlueprintCallable) void AskGoToNextBoss();
	UFUNCTION(BlueprintCallable) void AskGoToNextBossDelay();
	UFUNCTION(BlueprintCallable) void AskGoBackToMenu();
	UFUNCTION(BlueprintCallable) void AskGoBackToMenuDelay();
	UFUNCTION(BlueprintCallable) void GoBackToMainMenu();
	UFUNCTION(BlueprintCallable) void BossStarting();

};
