#include "UI_PauseMenuWidget.h"

#include "M_GameInstance.h"
#include "M_GameMode.h"

#include "IU_Player.h"
#include "C_RespawnComponent.h"

#include "HUD_Game.h"
#include "HUD_Main.h"

#include "UI_ConfirmWidget.h"

#include "IU_Macros.h"

void UUI_PauseMenuWidget::LockButtons()
{
	checkpointButton->SetIsEnabled(false);
	difficultyButton->SetIsEnabled(false);
}
void UUI_PauseMenuWidget::UnlockButtons()
{
	checkpointButton->SetIsEnabled(true);
	difficultyButton->SetIsEnabled(true);
}

void UUI_PauseMenuWidget::InitializeWidget()
{
	Super::InitializeWidget();

	checkpointButton->OnClicked.AddUniqueDynamic(this, &UUI_PauseMenuWidget::ReturnToCheckpoint);
	difficultyButton->OnClicked.AddUniqueDynamic(this, &UUI_PauseMenuWidget::OpenDifficultyMenu);
	optionButton->OnClicked.AddUniqueDynamic(this, &UUI_PauseMenuWidget::OpenOptionMenu);
	extraButton->OnClicked.AddUniqueDynamic(this, &UUI_PauseMenuWidget::OpenExtraMenu);
	menuButton->OnClicked.AddUniqueDynamic(this, &UUI_PauseMenuWidget::QuitMenu);
	quitButton->OnClicked.AddUniqueDynamic(this, &UUI_PauseMenuWidget::QuitGame);

	FTimerHandle _timerGetPlayer;
	GetWorld()->GetTimerManager().SetTimer(_timerGetPlayer, this, &UUI_PauseMenuWidget::GetPlayer, 0.2f, false);
}

void UUI_PauseMenuWidget::InitializeFocus()
{
	backButton->SetKeyboardFocus();
}

void UUI_PauseMenuWidget::Resume()
{
	if (!hud) return;
	if (AHUD_Game* _hud = Cast<AHUD_Game>(hud))
		_hud->MenuPause();
}

void UUI_PauseMenuWidget::ReturnToCheckpoint()
{
	if (player && player->IsInArena())
	{			
		if (UUI_ConfirmWidget* _widget = hud->GetConfirmWidget())
		{
			_widget->InitializeConfirmBox(returnCheckpointTitle, returnCheckpointText);
			_widget->OnConfirm().AddUniqueDynamic(this, &UUI_PauseMenuWidget::ConfirmReturnToCheckpoint);
			return;
		}
	}

	//else
	ConfirmReturnToCheckpoint();
}

void UUI_PauseMenuWidget::OpenDifficultyMenu()
{
	if (!hud) return;
	hud->ShowMenuWidget(WIDGET_DIFFICULTY);
}

void UUI_PauseMenuWidget::OpenOptionMenu()
{
	if (!hud) return;
	hud->ShowMenuWidget(WIDGET_OPTIONS);
}

void UUI_PauseMenuWidget::OpenExtraMenu()
{
	if (!hud) return;
	hud->ShowMenuWidget(WIDGET_EXTRA);	
}

void UUI_PauseMenuWidget::QuitMenu()
{
	if (UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
		_gi->ReturnToMainMenu();
}
void UUI_PauseMenuWidget::QuitGame()
{
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
		_gi->QuitGame();
}

void UUI_PauseMenuWidget::ConfirmReturnToCheckpoint()
{
	if (player)
	{
		player->RespawnComponent()->Death(false);
		Back();
	}
}

void UUI_PauseMenuWidget::GetPlayer()
{
	if (const AM_GameMode* _gm = GetWorld()->GetAuthGameMode<AM_GameMode>())
		player = _gm->GetPlayer();
}

void UUI_PauseMenuWidget::Back()
{
	if (!hud) return;
	if (AHUD_Game* _hud = Cast<AHUD_Game>(hud))
		_hud->MenuPause();
}
