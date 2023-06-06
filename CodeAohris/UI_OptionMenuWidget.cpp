#include "UI_OptionMenuWidget.h"

#include "STRUCT_GameInput.h"

#include "HUD_Main.h"

#include "UI_OptionSoundWidget.h"
#include "UI_OptionGraphicWidget.h"
#include "UI_OptionControlWidget.h"
#include "UI_OptionAccessibilityWidget.h"

#include "IU_Macros.h"
#include "M_GameInstance.h"
#include "M_InputManager.h"

void UUI_OptionMenuWidget::InitializeWidget()
{
	Super::InitializeWidget();
	soundButton->OnClicked.AddUniqueDynamic(this, &UUI_OptionMenuWidget::ShowSoundWidget);	
	graphicButton->OnClicked.AddUniqueDynamic(this, &UUI_OptionMenuWidget::ShowGraphicWidget);	
	controlButton->OnClicked.AddUniqueDynamic(this, &UUI_OptionMenuWidget::ShowControlWidget);
	accessibilityButton->OnClicked.AddUniqueDynamic(this, &UUI_OptionMenuWidget::ShowAccessibilityWidget);
	
	InitializeWidgets();
	UpdateWidgetVisibility();

	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		if (UM_InputManager* _inputManager = _gi->GetInputManager())
		{
			_inputManager->OnInputReset().AddUniqueDynamic(this, &UUI_OptionMenuWidget::UpdateOnInputReset);
			InitializeInput(_inputManager);
			UpdateOnInputReset(_inputManager);
		}
	}
}

void UUI_OptionMenuWidget::InitializeInput(UM_InputManager* _inputManager)
{
	if (!_inputManager) return;
	
	if (UInputComponent* _component =  GetWorld()->GetFirstPlayerController()->InputComponent)
	{
		const FGameInput& _gameInput = _inputManager->GetGameInput();
		const FInputActionData& _rightInput = _gameInput.UIRightAction;
		FInputActionBinding& _menuRightBinding = _component->BindAction(_rightInput.ActionName, IE_Pressed, this, &UUI_OptionMenuWidget::GoToPreviousPanel);
		_menuRightBinding.bExecuteWhenPaused = true;
		_menuRightBinding.bConsumeInput = false;

		const FInputActionData& _leftInput = _gameInput.UILeftAction;
		FInputActionBinding& _menuLeftBinding = _component->BindAction(_leftInput.ActionName, IE_Pressed, this, &UUI_OptionMenuWidget::GoToNextPanel);
		_menuLeftBinding.bExecuteWhenPaused = true;
		_menuLeftBinding.bConsumeInput = false;
	}
}

void UUI_OptionMenuWidget::InitializeWidgets()
{
	optionGraphicWidget = CreateOptionWidget(optionGraphicWidgetType);
	optionControlWidget = CreateOptionWidget(optionControlWidgetType);
	optionSoundWidget = CreateOptionWidget(optionSoundWidgetType);
	optionAccessibilityWidget = CreateOptionWidget(optionAccessibilityWidgetType);
}

void UUI_OptionMenuWidget::InitializeFocus()
{
	UpdateWidgetVisibility();
}

void UUI_OptionMenuWidget::InitializeKeyboardVisual()
{
	Super::InitializeKeyboardVisual();
	
	for (UUI_GameInputInfoWidget* _widget : gameInputInfoWidgets)
		_widget->SetVisibility(_widget->GetKey().IsGamepadKey() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);		

	for (const auto& current : optionWidgets)
		if (UUI_UserWidget* _widget = current.Value)
			_widget->InitializeKeyboardVisual();
}
void UUI_OptionMenuWidget::InitializeGamepadVisual()
{
	Super::InitializeGamepadVisual();
	
	for (UUI_GameInputInfoWidget* _widget : gameInputInfoWidgets)
		_widget->SetVisibility(_widget->GetKey().IsGamepadKey() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);	
	
	for (const auto& current : optionWidgets)
		if (UUI_UserWidget* _widget = current.Value)
			_widget->InitializeGamepadVisual();

	optionWidgets[iWidgetIndex]->InitializeFocus();
}

