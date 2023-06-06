#include "HUD_Main.h"

#include "STRUCT_GameInput.h"

#include "UI_Widget.h"
#include "UI_ConfirmWidget.h"
#include "UI_DifficultyMenuWidget.h"

#include "IU_Macros.h" 
#include "UI_PopUpWidgetWidget.h"

UUI_Widget* AHUD_Main::GetWidgetUI(const EWidgetEnum& _widgetEnum)
{
	if (!WidgetExist(_widgetEnum)) return nullptr;
	
	return widgets[_widgetEnum];
}

void AHUD_Main::ShowMenuWidget(const EWidgetEnum& _widgetEnum, const bool _playShowAnimation)
{
	if (!WidgetExist(_widgetEnum)) return;
	
	if (menuWidgetHeap.Num() > 0)
		visibleMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	visibleMenuWidget = widgets[_widgetEnum];
	menuWidgetHeap.Add(visibleMenuWidget);
	visibleMenuWidget->SetVisibility(ESlateVisibility::Visible);

	if (_playShowAnimation)
		visibleMenuWidget->PlayShowAnimation();
	
	if (bGamepadKey)
		visibleMenuWidget->InitializeFocus();
}

void AHUD_Main::GoBackMenu(const bool _playShowAnimation)
{
	visibleMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	menuWidgetHeap.RemoveAt(menuWidgetHeap.Num() - 1);
	if (menuWidgetHeap.Num() == 0) return;
	visibleMenuWidget = menuWidgetHeap.Last();
	visibleMenuWidget->SetVisibility(ESlateVisibility::Visible);
	
	if (_playShowAnimation)
		visibleMenuWidget->PlayShowAnimation();
	
	if (bGamepadKey)
		visibleMenuWidget->InitializeFocus();
}

UUI_ConfirmWidget* AHUD_Main::GetConfirmWidget()
{
	if (!WidgetExist(WIDGET_CONFIRM)) return nullptr;
	ShowMenuWidget(WIDGET_CONFIRM);
	return Cast<UUI_ConfirmWidget>(visibleMenuWidget);
}

UUI_DifficultyMenuWidget* AHUD_Main::GetDifficultyWidget()
{
	if (!WidgetExist(WIDGET_DIFFICULTY)) return nullptr;
	ShowMenuWidget(WIDGET_DIFFICULTY);
	return Cast<UUI_DifficultyMenuWidget>(visibleMenuWidget);
}

UUI_PopUpWidgetWidget* AHUD_Main::GetPopUpWidgetWidget()
{
	if (!WidgetExist(WIDGET_POPUP)) return nullptr;
	ShowMenuWidget(WIDGET_POPUP);
	return Cast<UUI_PopUpWidgetWidget>(visibleMenuWidget);
}

bool AHUD_Main::WidgetExist(const EWidgetEnum& _widgetEnum)
{
	if (!widgets.Contains(_widgetEnum))
		CreateWidgetHUD(_widgetEnum);
	
	return widgets.Contains(_widgetEnum); 
}
void AHUD_Main::CreateWidgetHUD(const EWidgetEnum& _enum)
{
	if (!widgetTypes.Contains(_enum) || widgets.Contains(_enum)) return;	//Type doens't exist or already created 

	if (widgetTypes[_enum].Get())
	{
		if (UUI_Widget* _widget = CreateWidget<UUI_Widget>(GetWorld(), widgetTypes[_enum]))
		{
			onKeyboardKey.AddUniqueDynamic(_widget, &UUI_Widget::InitializeKeyboardVisual);
			onGamepadKey.AddUniqueDynamic(_widget, &UUI_Widget::InitializeGamepadVisual);
	
			_widget->AddToViewport();
			_widget->SetVisibility(ESlateVisibility::Hidden);
			if (!widgets.Contains(_enum))
				widgets.Add(_enum, nullptr);
	
			widgets[_enum] = _widget;
		}
	}
	else
		LOG("HUD::CreateWidgetHUD => WidgetType is not valid for %s", *UEnum::GetValueAsString<EWidgetEnum>(_enum));
}

void AHUD_Main::UpdateKeyType(FKey _key)
{
	if (_key.IsGamepadKey())	//If Input is Gamepad Key
	{
		if (!bGamepadKey)		//If last input wasn't a Gamepad Key
		{
			ToGamepad();
		}
	}
	else						//Else Input isn't a Gamepad Key (is a Keyboard Key)
	{
		if (bGamepadKey)		//If last input was a Gamepad Key
		{
			ToKeyboard();
		}
	}
}

void AHUD_Main::UpdateKeyTypeFromMouse(float _axis)
{
	if (!bGamepadKey) return;
	const float& _value = abs(_axis);
	if (_value > 0.1f)
	{
		ToKeyboard();
	}
}

void AHUD_Main::CheckMouseCursor()
{
	if (!bGamepadKey || !playerController) return;
	
	float _x, _y = 0;
	playerController->GetMousePosition(_x, _y);
	const float& _deltaX = abs(mouseX - _x);
	const float& _deltaY = abs(mouseY - _y);

	if (_deltaX > 0.1f || _deltaY > 0.1f)
		ToKeyboard();
}

void AHUD_Main::ToGamepad()
{
	if (playerController)
		playerController->GetMousePosition(mouseX, mouseY);
	
	bGamepadKey = true;
	onGamepadKey.Broadcast();
	if (visibleMenuWidget) visibleMenuWidget->InitializeFocus();
}
void AHUD_Main::ToKeyboard()
{
	bGamepadKey = false;
	onKeyboardKey.Broadcast();
	if (visibleMenuWidget) visibleMenuWidget->SetKeyboardFocus();
}

void AHUD_Main::BeginPlay()
{
	Super::BeginPlay();
	playerController = GetWorld()->GetFirstPlayerController();
	if (playerController)
		InputComponent = playerController->InputComponent;
	
	InitializeWidgets();
	InitializeInput();
	InitializeHUD();

	FTimerHandle _checkTimer;
	GetWorld()->GetTimerManager().SetTimer(_checkTimer, this, &AHUD_Main::CheckMouseCursor, 0.1f, true);
}

void AHUD_Main::InitializeWidgets()
{
	for (auto& current : widgetTypes)
		CreateWidgetHUD(current.Key);
	
	onKeyboardKey.Broadcast();
}

void AHUD_Main::InitializeInput()
{
	if (!playerController) return;
	
	UInputComponent* _component =  playerController->InputComponent;
	if (!_component ) return;
	
	const FGameInput& _input = FGameInput();
	FInputActionBinding& _anyKeyBind = _component->BindAction(_input.AnyKeyAction.ActionName, IE_Pressed, this, &AHUD_Main::UpdateKeyType);
	_anyKeyBind.bConsumeInput = false;
	_anyKeyBind.bExecuteWhenPaused = true;
}
void AHUD_Main::InitializeHUD()
{
	//Declaration in child class...
}

void AHUD_Main::ShowInitialWidget()
{
	//Declaration in child class...
}
