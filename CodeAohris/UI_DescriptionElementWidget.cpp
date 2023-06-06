#include "UI_DescriptionElementWidget.h"

void UUI_DescriptionElementWidget::InitializeDescriptionElementWidget(const FString& _description)
{
	descriptionText->SetText(FText::FromString(_description));
}
