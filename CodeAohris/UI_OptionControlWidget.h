#pragma once

#include "CoreMinimal.h"
#include "UI_UserWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"

#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"

#include "STRUCT_InputData.h"
#include "UI_GameInputInfoWidget.h"
#include "UI_RebindFailInfoWidget.h"
#include "Components/GridPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "UI_OptionControlWidget.generated.h"

class UM_InputManager;
class UUI_GameInputActionWidget;
class UUI_GameInputAxisWidget;
 
UCLASS()
class INSIDEUS_API UUI_OptionControlWidget : public UUI_UserWidget
{
	GENERATED_BODY()

protected:	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* inputSB = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCheckBox* invertXCB = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCheckBox* invertYCB = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCheckBox* controllerVibrationCB = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* mouseSlider = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* gamepadXSlider = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* gamepadYSlider = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* aimSlider = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* keyboardModeButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* keyboardModeText = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* resetInputButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* resetInputPB = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* resetInputHB = nullptr;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* mouseVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* gamepadXVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* gamepadYVB = nullptr;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UUI_RebindFailInfoWidget* RebindFailInfoWidget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* RebindFailAnimation = nullptr;
	
private:
	UPROPERTY(EditAnywhere, Category = "Control Widget | Settings", meta = (UIMin = 0))
	float resetHoldFactor = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Control Widget | Settings")
	TSubclassOf<UUI_GameInputActionWidget> gameInputActionWidgetType;
	UPROPERTY(EditAnywhere, Category = "Control Widget | Settings")
	TSubclassOf<UUI_GameInputAxisWidget> gameInputAxisWidgetType;
	UPROPERTY(EditAnywhere, Category = "Control Widget | Settings")
	TSubclassOf<UUI_GameInputInfoWidget> gameInputInfoWidgetType;

	UPROPERTY(EditAnywhere, Category = "Control Widget | Settings")
	FString resetInputConfirmTitle = "Reset Input to default ?";
	UPROPERTY(EditAnywhere, Category = "Control Widget | Settings")
	FString resetInputConfirmText = "This action will reset input tu default values. Can not be undone";

	UPROPERTY()
	bool keyboardQwertyMode = false;
	UPROPERTY()
	bool holdingReset = false;
	UPROPERTY()
	UM_InputManager* inputManager = nullptr;
	UPROPERTY()
	class US_PlayerSave* playerSave = nullptr;
	UPROPERTY()
	TArray<UUI_GameInputInfoWidget*> resetInputWidgets = { };
	
public:
	virtual void InitializeFocus() override;

	virtual void InitializeKeyboardVisual() override;
	virtual void InitializeGamepadVisual() override;

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	
private:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void InitializeWidget() override;
	void InitializeInput();
	void InitializeWidgets();

	void InitializeResetGameInputWidgets();
	void InitializeGameInputWidgets(UM_InputManager* _inputManager);
	
	void InitializeGameInputActionWidgets(UM_InputManager* _inputManager, const TArray<FInputActionData>& _actionInputs);
	void InitializeGameInputAxisWidgets(UM_InputManager* _inputManager, const TArray<FInputAxisData>& _axisInputs);
	
	UFUNCTION() void InvertXAxisChanged(bool _invert);
	UFUNCTION() void InvertYAxisChanged(bool _invert);
	UFUNCTION() void MouseSensitivityChanged(float _value);
	UFUNCTION() void GamepadSensitivityXChanged(float _value);
	UFUNCTION() void GamepadSensitivityYChanged(float _value);
	UFUNCTION() void AimSensitivityChanged(float _value);

	UFUNCTION() void SetControllerVibration(bool _enable);
	
	UFUNCTION() void SwitchKeyboardMode();
	
	UFUNCTION() void BeginResetInputHolding();
	UFUNCTION() void StopResetInputHolding();
	void UpdateResetInputBar(const float _deltaTime);
	UFUNCTION() void ResetInput();

	UFUNCTION() void ConfirmResetInput();

	UFUNCTION() void RebindInputFailed(FName _inputName, FKey _key, FString _reason);
};