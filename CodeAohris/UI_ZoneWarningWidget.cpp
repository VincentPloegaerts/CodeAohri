#include "UI_ZoneWarningWidget.h"

void UUI_ZoneWarningWidget::Init(AIU_Player* _player, AIU_Camera* _cam)
{
	player = _player;
	cam = _cam;
}

void UUI_ZoneWarningWidget::AddWarning(AGPE_Pattern* _gpePattern)
{
	if (!warningWidgetType)return;
	UUI_WarningWidget* _warning = Cast<UUI_WarningWidget>(CreateWidget(GetWorld(),warningWidgetType));
	if (!canvas || !_warning)return; //ntm unreal
	canvas->AddChild(_warning);
	_warning->Init(player,cam,_gpePattern);
}

void UUI_ZoneWarningWidget::AddBossWarning(AB_Boss* _boss)
{
	if (!warningBossWidgetType)return;
	UUI_WarningBossWidget* _warning = Cast<UUI_WarningBossWidget>(CreateWidget(GetWorld(),warningBossWidgetType));
	if (!canvas || !_warning)return; //ntm unreal
	canvas->AddChild(_warning);
	_warning->Init(player,cam,_boss);
}
