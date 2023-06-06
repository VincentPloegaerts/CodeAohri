#include "UI_ScalabilitySelectorWidget.h"

void UUI_ScalabilitySelectorWidget::InitializeScalabilityWidget(const EScalabilityType& _type, const int32 _defaultValue)
{
	Super::InitializeScalabilityWidget(_type, _defaultValue);
	selectorText->SetText(FText::FromString(scalabilityName[_defaultValue]));
}

void UUI_ScalabilitySelectorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	lowerButton->OnClicked.AddUniqueDynamic(this, &UUI_ScalabilitySelectorWidget::DecreaseScalability);
	higherButton->OnClicked.AddUniqueDynamic(this, &UUI_ScalabilitySelectorWidget::IncreaseScalability);
}

void UUI_ScalabilitySelectorWidget::IncreaseScalability()
{
	if (scalabilityValue >= 4) return;
	scalabilityValue++;
	SelectScalability(scalabilityValue);
}
void UUI_ScalabilitySelectorWidget::DecreaseScalability()
{
	if (scalabilityValue <= 0) return;
	scalabilityValue--;
	SelectScalability(scalabilityValue);
}
void UUI_ScalabilitySelectorWidget::SelectScalability(const int32& _value)
{
	selectorText->SetText(FText::FromString(scalabilityName[_value]));
	ScalabilitySelect(_value);
}