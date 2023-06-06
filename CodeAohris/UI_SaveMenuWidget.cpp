#include "UI_SaveMenuWidget.h"

#include "HUD_Main.h"
#include "UI_GameSaveWidget.h"
#include "M_GameInstance.h"
#include "S_GameSave.h"
#include "S_SaveActor.h"

#include "Components/UniformGridSlot.h"
#include "Components/ScrollBoxSlot.h"

#include "IU_Macros.h"

void UUI_SaveMenuWidget::InitializeWidget()
{
	Super::InitializeWidget();
	gameInstance = GetWorld()->GetGameInstance<UM_GameInstance>();

	const ESlateVisibility& _visibility = bGameSave3D ? ESlateVisibility::Hidden : ESlateVisibility::Visible;  
	gameSaveSB->SetVisibility(_visibility);
	gameSaveUG->SetVisibility(_visibility);
}
void UUI_SaveMenuWidget::InitializeWidgets()
{
	if (bGameSave3D) return;
	
	if (gameSaveWidgetType.Get())
	{
		if (!gameInstance) return;
		
		const int& _maxGameSlot = gameInstance->GetMaxSaveSlot();
		if (_maxGameSlot <= iMaxGameSaveInUniformGrid)
			InitializeGameSaveUniformGrid(_maxGameSlot);
		else
			InitializeGameSaveScrollBox(_maxGameSlot);
	}
	else
		LOG("UUI_NewSaveMenuWidget::InitializeWidgets => Game Save Widget Type is not valid.")
}

void UUI_SaveMenuWidget::InitializeFocus()
{
	if (bGameSave3D || gameSaveWidgets.Num() == 0) return;
	gameSaveWidgets[0]->InitializeFocus();
}

void UUI_SaveMenuWidget::Back()
{
	if (hud)
		hud->GoBackMenu(true);
}

void UUI_SaveMenuWidget::SetVisibility(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
		InitializeWidgets();

	Super::SetVisibility(InVisibility);
}

void UUI_SaveMenuWidget::InitializeGameSaveUniformGrid(const int& _maxGameSave)
{
	gameSaveUG->ClearChildren();
	gameSaveWidgets.Empty();
	for (int i = 0; i < _maxGameSave; i++)
	{
		UUI_GameSaveWidget* _widget = CreateWidget<UUI_GameSaveWidget>(this, gameSaveWidgetType);
		if (!_widget) break;
		gameSaveWidgets.Add(_widget);
				
		const US_GameSave* _gameSave = nullptr;
		if (gameInstance->DoesGameSaveExist(i))
			_gameSave = gameInstance->GetGameSaveByIndex(i);

		_widget->OnGameSelected().AddUniqueDynamic(this, &UUI_SaveMenuWidget::SelectGame);
		_widget->OnGameDeleted().AddUniqueDynamic(this, &UUI_SaveMenuWidget::RefreshGameSave);
		InitializeGameSaveWidget(_widget, i, _gameSave);
		
		if (UUniformGridSlot* _slot = gameSaveUG->AddChildToUniformGrid(_widget, i, 0))
		{
			_slot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			_slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		}
	}
}
void UUI_SaveMenuWidget::InitializeGameSaveScrollBox(const int& _maxGameSave)
{
	if (!gameInstance) return;
	gameSaveSB->ClearChildren();
	gameSaveWidgets.Empty();
	for (int i = 0; i < _maxGameSave; i++)
	{
		UUI_GameSaveWidget* _widget = CreateWidget<UUI_GameSaveWidget>(this, gameSaveWidgetType);
		if (!_widget) break;
		gameSaveWidgets.Add(_widget);
		
		const US_GameSave* _gameSave = gameInstance->GetGameSaveByIndex(i);

		_widget->OnGameSelected().AddUniqueDynamic(this, &UUI_SaveMenuWidget::SelectGame);
		_widget->OnGameDeleted().AddUniqueDynamic(this, &UUI_SaveMenuWidget::RefreshGameSave);
		InitializeGameSaveWidget(_widget, i, _gameSave);
		
		if (UScrollBoxSlot* _slot = Cast<UScrollBoxSlot>(gameSaveSB->AddChild(_widget)))
		{
			_slot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			_slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		}
	}
}

void UUI_SaveMenuWidget::InitializeGameSaveWidget(UUI_GameSaveWidget* _widget, const int& _index, const US_GameSave* _gameSave)
{
	//Default action, re declaration in child class..
	_widget->InitializeGameSaveWidget(_index, _gameSave);
}

void UUI_SaveMenuWidget::RefreshGameSave()
{
	InitializeWidgets();
	if (hud && hud->IsGamepadVisual())
		InitializeFocus();
}

void UUI_SaveMenuWidget::SelectGame(const int& _gameIndex)
{
	//Declaration in Child class
}