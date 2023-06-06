#pragma once

#include "CoreMinimal.h"
#include "UI_UserWidget.h"

#include "M_InputManager.h"

#include "Components/TextBlock.h"
#include "Components/GridPanel.h"
#include "Components/HorizontalBox.h"

#include "UI_GameInputWidget.generated.h"

UCLASS(Abstract)
class INSIDEUS_API UUI_GameInputWidget : public UUI_UserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRebindInputFailed, FName, _inputName, FKey, failedKey, FString, _reason);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UGridPanel* GameInputGP = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InputNameText = nullptr;
	
	UPROPERTY()
	FOnRebindInputFailed onRebindInputFailed;
	UPROPERTY()
	UM_InputManager* InputManager = nullptr;

public:
	FORCEINLINE FOnRebindInputFailed& OnRebindInputFailed() { return onRebindInputFailed; }

protected:
	void RebindInputFailed(const FName& _inputName, const FKey& _failedKey, const FString& _reason);
};
