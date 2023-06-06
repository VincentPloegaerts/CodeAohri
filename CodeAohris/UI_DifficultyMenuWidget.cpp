#include "UI_DifficultyMenuWidget.h"

#include "HUD_Game.h"
#include "M_GameInstance.h"

#include "UI_DifficultyWidget.h"
#include "UI_DescriptionWidget.h"
#include "UI_ConfirmWidget.h"

#include "IU_Macros.h"
#include "Components/HorizontalBoxSlot.h"

void UUI_DifficultyMenuWidget::InitializeWidget()
{
	Super::InitializeWidget();

	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &UUI_DifficultyMenuWidget::InitializeWidgets, 0.10f);
}

void UUI_DifficultyMenuWidget::InitializeFocus()
{
	if (widgets.Num() == 0) return;
	widgets[0]->InitializeFocus();
}

void UUI_DifficultyMenuWidget::InitializeWidgets()
{
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
 		difficultyManager = _gi->GetDifficultyManager();
	
	if (difficultyManager)
	{
		if (difficultyWidgetType.Get())
		{
			const TMap<TEnumAsByte<EDifficultyType>, FDifficultyElement>& _difficulties = difficultyManager->GetDifficulties();
			difficultyHB->ClearChildren();
			TArray<TEnumAsByte<EDifficultyType>> _difficultyTypes = { };
			_difficulties.GenerateKeyArray(_difficultyTypes);
				
			const int _max = _difficultyTypes.Num();
			for (int i = 0; i < _max; ++i)
			{
				const EDifficultyType& _difficulty = _difficultyTypes[i];
				if (!_difficulties.Contains(_difficulty)) continue;
				const FDifficultyElement& _difficultyElement = _difficulties[_difficulty];
				const FString& _description = _difficultyElement.GetDifficultyDescription();
				const TArray<FString>& _extraDescription = _difficultyElement.GetDifficultyExtraDescription();
				UTexture2D* _texture = _difficultyElement.GetDifficultyTexture();

				CreateDifficultyWidget(_difficulty, _description, _extraDescription, _texture);

				if (i < _max - 1)
					CreateSeparator();
			}
		}
		else
			LOG("UUI_DifficultyMenuWidget::InitializeWidgets => Difficulty Widget Type is not valid.")
	}
}

void UUI_DifficultyMenuWidget::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	//Begin Thomas
	if (InVisibility == ESlateVisibility::Visible)
	{
		if (!difficultyManager) return;
		
		TArray<UWidget*> _children = difficultyHB->GetAllChildren();
		const int& _childrenCount = _children.Num();

		for (int _childIndex = 0; _childIndex < _childrenCount; ++_childIndex)
		{
			UUI_DifficultyWidget* _widget = Cast<UUI_DifficultyWidget>(_children[_childIndex]);
			if (!_widget) continue;
			_widget->Update(difficultyManager->GetCurrentDifficulty());
		}
	}
	//End Thomas
}


void UUI_DifficultyMenuWidget::CreateDifficultyWidget(const EDifficultyType& _difficulty, const FString& _description, const TArray<FString>& _extraDescription, UTexture2D* _texture)
{
	UUI_DifficultyWidget* _widget = CreateWidget<UUI_DifficultyWidget>(this, difficultyWidgetType);
	_widget->OnDifficultySelect().AddUniqueDynamic(this, &UUI_DifficultyMenuWidget::SelectDifficulty);

	_widget->InitializeDifficultyWidget(_difficulty, _description, _extraDescription, _texture);
					
	widgets.Add(_widget);
	if (UHorizontalBoxSlot* _slot = difficultyHB->AddChildToHorizontalBox(_widget))
	{
		FSlateChildSize _slate;
		_slate.SizeRule = ESlateSizeRule::Fill;
		_slot->SetSize(_slate);
	}
}

void UUI_DifficultyMenuWidget::CreateSeparator()
{
	if (difficultySeparatorType.Get())
	{
		UUserWidget* _seperator = CreateWidget(this, difficultySeparatorType);
		difficultyHB->AddChildToHorizontalBox(_seperator);
	}
}

void UUI_DifficultyMenuWidget::SelectDifficulty(const EDifficultyType& _difficulty)
{
	if (difficultyManager)
	{
		difficultyManager->SetDifficulty(_difficulty);
		onDifficultySelected.Broadcast(_difficulty);
	}

	if (AHUD_Game* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Game>())
	{
		_hud->GoBackAllMenuWidget();
	}
}