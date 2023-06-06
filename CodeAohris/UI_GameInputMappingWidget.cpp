#include "UI_GameInputMappingWidget.h"

void UUI_GameInputMappingWidget::InitializeWidget()
{
	MappingIKS->SetAllowGamepadKeys(true);
	MappingIKS->OnKeySelected.AddUniqueDynamic(this, &UUI_GameInputMappingWidget::RebindKey);
	MappingIKS->OnIsSelectingKeyChanged.AddUniqueDynamic(this, &UUI_GameInputMappingWidget::UpdateSelection);

	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &UUI_GameInputMappingWidget::UpdateKeyVisual, 0.2f);
}

void UUI_GameInputMappingWidget::RebindKey(FInputChord _key)
{
	//Declaration in Child class
}

void UUI_GameInputMappingWidget::UpdateKeyVisual()
{
	InputInfoWidget->InitializeInputInfoWidget(MappingIKS->SelectedKey.Key);
}

void UUI_GameInputMappingWidget::UpdateSelection()
{
	if (MappingIKS->GetIsSelectingKey())
	{
		InputInfoWidget->ClearInputInfoWidget(SelectionText);
	}
}
