#include "UI_IronModeWidget.h"

#include "HUD_Main.h"
#include "UI_ConfirmWidget.h"

void UUI_IronModeWidget::InitializeDifficultyWidget(const EDifficultyType& _difficulty, const FString& _description, const TArray<FString>& _extraDescription, UTexture2D* _image)
{
	Super::InitializeDifficultyWidget(_difficulty, _description, _extraDescription);
	difficultyText->SetText(FText::FromString(ironModeText));
}

void UUI_IronModeWidget::Refresh()
{
	SetIronMode(false);
}

void UUI_IronModeWidget::InitializeWidget()
{
	hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>();
	difficultyButton->OnHovered.AddUniqueDynamic(this, &UUI_IronModeWidget::HoverDifficulty);
	difficultyButton->OnUnhovered.AddUniqueDynamic(this, &UUI_IronModeWidget::UnhoverDifficulty);
	ironModeCB->OnCheckStateChanged.AddDynamic(this, &UUI_IronModeWidget::ChangeIronMode);	
}

void UUI_IronModeWidget::CheckDifficultyDescription()
{
	if (ironModeCB->HasKeyboardFocus() && !bFocused)
    {
    	bFocused = true;
    	HoverDifficulty();
    }
    if (!ironModeCB->HasKeyboardFocus() && bFocused)
    {
    	bFocused = false;
    	UnhoverDifficulty();
    }
}

void UUI_IronModeWidget::ChangeIronMode(bool _enable)
{
	ironModeCB->SetIsChecked(false);
	if (_enable)
		if (hud)
		{
			if (UUI_ConfirmWidget* _widget = hud->GetConfirmWidget())
			{
				_widget->InitializeConfirmBox(enableIronModeConfirmTitle, enableIronModeConfirmText);	
				_widget->OnConfirm().AddUniqueDynamic(this, &UUI_IronModeWidget::ConfirmIronMode);
			}
		}
		else
			SetIronMode(false);
}
void UUI_IronModeWidget::ConfirmIronMode()
{
	SetIronMode(true);
}
void UUI_IronModeWidget::SetIronMode(bool _enable)
{
	ironModeCB->SetIsChecked(_enable);
	onIronModeSelect.Broadcast(_enable);
}