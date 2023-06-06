#include "UI_OptionControlWidget.h"

#include "HUD_Main.h"
#include "M_GameInstance.h"

#include "S_PlayerSave.h"

#include "UI_GameInputActionWidget.h"
#include "UI_GameInputAxisWidget.h"

#include "UI_ConfirmWidget.h"

#include "IU_Macros.h"

void UUI_OptionControlWidget::InitializeFocus()
{
	invertXCB->SetKeyboardFocus();
}

void UUI_OptionControlWidget::InitializeKeyboardVisual()
{
	Super::InitializeKeyboardVisual();
	mouseVB->SetVisibility(ESlateVisibility::Visible);
	gamepadXVB->SetVisibility(ESlateVisibility::Collapsed);
	gamepadYVB->SetVisibility(ESlateVisibility::Collapsed);

	for (UUI_GameInputInfoWidget* _widget : resetInputWidgets)
		_widget->SetVisibility(_widget->GetKey().IsGamepadKey() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}
void UUI_OptionControlWidget::InitializeGamepadVisual()
{
	Super::InitializeGamepadVisual();
	mouseVB->SetVisibility(ESlateVisibility::Collapsed);
	gamepadXVB->SetVisibility(ESlateVisibility::Visible);
	gamepadYVB->SetVisibility(ESlateVisibility::Visible);

	for (UUI_GameInputInfoWidget* _widget : resetInputWidgets)
		_widget->SetVisibility(_widget->GetKey().IsGamepadKey() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UUI_OptionControlWidget::SetVisibility(ESlateVisibility InVisibility)
{
	if (GetVisibility() != ESlateVisibility::Hidden)
		if (InVisibility == ESlateVisibility::Hidden)
		{
			if (playerSave)
				playerSave->PlayerSaveEdited();
			
			StopResetInputHolding();
			resetInputPB->SetPercent(0);
		}
	
	Super::SetVisibility(InVisibility);
}

void UUI_OptionControlWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateResetInputBar(InDeltaTime);
}

void UUI_OptionControlWidget::InitializeWidget()
{
	invertXCB->OnCheckStateChanged.AddUniqueDynamic(this, &UUI_OptionControlWidget::InvertXAxisChanged);
	invertYCB->OnCheckStateChanged.AddUniqueDynamic(this, &UUI_OptionControlWidget::InvertYAxisChanged);
	mouseSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionControlWidget::MouseSensitivityChanged);
	gamepadXSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionControlWidget::GamepadSensitivityXChanged);
	gamepadYSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionControlWidget::GamepadSensitivityYChanged);
	aimSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionControlWidget::AimSensitivityChanged);
	resetInputButton->OnClicked.AddUniqueDynamic(this, &UUI_OptionControlWidget::ResetInput);

	controllerVibrationCB->OnCheckStateChanged.AddUniqueDynamic(this, &UUI_OptionControlWidget::SetControllerVibration);
	
	keyboardModeButton->OnClicked.AddUniqueDynamic(this, &UUI_OptionControlWidget::SwitchKeyboardMode);
	
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		playerSave = _gi->GetPlayerSave();
		if (playerSave)
		{
			const FCameraConfig& _config = playerSave->GetCameraConfig();
			invertXCB->SetIsChecked(_config.GetInvertAxisX());
			invertYCB->SetIsChecked(_config.GetInvertAxisY());
			mouseSlider->SetValue(_config.GetMouseSensitivity());
			gamepadXSlider->SetValue(_config.GetGamepadSensitivityX());
			gamepadYSlider->SetValue(_config.GetGamepadSensitivityY());
			aimSlider->SetValue(_config.GetAimSensitivity());
			controllerVibrationCB->SetIsChecked(playerSave->GetPlayerConfig().GetVibrationEnable());
		}
	}

	InitializeInput();
	InitializeWidgets();
}

void UUI_OptionControlWidget::InitializeInput()
{
	UInputComponent* _component =  GetWorld()->GetFirstPlayerController()->InputComponent;
	if (!_component ) return;

	const FGameInput& _gameInput = FGameInput();
	FInputActionBinding& _resetBeginInput = _component->BindAction(_gameInput.UIResetInputAction.ActionName, IE_Pressed, this, &UUI_OptionControlWidget::BeginResetInputHolding);
	_resetBeginInput.bExecuteWhenPaused = true;
	_resetBeginInput.bConsumeInput = false;
	FInputActionBinding& _resetStopInput = _component->BindAction(_gameInput.UIResetInputAction.ActionName, IE_Released, this, &UUI_OptionControlWidget::StopResetInputHolding);
	_resetStopInput.bExecuteWhenPaused = true;
	_resetStopInput.bConsumeInput = false;
}

void UUI_OptionControlWidget::InitializeWidgets()
{
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		inputManager = _gi->GetInputManager();
		if (!inputManager) return;

		InitializeGameInputWidgets(inputManager);
		InitializeResetGameInputWidgets();
		
		keyboardQwertyMode = inputManager->GetGameInput().IsQwerty;
		keyboardModeText->SetText(FText::FromString(keyboardQwertyMode ? "QWERTY" : "AZERTY"));
	}
}

