#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_UserWidget.generated.h"

class UButton;
class USlider;
class UComboBoxString;
class UCheckBox;
class UBorder;

USTRUCT(BlueprintType)
struct FComboBoxPair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UComboBoxString* ComboBox = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBorder* Border = nullptr;
};

USTRUCT(BlueprintType)
struct FCheckBoxPair
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCheckBox* CheckBox = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBorder* Border = nullptr;
};

UCLASS(Abstract)
class INSIDEUS_API UUI_UserWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVisibilityChanged);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Widget")
	TArray<UButton*> Buttons = { };
	UPROPERTY(BlueprintReadWrite, Category = "Widget")
	TArray<USlider*> Sliders = { };
	UPROPERTY(BlueprintReadWrite, Category = "Widget")
	TArray<FComboBoxPair> ComboBoxes = { };
	UPROPERTY(BlueprintReadWrite, Category = "Widget")
	TArray<FCheckBoxPair> CheckBoxes = { };
	
	UPROPERTY()
	FOnVisibilityChanged onShow;
	UPROPERTY()
	FOnVisibilityChanged onHide;

	bool bIsGamepad = false;

public:
	FORCEINLINE FOnVisibilityChanged& OnShow() { return onShow; }
	FORCEINLINE FOnVisibilityChanged& OnHide() { return onHide; }
	
private:
	virtual void NativeConstruct() override;
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void InitializeWidget();
	
	UFUNCTION(BlueprintImplementableEvent) void OnKeyboardVisual(); 
	UFUNCTION(BlueprintImplementableEvent) void OnGamepadVisual(); 
	UFUNCTION(BlueprintImplementableEvent) void OnUpdateGamepadFocus(); 
	UFUNCTION(BlueprintImplementableEvent) void OnUpdateKeyboardFocus();
	
	UFUNCTION(BlueprintCallable) void CheckGamepadButtonsFocus(const FButtonStyle& _normalButtonStyle, const FButtonStyle& _focusedButtonStyle, const FLinearColor& _normalColor, const FLinearColor& _focusedColor);
	UFUNCTION(BlueprintCallable) void CheckGamepadSlidersFocus(const FLinearColor& _normalColor, const FLinearColor& _focusedColor);
	UFUNCTION(BlueprintCallable) void CheckGamepadComboBoxesFocus(const FComboBoxStyle& _normalComboBoxStyle, const FComboBoxStyle& _focusedComboBoxStyle, const FTableRowStyle& _itemStyle, const FLinearColor& _normalColor, const FLinearColor& _focusedColor);
	UFUNCTION(BlueprintCallable) void CheckGamepadCheckBoxesFocus(const FLinearColor& _normalColor, const FLinearColor& _focusedColor);
	
	UFUNCTION(BlueprintCallable) void SetButtonsStyle(const FButtonStyle& _style);
	UFUNCTION(BlueprintCallable) void SetComboBoxesStyle(const FComboBoxStyle& _comboBoxStyle, const FTableRowStyle& _comboBoxItemStyle);	
	
public:
	//Set the first Widget element to have the Keyboard Focus
	virtual void InitializeFocus();

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	
	//Initialize Widget display for Keyboard
	UFUNCTION() virtual void InitializeKeyboardVisual();
	//Initialize Widget display for Gamepad
	UFUNCTION() virtual void InitializeGamepadVisual();
};
