#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BT_D_CheckLife.generated.h"

UENUM()
enum ECheckEnum
{
	ABOVE,
	UNDER,
	BETWEEN
};

UCLASS(Abstract)
class INSIDEUS_API UBT_D_CheckLife : public UBTDecorator
{
	GENERATED_BODY()
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckLifeConditionTrigger);

	UPROPERTY(EditAnywhere, Category = "Check Life | Settings")
	TEnumAsByte<ECheckEnum> checkType = ECheckEnum::ABOVE;
	
	UPROPERTY(EditAnywhere, Category = "Check Life | Settings")
	bool bPercent = true;
	
	UPROPERTY(EditAnywhere, Category = "Check Life | Settings", meta = (ClampMin = "0", ClampMax = "1", Delta = "0.01", EditCondition = "bPercent && checkType != ECheckEnum::BETWEEN", EditConditionHides))
	float checkPercent = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Check Life | Settings", meta = (ClampMin = "0", ClampMax = "0.99", Delta = "0.01", EditCondition = "bPercent && checkType == ECheckEnum::BETWEEN", EditConditionHides))
	float minCheckPercent = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Check Life | Settings", meta = (ClampMin = "0.01", ClampMax = "1", Delta = "0.01", EditCondition = "bPercent && checkType == ECheckEnum::BETWEEN", EditConditionHides))
	float maxCheckPercent = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Check Life | Settings", meta = (ClampMin = "0", EditCondition = "!bPercent && checkType != ECheckEnum::BETWEEN", EditConditionHides))
	float checkValue = 50;
	UPROPERTY(EditAnywhere, Category = "Check Life | Settings", meta = (ClampMin = "0", EditCondition = "!bPercent && checkType == ECheckEnum::BETWEEN", EditConditionHides))
	float minCheckValue = 50;
	UPROPERTY(EditAnywhere, Category = "Check Life | Settings", meta = (ClampMin = "1", EditCondition = "!bPercent && checkType == ECheckEnum::BETWEEN", EditConditionHides))
	float maxCheckValue = 50;

	UPROPERTY()
	FOnCheckLifeConditionTrigger onCheckLifeConditionTrigger;

public:
	FORCEINLINE FOnCheckLifeConditionTrigger& OnCheckLifeConditionTrigger() { return onCheckLifeConditionTrigger; }

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual bool CheckLife(const float& _life) const;
	
private:
	bool CheckAbove(const float& _life) const;
	bool CheckUnder(const float& _life) const;
	bool CheckBetween(const float& _life) const;
	
	virtual FString GetStaticDescription() const override;
protected:
	virtual FString GetStaticSpecificDescription() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};