#include "UI_ScalabilityWidget.h"

void UUI_ScalabilityWidget::InitializeScalabilityWidget(const EScalabilityType& _type, const int32 _defaultValue)
{
	FString _text = *UEnum::GetValueAsString<EScalabilityType>(_type) + FString(" : ");
	_text = _text.Replace(*FString("_"), *FString(" "));
	scalabilityText->SetText(FText::FromString(_text));
	scalabilityType	 = _type;
	scalabilityValue = _defaultValue;
}
void UUI_ScalabilityWidget::SetSpacerSize(const FVector2D& _size) const
{
	spacerTop->SetSize(_size);
	spacerBottom->SetSize(_size);
}

void UUI_ScalabilityWidget::ScalabilitySelect(const int32& _value) const
{
	onScalabilitySelect.Broadcast(scalabilityType, _value);
}