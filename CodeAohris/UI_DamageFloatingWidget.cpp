#include "UI_DamageFloatingWidget.h"

void AUI_DamageFloatingWidget::SetType(const float _damage, const EHitFeedbackEnum _feedbackType) const
{
	const FColor& _color = _feedbackType == HIT_BOSS ? bossColor
						: _feedbackType == HIT_GPE ? gpeColor
						: playerColor;

	floatingText->SetTime(fLifeSpan);
	floatingText->SetColor(_color);
	floatingText->SetText(FString::FromInt(_damage));
}