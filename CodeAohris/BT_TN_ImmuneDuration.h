#pragma once

#include "CoreMinimal.h"
#include "BT_TN_Immune.h"

#include "BT_TN_ImmuneDuration.generated.h"

class UIU_HealthSystemComponent;
UCLASS()
class INSIDEUS_API UBT_TN_ImmuneDuration : public UBT_TN_Immune
{
	GENERATED_BODY()

	UPROPERTY()
	UIU_HealthSystemComponent* health = nullptr;

	UPROPERTY(EditAnywhere, Category = "Immune Settings", meta = (ClampMin = "0", ClampMax = "120"))
	float fImmuneTime = 3;

	UPROPERTY()
	FTimerHandle timer;

private:
	virtual void ImmuneTask() override;
	UFUNCTION() void StopImmune();

	virtual void PostInitProperties() override;
	virtual FString GetStaticDescription() const override;
};