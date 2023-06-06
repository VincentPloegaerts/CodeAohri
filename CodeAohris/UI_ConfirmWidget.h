#pragma once
#include "CoreMinimal.h"
#include "UI_BackWidget.h"
#include "Components/TextBlock.h"

#include "UI_ConfirmWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_ConfirmWidget : public UUI_BackWidget
{
	GENERATED_BODY() 

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirm);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCancel);
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Confirm box values")
		UTextBlock* titleText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Confirm box values")
		UTextBlock* text = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Confirm box values")
		UButton* confirmButton = nullptr;
	
private:
	UPROPERTY()
	FOnConfirm onConfirm;
	UPROPERTY()
	FOnCancel onCancel;
	
public:
	FORCEINLINE FOnConfirm& OnConfirm() { return onConfirm; }
	FORCEINLINE FOnCancel& OnCancel() { return onCancel; }

private:
	virtual void InitializeWidget() override;
	virtual void InitializeFocus() override;
	
	UFUNCTION() void Confirm();
	UFUNCTION() void Cancel();

public:
	void InitializeConfirmBox(const FString& _titleText, const FString& _text) const;
};