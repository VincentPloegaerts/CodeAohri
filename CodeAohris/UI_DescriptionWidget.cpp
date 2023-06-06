#include "UI_DescriptionWidget.h"

#include "UI_DescriptionElementWidget.h"

#include "IU_Macros.h"

void UUI_DescriptionWidget::InitializeDescriptionWidget(const FString& _principalDescription, const TArray<FString>& _extraDescription)
{
	if (descriptionElementWidgetType.Get())
	{
		UUI_DescriptionElementWidget* _principal = CreateWidget<UUI_DescriptionElementWidget>(this, descriptionElementWidgetType);
		_principal->InitializeDescriptionElementWidget(_principalDescription);
		descriptionsVB->AddChildToVerticalBox(_principal);

		for (const auto& _current : _extraDescription)
		{
			UUI_DescriptionElementWidget* _widget = CreateWidget<UUI_DescriptionElementWidget>(this, descriptionElementWidgetType);
			_widget->InitializeDescriptionElementWidget(_current);
			descriptionsVB->AddChildToVerticalBox(_widget);
		}
	}
	else
		LOG("UUI_DescriptionWidget::InitializeDescriptionWidget => Description Widget Element is not valid.")
}
