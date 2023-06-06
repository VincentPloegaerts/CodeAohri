#include "UI_PopUpWidgetWidget.h"

void UUI_PopUpWidgetWidget::InitializeFocus()
{
	Super::InitializeFocus();
	backButton->SetKeyboardFocus();
}

void UUI_PopUpWidgetWidget::InitializePopUpWidget(const FString& _titleText, const FString& _text) const
{
	title->SetText(FText::FromString(_titleText));
	text->SetText(FText::FromString(_text));
}
