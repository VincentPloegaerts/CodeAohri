#pragma once

#include "CoreMinimal.h"
#include "ENUM_BossPhase.h"
#include "B_AICasterBase.h"
#include "B_AI_AFKKiller.generated.h"

class AB_AIBoss;
class UIU_HealthSystemComponent;
class AM_BossManager;
class AB_Boss;

UCLASS()
class INSIDEUS_API AB_AI_AFKKiller : public AB_AICasterBase
{
	GENERATED_BODY()

	friend class AM_BlackBoardManager;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBossPhase> bossPhase = EBossPhase::UN;

	UPROPERTY()
	AB_Boss* boss = nullptr;
	
	UPROPERTY()
	AM_BossManager* bossManager = nullptr;
	
	UPROPERTY()
	UIU_HealthSystemComponent* bossHealth = nullptr;

public:
	AB_AI_AFKKiller();
	
protected:
	virtual void BeginPlay() override;

	virtual void InitAIBossPhase() override;
	
private:
	UFUNCTION() void SubToBossAndBossManager();
	UFUNCTION() void SubToBoss(AB_Boss* _boss);
	UFUNCTION() void DestroyAFKKillerOnBossDeath();
};