#include "UI_UserWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"

void UUI_UserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}

void UUI_UserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bIsGamepad)
		OnUpdateGamepadFocus();
	else
		OnUpdateKeyboardFocus();
}

void UUI_UserWidget::CheckGamepadButtonsFocus(const FButtonStyle& _normalButtonStyle, const FButtonStyle& _focusedButtonStyle, const FLinearColor& _normalColor, const FLinearColor& _focusedColor)
{
	const int& _max = Buttons.Num();
	for (int i = 0; i < _max; ++i)
		if (UButton* _button = Buttons[i])
		{
			const bool& _focused = _button->HasKeyboardFocus();
			const FButtonStyle& _style = _focused ? _focusedButtonStyle : _normalButtonStyle; 
			const FLinearColor& _color = _focused ? _focusedColor : _normalColor;
			_button->SetStyle(_style);
			_button->SetColorAndOpacity(_color);
			_button->SetBackgroundColor(_color);
		}
}
void UUI_UserWidget::CheckGamepadSlidersFocus(const FLinearColor& _normalColor, const FLinearColor& _focusedColor)
{
	const int& _max = Sliders.Num();
	for (int i = 0; i < _max; ++i)
		if (USlider* _slider = Sliders[i])
			_slider->SetSliderHandleColor(_slider->HasKeyboardFocus() ? _focusedColor : _normalColor);
}
void UUI_UserWidget::CheckGamepadComboBoxesFocus(const FComboBoxStyle& _normalComboBoxStyle, const FComboBoxStyle& _focusedComboBoxStyle, const FTableRowStyle& _itemStyle, const FLinearColor& _normalColor, const FLinearColor& _focusedColor)
{
	const int& _max = ComboBoxes.Num();
	for (int i = 0; i < _max; ++i)
	{
		const FComboBoxPair& _pair = ComboBoxes[i]; 
		if (UComboBoxString* _box = _pair.ComboBox)
		{
			const bool& _focused = _box->HasKeyboardFocus();
			const bool& _open = _box->IsOpen(); 
			_box->WidgetStyle = _focused && !_open ? _focusedComboBoxStyle : _normalComboBoxStyle;
			if (_pair.Border) _pair.Border->SetContentColorAndOpacity(_focused && !_open ? _focusedColor : _normalColor);
			_box->ItemStyle = _itemStyle;
		}
	}
}
void UUI_UserWidget::CheckGamepadCheckBoxesFocus(const FLinearColor& _normalColor, const FLinearColor& _focusedColor)
{
	const int& _max = CheckBoxes.Num();
	for (int i = 0; i < _max; ++i)
	{
		const FCheckBoxPair& _pair = CheckBoxes[i]; 
		if (UCheckBox* _box = _pair.CheckBox)
		{
			const bool& _focused = _box->HasKeyboardFocus();
			if (_pair.Border) _pair.Border->SetContentColorAndOpacity(_focused ? _focusedColor : _normalColor);
		}
	}
}

void UUI_UserWidget::SetButtonsStyle(const FButtonStyle& _style)
{
	for (UButton* _button : Buttons)
		_button->SetStyle(_style);
}
void UUI_UserWidget::SetComboBoxesStyle(const FComboBoxStyle& _comboBoxStyle, const FTableRowStyle& _comboBoxItemStyle)
{
	for (const FComboBoxPair& _pair : ComboBoxes)
	{
		if (UComboBoxString* _box = _pair.ComboBox)
		{
			_box->WidgetStyle = _comboBoxStyle;
			_box->ItemStyle = _comboBoxItemStyle;
		}
	}
}

void UUI_UserWidget::InitializeWidget()
{
	//Declaration in child class
}

void UUI_UserWidget::InitializeFocus()
{
	//Initialize first Focused element
}

void UUI_UserWidget::SetVisibility(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
		onShow.Broadcast();
	if (InVisibility == ESlateVisibility::Hidden)
		onHide.Broadcast();
	
	Super::SetVisibility(InVisibility);
}

void UUI_UserWidget::InitializeKeyboardVisual()
{
	bIsGamepad = false;
	SetKeyboardFocus();
	OnUpdateGamepadFocus();	//Clear Buttons/Sliders/... Gamepad Focus in BPs
	OnKeyboardVisual();
}

void UUI_UserWidget::InitializeGamepadVisual()
{
	bIsGamepad = true;
	OnGamepadVisual();
}
