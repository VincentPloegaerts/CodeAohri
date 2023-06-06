#pragma once

#include "CoreMinimal.h"
#include "UI_BackWidget.h"
#include "UI_PopUpWidgetWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_PopUpWidgetWidget : public UUI_BackWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* title = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* text = nullptr;

private:
	virtual void InitializeFocus() override;
	
public:
	void InitializePopUpWidget(const FString& _titleText, const FString& _text) const;
};
