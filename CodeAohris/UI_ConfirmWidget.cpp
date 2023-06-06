#include "UI_ConfirmWidget.h"

#include "HUD_Main.h"
#include "M_GameMode.h"

void UUI_ConfirmWidget::InitializeWidget()
{
	hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>();
	
	confirmButton->OnClicked.AddUniqueDynamic(this, &UUI_ConfirmWidget::Confirm);
	backButton->OnClicked.AddUniqueDynamic(this, &UUI_ConfirmWidget::Cancel);
}

void UUI_ConfirmWidget::InitializeFocus()
{
	backButton->SetKeyboardFocus();
}

void UUI_ConfirmWidget::Confirm()
{
	if (!hud) return;
	InitializeConfirmBox("Not Initialized", "...");
	hud->GoBackMenu();
	
	onConfirm.Broadcast();
	onConfirm.Clear();
	onCancel.Clear();
}
void UUI_ConfirmWidget::Cancel()
{
	if (!hud) return;
	InitializeConfirmBox("Not Initialized", "...");
	hud->GoBackMenu();

	onCancel.Broadcast();
	onCancel.Clear();
	onConfirm.Clear();
}

void UUI_ConfirmWidget::InitializeConfirmBox(const FString& _titleText, const FString& _text) const
{
	titleText->SetText(FText::FromString(_titleText));
	text->SetText(FText::FromString(_text));
}
