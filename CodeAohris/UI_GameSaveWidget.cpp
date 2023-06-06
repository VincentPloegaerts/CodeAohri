#include "UI_GameSaveWidget.h"

#include "HUD_Main.h"
#include "M_GameInstance.h"
#include "S_GameSave.h"
#include "UI_ConfirmWidget.h"

#include "IU_Macros.h"

void UUI_GameSaveWidget::InitializeGameSaveWidget(const int& _index, const US_GameSave* _gameSave, const bool& _bDisableEmpty)
{
	bDisableEmpty = _bDisableEmpty;
	iGameIndex = _index;
	gameText->SetText(FText::FromString("Game " + FString::FromInt(iGameIndex + 1)));
	FString _infoText = "Empty";
	
	if (_gameSave)
	{
		const EBossPhase& _phase = _gameSave->GetBossPhase();
		if (_phase == EBossPhase::UN)
			_infoText = "Boss Purple Desert";
		if (_phase == EBossPhase::DEUX)
			_infoText = "Boss Jungle Town";
		if (_phase == EBossPhase::TROIS)
			_infoText = "Boss Giant Temple";
	}
	
	gameButton->SetIsEnabled(_gameSave || !bDisableEmpty);
	deleteButton->SetIsEnabled(_gameSave != nullptr);	
	gameInfoText->SetText(FText::FromString(_infoText));
}

void UUI_GameSaveWidget::InitializeFocus()
{
	gameButton->SetKeyboardFocus();
}

void UUI_GameSaveWidget::InitializeWidget()
{
	gameButton->OnClicked.AddUniqueDynamic(this, &UUI_GameSaveWidget::OnSelected);
	deleteButton->OnClicked.AddUniqueDynamic(this, &UUI_GameSaveWidget::DeleteSave);
}

void UUI_GameSaveWidget::OnSelected()
{
	onGameSelected.Broadcast(iGameIndex);
}

void UUI_GameSaveWidget::DeleteSave()
{
	if (AHUD_Main* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>())
	if (UUI_ConfirmWidget* _widget = _hud->GetConfirmWidget())
	{
		_widget->InitializeConfirmBox(confirmTitle, confirmText);
		_widget->OnConfirm().AddUniqueDynamic(this, &UUI_GameSaveWidget::ConfirmDeleteSave);
	}
	else
		ConfirmDeleteSave();
}

void UUI_GameSaveWidget::ConfirmDeleteSave()
{
	if (UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		_gi->DeleteGameSave(iGameIndex);
		onGameDeleted.Broadcast();
	}
}
