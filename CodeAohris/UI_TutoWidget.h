#pragma once

#include "CoreMinimal.h"
#include "UI_Widget.h"

#include "UI_TutoElementWidget.h"

#include "Components/CanvasPanel.h"

#include "UI_TutoWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_TutoWidget : public UUI_Widget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* tutoPanel = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Tuto Widget | Settings")
	FAnchors anchorSettings = FAnchors(0, 0, 1, 1); 
	UPROPERTY(EditAnywhere, Category = "Tuto Widget | Settings")
	FMargin marginSettings = FMargin(0, 0, 0, 0); 

	UPROPERTY()
	UUI_TutoElementWidget* currentTutoElementWidget = nullptr;
	
public:
	UUI_TutoElementWidget* InitializeTutoWidget(TSubclassOf<UUI_TutoElementWidget> _widgetToCreate);
	void ClearTutoWidget();

private:
	virtual void InitializeKeyboardVisual() override;
	virtual void InitializeGamepadVisual() override;
};
