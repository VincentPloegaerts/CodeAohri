#include "UI_DifficultyWidget.h"

#include "M_GameMode.h"
#include "IU_Player.h"

#include "HUD_Main.h"
#include "UI_ConfirmWidget.h"
#include "UI_DescriptionWidget.h"

#include "IU_Macros.h"

void UUI_DifficultyWidget::InitializeWidget()
{
	hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>();
	
	difficultyButton->OnClicked.AddUniqueDynamic(this, &UUI_DifficultyWidget::SelectDifficulty);
	difficultyButton->OnHovered.AddUniqueDynamic(this, &UUI_DifficultyWidget::HoverDifficulty);
	difficultyButton->OnUnhovered.AddUniqueDynamic(this, &UUI_DifficultyWidget::UnhoverDifficulty);
}

void UUI_DifficultyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	CheckDifficultyDescription();
}

void UUI_DifficultyWidget::CheckDifficultyDescription()
{
	if (difficultyButton->HasKeyboardFocus() && !bFocused)
	{
		bFocused = true;
		HoverDifficulty();
	}
	if (!difficultyButton->HasKeyboardFocus() && bFocused)
	{
		bFocused = false;
		UnhoverDifficulty();
	}
}

void UUI_DifficultyWidget::InitializeFocus()
{
	difficultyButton->SetKeyboardFocus();
}

void UUI_DifficultyWidget::InitializeDifficultyWidget(const EDifficultyType& _difficulty, const FString& _description, const TArray<FString>& _extraDescription, UTexture2D* _image)
{
	difficulty = _difficulty;
	
	FString _name = UEnum::GetValueAsString<EDifficultyType>(difficulty);
	_name = _name.Replace(*FString("DIFFICULTY_"), *FString(""));
	difficultyText->SetText(FText::FromString(_name));

	descriptionWidget->InitializeDescriptionWidget(_description, _extraDescription);
	
	difficultyImage->SetBrushFromTexture(_image);
	difficultyImage->SetVisibility(ESlateVisibility::Visible);
}

void UUI_DifficultyWidget::Update(const EDifficultyType& _difficulty)
{
	isCurrent = _difficulty == difficulty;
	difficultyImage->SetOpacity(isCurrent ? 1.0f : 0.25f);
	currentText->SetVisibility(isCurrent ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UUI_DifficultyWidget::SelectDifficulty()
{
	if (isCurrent) return;
	
	if (const AM_GameMode* _gm = GetWorld()->GetAuthGameMode<AM_GameMode>())
		if (const AM_GameLogic* _gl = _gm->GetGameLogic())
			if (const AIU_Player* _player = _gl->GetPlayer())
				if (_player->IsInArena())
					if (UUI_ConfirmWidget* _widget = hud->GetConfirmWidget())
					{
						_widget->InitializeConfirmBox(changeDifficultyConfirmTitle, changeDifficultyConfirmText);
						_widget->OnConfirm().AddUniqueDynamic(this, &UUI_DifficultyWidget::ConfirmSelectDifficulty);
						return;
					}
	//else 
	ConfirmSelectDifficulty();
}

void UUI_DifficultyWidget::ConfirmSelectDifficulty()
{
	onDifficultySelect.Broadcast(difficulty);
}

void UUI_DifficultyWidget::HoverDifficulty()
{
	if (isCurrent) return;
	difficultyImage->SetOpacity(1.0f);
}

void UUI_DifficultyWidget::UnhoverDifficulty()
{
	if (isCurrent) return;
	difficultyImage->SetOpacity(0.25f);
}
