#pragma once
#include "Components/InputKeySelector.h"
#include "STRUCT_BindWidget.generated.h"

USTRUCT(BlueprintType)
struct FBind
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Bind values")
		FString bindName = FString();

	UPROPERTY(EditAnywhere, Category = "Bind values", meta = (UIMin = "-1.0", UIMax = "1.0", ClampMin = "-1.0", ClampMax = "1.0"))
		float fScale = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "Bind values")
		bool bIsAxis = nullptr;

	FBind() { }
};

USTRUCT(BlueprintType)
struct FBindWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Bind widget values")
		UInputKeySelector* inputKey = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Bind widget values")
		FString inputName = FString();

	UPROPERTY(VisibleAnywhere, Category = "Bind widget values")
		FKey oldKey = FKey();

	UPROPERTY(VisibleAnywhere, Category = "Bind values", meta = (UIMin = "-1.0", UIMax = "1.0", ClampMin = "-1.0", ClampMax = "1.0"))
		float fScaleInput = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Bind widget values")
		bool bIsAxisInput = true;
	
	FORCEINLINE FKey GetSelectedKey() const { return inputKey->SelectedKey.Key; }
	
	FBindWidget() { }
	
	FBindWidget(UInputKeySelector* _inputKey, const FString& _name, const FKey& _key, const bool _isAxis)
	{
		inputKey = _inputKey;
		inputName = _name;
		oldKey = _key;
		bIsAxisInput = _isAxis;
	}
};