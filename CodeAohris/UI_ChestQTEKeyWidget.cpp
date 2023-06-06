#include "UI_ChestQTEKeyWidget.h"

#if WITH_EDITOR
#include "IU_Macros.h"
#endif

void UUI_ChestQTEKeyWidget::ToInitKey()
{
	
	OnInitKey();
}

void UUI_ChestQTEKeyWidget::ToClearedKey()
{
	
	OnClearedKey();
}
void UUI_ChestQTEKeyWidget::ToCurrentKey()
{
	
	OnCurrentKey();
}
