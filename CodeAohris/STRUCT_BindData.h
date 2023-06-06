#pragma once
#include "Components/InputKeySelector.h"
#include "STRUCT_BindData.generated.h"

USTRUCT(BlueprintType)
struct FBindSetupData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Bind values")
		FName name = FName();

	UPROPERTY(EditAnywhere, Category = "Bind values")
		bool isAction = true;
	
	UPROPERTY(EditAnywhere, Category = "Bind values", meta = (UIMin = "-1.0", UIMax = "1.0", ClampMin = "-1.0", ClampMax = "1.0", EditCondition = "!isAction", EditConditionHides))
		float scale = 0.0f;

	FBindSetupData() { }
};

USTRUCT(BlueprintType)
struct FBindData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = "Bind values")
		UInputKeySelector* inputKey = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Bind values")
		FKey oldKey = FKey();

	UPROPERTY(VisibleAnywhere, Category = "Bind values")
		FBindSetupData setupData = FBindSetupData();

	FBindData() { }
	FBindData(UInputKeySelector* _inputKey, const FBindSetupData& _setupData)
	{
		inputKey = _inputKey;
		setupData = _setupData;
	}
};