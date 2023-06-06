#include "UI_CinematicWidget.h"

void UUI_CinematicWidget::ShowDisplayImage()
{
	if (timerManager)
	{
		timerManager->SetTimer(showTimer, this, &UUI_CinematicWidget::ShowDisplayImageDelayed, 0.1f);
	}
}

void UUI_CinematicWidget::HideDisplayImage()
{
	if (timerManager && timerManager->TimerExists(showTimer))
		timerManager->ClearTimer(showTimer);
	
	cinematicDisplayImage->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_CinematicWidget::UpdateProgressBar(const float& _current, const float& _max)
{
	holdingPB->SetPercent(_current / _max);
}
void UUI_CinematicWidget::ResetProgressbar()
{
	holdingPB->SetPercent(0);	
}

void UUI_CinematicWidget::InitializeWidget()
{
	Super::InitializeWidget();

	timerManager = &GetWorld()->GetTimerManager();
}

void UUI_CinematicWidget::ShowDisplayImageDelayed()
{
	cinematicDisplayImage->SetVisibility(ESlateVisibility::Visible);
}
