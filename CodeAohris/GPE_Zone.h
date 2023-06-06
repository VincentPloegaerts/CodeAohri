#pragma once

#include "CoreMinimal.h"
#include "STRUCT_GPE_GlobalSettings.h"
#include "STRUCT_SpawnerFeedBack.h"
#include "GPE_Pattern.h"
#include "STRUCT_ProjectileSettings.h"
#include "GPE_Zone.generated.h"

class AM_BlackBoardManager;
class AM_FeedbackManager;
class UC_RespawnComponent;
class AM_GameMode;
class AIU_Player;
class UP_DT_ZoneDataTable;

UCLASS(Abstract)
class INSIDEUS_API AGPE_Zone : public AGPE_Pattern
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	AIU_Player* player = nullptr;
	
	UPROPERTY()
	UC_RespawnComponent* playerRespawn = nullptr;

	UPROPERTY()
	AM_FeedbackManager* feedbackManager = nullptr;

	UPROPERTY()
	FKnockbackSettings knockbackSettings = FKnockbackSettings();

	UPROPERTY(EditAnywhere, Category = "GPE Zone")
	TArray<TEnumAsByte<EObjectTypeQuery>> otherFloorLayer;
	
	UPROPERTY()	const UWorld* world = nullptr;
	UPROPERTY()	AM_GameMode* gameMode = nullptr;
	UPROPERTY() AM_BlackBoardManager* blackBoardManager = nullptr;

	FTimerManager* timerManager = nullptr;
	
	UPROPERTY() FTimerHandle debugBossTimer = FTimerHandle();
	UPROPERTY()	FTimerHandle feedbackTimer = FTimerHandle();
	
public:
	AGPE_Zone();

public:
	virtual void InitializeGPEBehavior(UP_DT_ZoneDataTable* _zoneDataTable);
	virtual void InitializeGPEBehavior(FGPE_GlobalSettings& _patternData);
	UFUNCTION() void InitFeedback(AM_FeedbackManager* _feedbackManager, const FSpawnerFeedBack& _feedback);
	UFUNCTION() void InitKnockBackSettings(const FKnockbackSettings& _knockBackSettings);
	virtual void BehaviorTick(const float& _deltaTime) override;
	virtual void CompleteBehavior() override;

private:
	UFUNCTION() void PlayFeedback(const FSpawnerFeedBack& _feedback) const;

	UFUNCTION() void DebugBossStun();
};