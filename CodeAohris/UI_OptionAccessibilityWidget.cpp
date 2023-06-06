#include "UI_OptionAccessibilityWidget.h"

#include "M_GameInstance.h"
#include "S_PlayerSave.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "IU_Macros.h"

void UUI_OptionAccessibilityWidget::InitializeFocus()
{
	colorblindCB->SetKeyboardFocus();
}

void UUI_OptionAccessibilityWidget::SetVisibility(ESlateVisibility InVisibility)
{
	if (GetVisibility() != ESlateVisibility::Hidden)
		if (InVisibility == ESlateVisibility::Hidden)
			if (playerSave)
				playerSave->PlayerSaveEdited();
	
	Super::SetVisibility(InVisibility);
}

void UUI_OptionAccessibilityWidget::InitializeWidget()
{
	colorblindCB->OnSelectionChanged.AddUniqueDynamic(this, &UUI_OptionAccessibilityWidget::SelectColorBlind);
	colorblindSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionAccessibilityWidget::ChangeColorBlindSeverity);

	fpsCB->OnCheckStateChanged.AddUniqueDynamic(this, &UUI_OptionAccessibilityWidget::SetDisplayFPS);
	for (auto _type : colorblindTypes)
		colorblindCB->AddOption(_type.Value);
	
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		playerSave = _gi->GetPlayerSave();
		if (playerSave)
		{
			const FAccessibilityConfig& _config = playerSave->GetAccessibilityConfig();

			InitializeColorBlindAccessibility(_config);
			fpsCB->SetIsChecked(_config.GetDisplayFPS());
		}
	}
}

void UUI_OptionAccessibilityWidget::InitializeColorBlindAccessibility(const FAccessibilityConfig& _config)
{
	const EColorVisionDeficiency& _colorblindDeficiency = _config.GetColorVisionDeficiency();
	if (colorblindTypes.Contains(_colorblindDeficiency))
		colorblindCB->SetSelectedOption(colorblindTypes[_colorblindDeficiency]);
	colorblindSlider->SetValue(_config.GetColorVisionDeficiencySeverity());

	SelectColorBlind(colorblindCB->GetSelectedOption(), ESelectInfo::Direct);	//Call to set Color blind at game launch
}

void UUI_OptionAccessibilityWidget::SelectColorBlind(FString _selection, ESelectInfo::Type SelectionType)
{
	const EColorVisionDeficiency& _enum = static_cast<EColorVisionDeficiency>(colorblindCB->GetSelectedIndex());
	if (!colorblindTypes.Contains(_enum)) return;
	UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(_enum, colorblindSlider->GetValue(), false, false);

	playerSave->EditAccessibilityConfig().SetColorVisionDeficiency(_enum);
}

void UUI_OptionAccessibilityWidget::ChangeColorBlindSeverity(float _severity)
{
	SelectColorBlind(colorblindCB->GetSelectedOption(), ESelectInfo::Direct);
	
	playerSave->EditAccessibilityConfig().SetColorVisionDeficiencySeverity(_severity);
}

void UUI_OptionAccessibilityWidget::SetDisplayFPS(bool _enable)
{
	playerSave->EditAccessibilityConfig().SetDisplayFPS(_enable);
}
