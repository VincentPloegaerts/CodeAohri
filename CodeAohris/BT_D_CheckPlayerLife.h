#pragma once

#include "CoreMinimal.h"
#include "BT_D_CheckLife.h"
#include "BT_D_CheckPlayerLife.generated.h"

class UBT_TN_CastPatternSerie;
class UBT_TN_CastPattern;

UCLASS()
class INSIDEUS_API UBT_D_CheckPlayerLife : public UBT_D_CheckLife
{
	GENERATED_BODY()

	UPROPERTY()
	UBT_TN_CastPatternSerie* serieOwner = nullptr;

	UPROPERTY()
	UBT_TN_CastPattern* patternOwner = nullptr;

public:
	UBT_D_CheckPlayerLife();

private:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;

	UFUNCTION() void ReCalculateLifeCondition(float _actualLife, float _totalLife);
	
	virtual void PostInitProperties() override;
	virtual FString GetStaticSpecificDescription() const override;
};
