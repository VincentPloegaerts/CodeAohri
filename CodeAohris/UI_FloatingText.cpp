#include "UI_FloatingText.h"

void UUI_FloatingText::SetText(const FString& _text) const
{
	floatingText->SetText(FText::FromString(_text));
}

void UUI_FloatingText::SetColor(const FColor& _color) const
{
	floatingText->SetColorAndOpacity(_color);
}