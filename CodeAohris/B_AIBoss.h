#pragma once

#include "CoreMinimal.h"
#include "B_AICasterBase.h"
#include "B_AIBoss.generated.h"

class UBehaviorTreeComponent;
class AB_Boss;
class UP_PatternCaster;
class UBT_BlackBoard;
class AM_BlackBoardManager;

UCLASS()
class INSIDEUS_API AB_AIBoss : public AB_AICasterBase
{
	GENERATED_BODY()

	friend class AM_BlackBoardManager;
	
	UPROPERTY(VisibleAnywhere, Category = "AI Boss | Values")
	AB_Boss* boss = nullptr;

public:
	FORCEINLINE AB_Boss* GetBoss() const { return boss; }

protected:
	virtual void BeginPlay() override;

	virtual void InitAIBossPhase() override;
	
};