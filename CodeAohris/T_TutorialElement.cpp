#include "T_TutorialElement.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UUserWidget* UT_TutorialElement::GetWidget(UWorld* _world) const
{
	return IsValid(widget) ? widget : CreateWidget(_world, widgetType);
}