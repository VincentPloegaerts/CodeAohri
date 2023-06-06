#pragma once
#include "CoreMinimal.h"
#include "UI_UserWidget.h"
#include "STRUCT_BindData.h"

#include "Components/InputKeySelector.h"
#include "Components/TextBlock.h"

#include "UI_BindWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_BindWidget : public UUI_UserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UInputKeySelector* keyboardIK = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UInputKeySelector* gamepadIK = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Bind widget Values")
		UTextBlock* bindText = nullptr;

private:
	DECLARE_DELEGATE_OneParam(FOnBindKeyChanged, const FKey&)
		FOnBindKeyChanged onBindKeyChanged = FOnBindKeyChanged();
	
public:
	FORCEINLINE FOnBindKeyChanged* OnBindKeyChanged() { return &onBindKeyChanged; }
	
	FORCEINLINE UInputKeySelector* GetInputKeyForKeyboard() const { return keyboardIK; }
	FORCEINLINE UInputKeySelector* GetInputKeyForGamepad() const { return gamepadIK; }
	
	FORCEINLINE void SetBindName(const FName& _name) const
	{
		if (!bindText) return;
		bindText->SetText(FText::FromName(_name));
	}
	FORCEINLINE bool IsValidKey(const FKey& _key, bool _isKeyboard) const
	{
		return _isKeyboard != _key.IsGamepadKey();
	}
	
private:
	virtual void InitializeWidget() override;

	UFUNCTION(BlueprintCallable) void KeyboardInputKeyChanged(FInputChord _input);
	UFUNCTION(BlueprintCallable) void GamepadInputKeyChanged(FInputChord _input);
};