#include "UI_TutoWidget.h"

#include "Components/CanvasPanelSlot.h"

#include "IU_Macros.h"

UUI_TutoElementWidget* UUI_TutoWidget::InitializeTutoWidget(TSubclassOf<UUI_TutoElementWidget> _widgetToCreate)
{
	if (!_widgetToCreate.Get()) return nullptr;

	ClearTutoWidget();
	
	UUI_TutoElementWidget* _widget = CreateWidget<UUI_TutoElementWidget>(this, _widgetToCreate);
	UCanvasPanelSlot* _slot = tutoPanel->AddChildToCanvas(_widget);
	_slot->SetOffsets(marginSettings);
	_slot->SetAnchors(anchorSettings);

	return currentTutoElementWidget = _widget;
}

void UUI_TutoWidget::ClearTutoWidget()
{
	tutoPanel->ClearChildren();
	currentTutoElementWidget = nullptr;
}

void UUI_TutoWidget::InitializeKeyboardVisual()
{
	if (!currentTutoElementWidget) return;
	currentTutoElementWidget->InitializeKeyboardVisual();
}

void UUI_TutoWidget::InitializeGamepadVisual()
{
	if (!currentTutoElementWidget) return;
	currentTutoElementWidget->InitializeGamepadVisual();	
}
