#pragma once

#include "CoreMinimal.h"
#include "UI_DifficultyWidget.h"

#include "Components/CheckBox.h"
#include "Delegates/DelegateCombinations.h"

#include "UI_IronModeWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_IronModeWidget : public UUI_DifficultyWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIronModeSelect, const bool, _ironModeEnable);
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCheckBox* ironModeCB = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = "Settings | Text")
	FString ironModeText = "Iron Mode";
	
	UPROPERTY(EditAnywhere, Category = "Settings | Confirm Box")
	FString enableIronModeConfirmTitle = "Enable Iron Mode ?";
	UPROPERTY(EditAnywhere, Category = "Settings | Confirm Box")
	FString enableIronModeConfirmText = "Iron mode can not be disabled once enabled on a Save";
	
	UPROPERTY()
	FOnIronModeSelect onIronModeSelect;

public:
	FORCEINLINE FOnIronModeSelect& OnIronModeSelect() { return onIronModeSelect; }
	
public:
	virtual void InitializeDifficultyWidget(const EDifficultyType& _difficulty, const FString& _description, const TArray<FString>& _extraDescription, UTexture2D* _image = nullptr) override;

	void Refresh();
	
private:
	virtual void InitializeWidget() override;

	virtual void CheckDifficultyDescription() override;
	
	UFUNCTION(BlueprintCallable) void ChangeIronMode(bool _enable);
	UFUNCTION(BlueprintCallable) void ConfirmIronMode();
	UFUNCTION(BlueprintCallable) void SetIronMode(bool _enable);
};
