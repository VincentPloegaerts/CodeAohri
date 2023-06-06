#pragma once

#include "CoreMinimal.h"
#include "BT_D_CheckLife.h"
#include "BT_D_CheckBossLife.generated.h"

class UBT_TN_CastPattern;
class UBT_TN_CastPatternSerie;

UCLASS()
class INSIDEUS_API UBT_D_CheckBossLife : public UBT_D_CheckLife
{
	GENERATED_BODY()

	UPROPERTY()
	UBT_TN_CastPatternSerie* serieOwner = nullptr;

	UPROPERTY()
	UBT_TN_CastPattern* patternOwner = nullptr;

public:
	UBT_D_CheckBossLife();
	
protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;

	UFUNCTION() void ReCalculateLifeCondition(float _actualLife, float _totalLife);
	
	virtual void PostInitProperties() override;
	virtual FString GetStaticSpecificDescription() const override;
};
