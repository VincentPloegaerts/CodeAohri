#pragma once

#include "CoreMinimal.h"
#include "STRUCT_AccessibilityConfig.h"
#include "UI_UserWidget.h"

#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/HorizontalBox.h"
#include "Components/Slider.h"

#include "UI_OptionAccessibilityWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_OptionAccessibilityWidget : public UUI_UserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* colorblindCB = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USlider* colorblindSlider = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCheckBox* fpsCB = nullptr;
	
private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TMap<EColorVisionDeficiency, FString> colorblindTypes =
	{
		{ EColorVisionDeficiency::NormalVision, "Normal"},
		{ EColorVisionDeficiency::Deuteranope, "Deuteranope"},
		{ EColorVisionDeficiency::Protanope, "Protanope"},
		{ EColorVisionDeficiency::Tritanope, "Tritanope"}
	};

	UPROPERTY()
	class US_PlayerSave* playerSave = nullptr;

public:
	virtual void InitializeFocus() override;
	
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	
private:
	virtual void InitializeWidget() override;

	void InitializeColorBlindAccessibility(const FAccessibilityConfig& _config);

	UFUNCTION() void SelectColorBlind(FString _selection, ESelectInfo::Type SelectionType);
	UFUNCTION() void ChangeColorBlindSeverity(float _severity);

	UFUNCTION() void SetDisplayFPS(bool _enable);
};
