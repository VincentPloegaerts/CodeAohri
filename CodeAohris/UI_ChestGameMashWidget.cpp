#include "UI_ChestGameMashWidget.h"

void UUI_ChestGameMashWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetVisibility() == ESlateVisibility::Visible)
	{
		fTime -= InDeltaTime;
		timePB->SetPercent(fTime / fMaxTime);
	}
}