void UUI_OptionMenuWidget::UpdateOptionMenuBar(int _index)
{
	graphicButton->SetColorAndOpacity(_index == 0 ? currentColor : notCurrentColor);
	controlButton->SetColorAndOpacity(_index == 1 ? currentColor : notCurrentColor);
	soundButton->SetColorAndOpacity(_index == 2 ? currentColor : notCurrentColor);
	accessibilityButton->SetColorAndOpacity(_index == 3 ? currentColor : notCurrentColor);
}

void UUI_OptionMenuWidget::UpdateWidgetVisibility()
{
	const int& _max = optionWidgets.Num() - 1;
	if (_max < 0) return;

	iWidgetIndex = iWidgetIndex > _max ? 0 : iWidgetIndex < 0 ? _max : iWidgetIndex;
	UpdateOptionMenuBar(iWidgetIndex);
	
	HideWidgets();
	
	if (UUI_UserWidget* _widget = optionWidgets[iWidgetIndex])
	{
		_widget->SetVisibility(ESlateVisibility::Visible);
		if (hud && hud->IsGamepadVisual())
			_widget->InitializeFocus();
	}
}

void UUI_OptionMenuWidget::UpdateOnInputReset(UM_InputManager* _inputManager)
{
	if (!_inputManager || !gameInputInfoWidgetType.Get()) return;

	gameInputInfoWidgets.Empty();
	const FGameInput& _gameInput = _inputManager->GetGameInput();
	
	leftOverlay->ClearChildren();
	const FInputActionData& _leftInput = _gameInput.UILeftAction;
	const TArray<FInputActionKeyMapping>& _leftMappings = _leftInput.Mappings;
	for (const FInputActionKeyMapping& _mapping : _leftMappings)
	{
		UUI_GameInputInfoWidget* _widget = CreateWidget<UUI_GameInputInfoWidget>(this, gameInputInfoWidgetType);
		_widget->InitializeInputInfoWidget(_mapping.Key);
		gameInputInfoWidgets.Add(_widget);
		leftOverlay->AddChildToOverlay(_widget);
	}

	rightOverlay->ClearChildren();
	const FInputActionData& _rightInput = _gameInput.UIRightAction;
	const TArray<FInputActionKeyMapping>& _rightMappings = _rightInput.Mappings;
	for (const FInputActionKeyMapping& _mapping : _rightMappings)
	{
		UUI_GameInputInfoWidget* _widget = CreateWidget<UUI_GameInputInfoWidget>(this, gameInputInfoWidgetType);
		_widget->InitializeInputInfoWidget(_mapping.Key);
		gameInputInfoWidgets.Add(_widget);
		rightOverlay->AddChildToOverlay(_widget);
	}

	if (hud->IsGamepadVisual())
		InitializeGamepadVisual();
	else
		InitializeKeyboardVisual();		
}

void UUI_OptionMenuWidget::HideWidgets()
{
	for (const auto& current : optionWidgets)
		if (UUI_UserWidget* _widget = current.Value)
			_widget->SetVisibility(ESlateVisibility::Hidden);	
}

void UUI_OptionMenuWidget::ShowGraphicWidget()
{
	iWidgetIndex = GetWidgetIndex(optionGraphicWidget);
	UpdateWidgetVisibility();	
}
void UUI_OptionMenuWidget::ShowControlWidget()
{
	iWidgetIndex = GetWidgetIndex(optionControlWidget);
	UpdateWidgetVisibility();
}

void UUI_OptionMenuWidget::ShowAccessibilityWidget()
{
	iWidgetIndex = GetWidgetIndex(optionAccessibilityWidget);
	UpdateWidgetVisibility();
}

void UUI_OptionMenuWidget::ShowSoundWidget()
{
	iWidgetIndex = GetWidgetIndex(optionSoundWidget);	
	UpdateWidgetVisibility();
}

int32 UUI_OptionMenuWidget::GetWidgetIndex(const UUI_UserWidget* _widget) const
{
	for (const auto& _current : optionWidgets)
		if (_current.Value == _widget)
			return _current.Key;
	
	return -1;
}

void UUI_OptionMenuWidget::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	if (InVisibility == ESlateVisibility::Hidden)
		HideWidgets();
	if (InVisibility == ESlateVisibility::Visible)
		UpdateWidgetVisibility();
}

void UUI_OptionMenuWidget::GoToPreviousPanel()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;
	iWidgetIndex++;
	UpdateWidgetVisibility();
}
void UUI_OptionMenuWidget::GoToNextPanel()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;
	iWidgetIndex--;
	UpdateWidgetVisibility();
}