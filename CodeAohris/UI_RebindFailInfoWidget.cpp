#include "UI_RebindFailInfoWidget.h"

#include "DT_InputInfoDataTable.h"

void UUI_RebindFailInfoWidget::InitializeText(const FName& _inputName, const FKey& _key, const FString& _reason) const
{
	const FInputInfo& _inputInfo = InputInfoDataTable ? InputInfoDataTable->GetInputInfoForKey(_key) : FInputInfo(_key.GetDisplayName());
		
	const FString& _infoStr = "Failed to rebind " + _inputName.ToString() + " with " + _inputInfo.InputDisplayName.ToString();

	FailInfoText->SetText(FText::FromString(_infoStr));
	ReasonText->SetText(FText::FromString(_reason));
}
