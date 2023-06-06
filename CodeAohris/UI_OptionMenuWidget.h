#pragma once
#include "CoreMinimal.h"
#include "UI_BackWidget.h"
#include "UI_GameInputInfoWidget.h"

#include "Components/CanvasPanel.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"

#include "UI_OptionMenuWidget.generated.h"

class UUI_UserWidget;
class UUI_OptionSoundWidget;
class UUI_OptionGraphicWidget;
class UUI_OptionControlWidget;
class UUI_OptionAccessibilityWidget;

UCLASS()
class INSIDEUS_API UUI_OptionMenuWidget : public UUI_BackWidget
{
	GENERATED_BODY() 

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* soundButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* graphicButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* controlButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* accessibilityButton = nullptr;

	UPROPERTY(meta  = (BindWidget))
	UCanvasPanel* widgetPanel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* rightOverlay = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* leftOverlay = nullptr;

	
private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	FColor currentColor = FColor::White;
	UPROPERTY(EditAnywhere, Category = "Settings")
	FColor notCurrentColor = FColor::Silver;
	
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UUI_OptionSoundWidget> optionSoundWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UUI_OptionGraphicWidget> optionGraphicWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UUI_OptionControlWidget> optionControlWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UUI_OptionAccessibilityWidget> optionAccessibilityWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UUI_GameInputInfoWidget> gameInputInfoWidgetType;	
	
	UPROPERTY()
	UUI_OptionSoundWidget* optionSoundWidget = nullptr;
	UPROPERTY()
	UUI_OptionGraphicWidget* optionGraphicWidget = nullptr;
	UPROPERTY()
	UUI_OptionControlWidget* optionControlWidget = nullptr;
	UPROPERTY()
	UUI_OptionAccessibilityWidget* optionAccessibilityWidget = nullptr;

	UPROPERTY()
	TArray<UUI_GameInputInfoWidget*> gameInputInfoWidgets = { };
	UPROPERTY()
	TMap<int32, UUI_UserWidget*> optionWidgets = { };
	
	UPROPERTY()
	int32 iWidgetIndex = 0;

private:
	template<class ClassType>
	ClassType* CreateOptionWidget(TSubclassOf<ClassType> _widgetType)
	{
		if (_widgetType.Get())
		{
			ClassType* _widget = CreateWidget<ClassType>(this, _widgetType);
			_widget->SetVisibility(ESlateVisibility::Hidden);
			UCanvasPanelSlot* _slot = widgetPanel->AddChildToCanvas(_widget);
			_slot->SetOffsets(FMargin(0, 0, 0, 0));
			_slot->SetAnchors(FAnchors(0, 0, 1, 1));
			optionWidgets.Add(optionWidgets.Num(), _widget);
			return _widget;
		}

		UE_LOG(LogTemp, Warning, TEXT("UUI_OptionMenuWidget::CreateOptionWidget => Widget Type is not valid."));
		return nullptr;			
	}
	
private:
	virtual void InitializeWidget() override;
	void InitializeInput(class UM_InputManager* _inputManager);
	void InitializeWidgets();
	
	virtual void InitializeFocus() override;

	virtual void InitializeKeyboardVisual() override;
	virtual void InitializeGamepadVisual() override;

	void UpdateOptionMenuBar(int _index);
	void UpdateWidgetVisibility();

	UFUNCTION() void UpdateOnInputReset(UM_InputManager* _inputManager);
	
	void HideWidgets();
	
	UFUNCTION() void ShowSoundWidget();
	UFUNCTION() void ShowGraphicWidget();
	UFUNCTION() void ShowControlWidget();
	UFUNCTION() void ShowAccessibilityWidget();

	int32 GetWidgetIndex(const UUI_UserWidget* _widget) const;
	
public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	
	UFUNCTION(BlueprintCallable) void GoToPreviousPanel();
	UFUNCTION(BlueprintCallable) void GoToNextPanel();
};