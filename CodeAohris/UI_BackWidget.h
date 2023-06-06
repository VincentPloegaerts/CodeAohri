#pragma once

#include "CoreMinimal.h"
#include "UI_Widget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "UI_BackWidget.generated.h"

UCLASS(Abstract)
class INSIDEUS_API UUI_BackWidget : public UUI_Widget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBack);
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* backButton = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* keyboardBackText = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* gamepadBackText = nullptr;

public:
	FOnBack OnBack;
	
protected:
	virtual void InitializeWidget() override;

	virtual void InitializeKeyboardVisual() override;
	virtual void InitializeGamepadVisual() override;
	
protected:
	UFUNCTION() virtual void Back();
	UFUNCTION() virtual void BackInput();
};
