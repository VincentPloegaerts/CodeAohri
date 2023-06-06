#include "UI_Widget.h"

#include "M_GameMode.h"
#include "HUD_Main.h"

void UUI_Widget::InitializeWidget()
{
	hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>();
}

void UUI_Widget::PlayShowAnimation()
{
	PlayAnimation(showAnimation);
}
