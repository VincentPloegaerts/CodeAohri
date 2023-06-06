#pragma once

#include "CoreMinimal.h"
#include "BT_TN_Immune.h"

#include "BT_TN_ImmuneEnd.generated.h"

UCLASS()
class INSIDEUS_API UBT_TN_ImmuneEnd : public UBT_TN_Immune
{
	GENERATED_BODY()

private:
	virtual void ImmuneTask() override;

	virtual void PostInitProperties() override;
	virtual FString GetStaticDescription() const override;
};