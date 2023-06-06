#include "UI_BossWidget.h"

#include "B_Boss.h"
#include "HUD_Game.h"
#include "IU_HealthSystemComponent.h"

void UUI_BossWidget::InitializeBossWidget(AB_Boss* _boss)
{
	if (!_boss) return;
	if (AHUD_Game* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Game>())
	{
		if (UIU_HealthSystemComponent* _health = _boss->GetHealthComponent())
		{
			_health->OnDeath()->AddDynamic(_hud, &AHUD_Game::HideBossWidget);
			_health->OnLifeUpdatedWithMaxLife()->AddDynamic(this, &UUI_BossWidget::UpdateBossLifeBar);
			UpdateBossLifeBar(_health->GetHealth(), _health->GetActualMaxHealth());
		}
		_boss->OnBossActivated().AddDynamic(_hud, &AHUD_Game::ShowBossWidget);
		_boss->OnBossReset().AddDynamic(_hud, &AHUD_Game::HideBossWidget);
	}
}

void UUI_BossWidget::UpdateBossLifeBar(float _life, float _maxLife)
{
	bossLifePB->SetPercent(_life / _maxLife);
}