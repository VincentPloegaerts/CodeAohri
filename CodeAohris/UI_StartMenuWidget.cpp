#include "UI_StartMenuWidget.h"

#include "HUD_Main.h"
#include "M_GameInstance.h"

#include "M_UIManager.h"

#include "IU_Macros.h"

void UUI_StartMenuWidget::InitializeWidget()
{
	Super::InitializeWidget();
	continueButton->OnClicked.AddUniqueDynamic(this, &UUI_StartMenuWidget::ContinueGame);
	newGameButton->OnClicked.AddUniqueDynamic(this, &UUI_StartMenuWidget::OpenNewGameMenu);
	loadGameButton->OnClicked.AddUniqueDynamic(this, &UUI_StartMenuWidget::OpenLoadGameMenu);
	optionButton->OnClicked.AddUniqueDynamic(this, &UUI_StartMenuWidget::OpenOptionMenu);
	rushButton->OnClicked.AddUniqueDynamic(this, &UUI_StartMenuWidget::OpenRushModeMenu);	
	extraButton->OnClicked.AddUniqueDynamic(this, &UUI_StartMenuWidget::OpenExtraMenu);
	creditButton->OnClicked.AddUniqueDynamic(this, &UUI_StartMenuWidget::OpenCredit);
	quitButton->OnClicked.AddUniqueDynamic(this, &UUI_StartMenuWidget::Quit);	
}

void UUI_StartMenuWidget::InitializeFocus()
{
	newGameButton->SetKeyboardFocus();
}

void UUI_StartMenuWidget::ContinueGame()
{
	//TODO peut etre
}
void UUI_StartMenuWidget::OpenNewGameMenu()
{
	if (!hud) return;
	hud->ShowMenuWidget(WIDGET_NEW_GAME);
}
void UUI_StartMenuWidget::OpenLoadGameMenu()
{
	if (!hud) return;
	hud->ShowMenuWidget(WIDGET_LOAD_GAME);
}

void UUI_StartMenuWidget::OpenOptionMenu()
{
	if (!hud) return;
	hud->ShowMenuWidget(WIDGET_OPTIONS);
}

void UUI_StartMenuWidget::OpenRushModeMenu()
{
	if (!hud) return;
	hud->ShowMenuWidget(WIDGET_RUSH_MODE);	
}
void UUI_StartMenuWidget::OpenExtraMenu()
{
	if (!hud) return;
	hud->ShowMenuWidget(WIDGET_EXTRA);
}

void UUI_StartMenuWidget::OpenCredit()
{
	if (!hud) return;
	hud->ShowMenuWidget(WIDGET_CREDIT);
}

void UUI_StartMenuWidget::Quit()
{
	if (UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
		_gi->QuitGame();
}
