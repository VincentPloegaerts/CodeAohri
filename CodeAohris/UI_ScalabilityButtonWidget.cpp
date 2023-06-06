#include "UI_ScalabilityButtonWidget.h"

void UUI_ScalabilityButtonWidget::InitializeScalabilityWidget(const EScalabilityType& _type, const int32 _defaultValue)
{
	Super::InitializeScalabilityWidget(_type, _defaultValue);

	if (_defaultValue == 0)
		UpdateButtons(lowButton);
	else if (_defaultValue == 1)
		UpdateButtons(medButton);
	else if (_defaultValue == 2)
		UpdateButtons(highButton);
	else if (_defaultValue == 3)
		UpdateButtons(epicButton);
	else if (_defaultValue == 4)
		UpdateButtons(cinButton);
	else 
		UpdateButtons(nullptr);
}

void UUI_ScalabilityButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	lowButton->OnClicked.AddUniqueDynamic(this, &UUI_ScalabilityButtonWidget::LowSelect);
	medButton->OnClicked.AddUniqueDynamic(this, &UUI_ScalabilityButtonWidget::MediumSelect);
	highButton->OnClicked.AddUniqueDynamic(this, &UUI_ScalabilityButtonWidget::HighSelect);
	epicButton->OnClicked.AddUniqueDynamic(this, &UUI_ScalabilityButtonWidget::EpicSelect);
	cinButton->OnClicked.AddUniqueDynamic(this, &UUI_ScalabilityButtonWidget::CinematicSelect);
}

void UUI_ScalabilityButtonWidget::LowSelect()
{
	ScalabilitySelect(0);
	UpdateButtons(lowButton);
}
void UUI_ScalabilityButtonWidget::MediumSelect()
{
	ScalabilitySelect(1);	
	UpdateButtons(medButton);
}
void UUI_ScalabilityButtonWidget::HighSelect()
{
	ScalabilitySelect(2);
	UpdateButtons(highButton);
}
void UUI_ScalabilityButtonWidget::EpicSelect()
{
	ScalabilitySelect(3);
	UpdateButtons(epicButton);
}
void UUI_ScalabilityButtonWidget::CinematicSelect()
{
	ScalabilitySelect(4);	
	UpdateButtons(cinButton);
}

void UUI_ScalabilityButtonWidget::UpdateButtons(UButton* _selectedButton)
{
	lowButton->SetColorAndOpacity(unselectedButtonColor);
	medButton->SetColorAndOpacity(unselectedButtonColor);
	highButton->SetColorAndOpacity(unselectedButtonColor);
	epicButton->SetColorAndOpacity(unselectedButtonColor);
	cinButton->SetColorAndOpacity(unselectedButtonColor);
	if (!_selectedButton) return;
	_selectedButton->SetColorAndOpacity(selectedButtonColor);
}