void UUI_OptionControlWidget::InitializeResetGameInputWidgets()
{
	if (gameInputInfoWidgetType.Get())
	{
		const FGameInput& _input = inputManager->GetGameInput();
		const TArray<FInputActionKeyMapping>& _mappings = _input.UIResetInputAction.Mappings;
		for (const FInputActionKeyMapping& _mapping : _mappings)
		{
			UUI_GameInputInfoWidget* _widget = CreateWidget<UUI_GameInputInfoWidget>(this, gameInputInfoWidgetType);
			_widget->InitializeInputInfoWidget(_mapping.Key);
			resetInputHB->AddChildToHorizontalBox(_widget);
			resetInputWidgets.Add(_widget);
		}
	}
}

void UUI_OptionControlWidget::InitializeGameInputWidgets(UM_InputManager* _inputManager)
{
	InitializeGameInputAxisWidgets(_inputManager, _inputManager->GetRebindableInputAxis());
	InitializeGameInputActionWidgets(_inputManager, _inputManager->GetRebindableInputAction());
}

void UUI_OptionControlWidget::InitializeGameInputActionWidgets(UM_InputManager* _inputManager, const TArray<FInputActionData>& _actionInputs)
{
	if (gameInputActionWidgetType.Get())
	{
		for (const FInputActionData& actionInput : _actionInputs)
		{
			UUI_GameInputActionWidget* _widget = CreateWidget<UUI_GameInputActionWidget>(GetWorld(), gameInputActionWidgetType);
			_widget->InitializeGameInputActionWidget(_inputManager, actionInput);
			_widget->OnRebindInputFailed().AddUniqueDynamic(this, &UUI_OptionControlWidget::RebindInputFailed);
			inputSB->AddChild(_widget);
		}
	}
}
void UUI_OptionControlWidget::InitializeGameInputAxisWidgets(UM_InputManager* _inputManager, const TArray<FInputAxisData>& _axisInputs)
{
	if (gameInputAxisWidgetType.Get())
	{
		for (const FInputAxisData& _axisInput : _axisInputs)
		{
			UUI_GameInputAxisWidget* _widget = CreateWidget<UUI_GameInputAxisWidget>(GetWorld(), gameInputAxisWidgetType);
			_widget->InitializeGameInputAxisWidget(_inputManager, _axisInput);
			_widget->OnRebindInputFailed().AddUniqueDynamic(this, &UUI_OptionControlWidget::RebindInputFailed);
			inputSB->AddChild(_widget);
		}
	}
}

void UUI_OptionControlWidget::InvertXAxisChanged(bool _invert)
{
	if (playerSave)
		playerSave->EditCameraConfig().SetInvertAxisX(_invert);
}
void UUI_OptionControlWidget::InvertYAxisChanged(bool _invert)
{
	if (playerSave)
		playerSave->EditCameraConfig().SetInvertAxisY(_invert);
}

void UUI_OptionControlWidget::MouseSensitivityChanged(float _value)
{
	if (playerSave)
		playerSave->EditCameraConfig().SetMouseSensitivity(_value);
}
void UUI_OptionControlWidget::GamepadSensitivityXChanged(float _value)
{
	if (playerSave)
		playerSave->EditCameraConfig().SetGamepadSensitivityX(_value);
}
void UUI_OptionControlWidget::GamepadSensitivityYChanged(float _value)
{
	if (playerSave)
		playerSave->EditCameraConfig().SetGamepadSensitivityY(_value);
}

void UUI_OptionControlWidget::AimSensitivityChanged(float _value)
{
	if (playerSave)
		playerSave->EditCameraConfig().SetAimSensitivity(_value);
}

void UUI_OptionControlWidget::SetControllerVibration(bool _enable)
{
	playerSave->EditPlayerConfig().SetVibrationEnable(_enable);
}

void UUI_OptionControlWidget::SwitchKeyboardMode()
{
	if (!inputManager) return;
	keyboardQwertyMode = !keyboardQwertyMode;
	keyboardModeText->SetText(FText::FromString(keyboardQwertyMode ? "QWERTY" : "AZERTY"));
	inputManager->SwitchKeyboardInputMode(keyboardQwertyMode);
}

void UUI_OptionControlWidget::BeginResetInputHolding()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;
	holdingReset = true;	
}
void UUI_OptionControlWidget::StopResetInputHolding()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;
	holdingReset = false;	
}
void UUI_OptionControlWidget::UpdateResetInputBar(const float _deltaTime)
{
	float _percent = FMath::Lerp(resetInputPB->GetPercent(), holdingReset ? 1.1f : 0, _deltaTime * resetHoldFactor);

	if (_percent > 1.0f)
	{
		_percent = 0;
		StopResetInputHolding();
		ResetInput();
	}

	resetInputPB->SetPercent(_percent);
}

void UUI_OptionControlWidget::ResetInput()
{
	if (AHUD_Main* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>())
	{
		if (UUI_ConfirmWidget* _confirmWidget = _hud->GetConfirmWidget())
		{
			_confirmWidget->InitializeConfirmBox(resetInputConfirmTitle, resetInputConfirmText);
			_confirmWidget->OnConfirm().AddUniqueDynamic(this, &UUI_OptionControlWidget::ConfirmResetInput);
			return;
		}
	}
	
	ConfirmResetInput();
}

void UUI_OptionControlWidget::ConfirmResetInput()
{
	if (!inputManager) return;
	
	inputManager->ResetInputToDefault();
}

void UUI_OptionControlWidget::RebindInputFailed(FName _inputName, FKey _key, FString _reason)
{
	if (!RebindFailInfoWidget || !RebindFailAnimation) return;

	RebindFailInfoWidget->InitializeText(_inputName, _key, _reason);
	PlayAnimation(RebindFailAnimation);
}