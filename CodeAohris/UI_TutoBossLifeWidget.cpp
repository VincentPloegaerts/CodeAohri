#include "UI_TutoBossLifeWidget.h"
#include "IU_HealthSystemComponent.h"

void UUI_TutoBossLifeWidget::InitializeTutoBossLifeWidget(UIU_HealthSystemComponent* _health)
{
	if (_health)
	{
		_health->OnLifeUpdated()->AddUniqueDynamic(this, &UUI_TutoBossLifeWidget::UpdateLifeBar);
	}
}

UFUNCTION() void UUI_TutoBossLifeWidget::UpdateLifeBar(float _life, float _max)
{
	bossLife->SetPercent(_life / _max);
}
