#pragma once
#include "CoreMinimal.h"

#include "IU_Player.h"
#include "ENUM_AdditionalConditionType.h"

#include "Engine/DataAsset.h"
#include "T_AdditionalCondition.generated.h"

UCLASS()
class INSIDEUS_API UT_AdditionalCondition : public UDataAsset
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConditionDone);
	
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EAdditionalConditionType> type = TEnumAsByte<EAdditionalConditionType>();

	UPROPERTY()
		UC_PlayerAttackComponent* attack = nullptr;
	
	UPROPERTY()
		FOnConditionDone onConditionDone = FOnConditionDone();
	
public:
	FORCEINLINE FOnConditionDone* OnConditionDone() { return &onConditionDone; }
	
public:
	UT_AdditionalCondition();
	
public:
	void InitCondition(const AIU_Player* _player);
	void ResetCondition();

private:
	UFUNCTION(BlueprintCallable) void StopCondition();
};