#include "UI_BackWidget.h"

#include "STRUCT_GameInput.h"

#include "HUD_Main.h"

#include "IU_Macros.h"

void UUI_BackWidget::InitializeWidget()
{
	Super::InitializeWidget();
	backButton->OnClicked.AddUniqueDynamic(this, &UUI_BackWidget::Back);

	UWorld* _w = GetWorld();
	UInputComponent* _component =  _w->GetFirstPlayerController()->InputComponent;
	if (!_component ) return;
	
	const FGameInput& _gameInput = FGameInput();
	FInputActionBinding& _bind = _component->BindAction(_gameInput.UIBackAction.ActionName, IE_Pressed, this, &UUI_BackWidget::BackInput);
	_bind.bExecuteWhenPaused = true;
	_bind.bConsumeInput = false;
}

void UUI_BackWidget::InitializeKeyboardVisual()
{
	Super::InitializeKeyboardVisual();
	keyboardBackText->SetVisibility(ESlateVisibility::Visible);
	gamepadBackText->SetVisibility(ESlateVisibility::Hidden);
}
void UUI_BackWidget::InitializeGamepadVisual()
{
	Super::InitializeGamepadVisual();
	gamepadBackText->SetVisibility(ESlateVisibility::Visible);
	keyboardBackText->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_BackWidget::Back()
{
	if (hud)
		hud->GoBackMenu();

	OnBack.Broadcast();
}

void UUI_BackWidget::BackInput()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;
	Back();
}
