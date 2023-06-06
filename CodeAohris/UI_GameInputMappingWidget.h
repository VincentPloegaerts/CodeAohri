#pragma once

#include "CoreMinimal.h"
#include "M_InputManager.h"
#include "UI_GameInputInfoWidget.h"
#include "UI_UserWidget.h"

#include "Components/InputKeySelector.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"

#include "UI_GameInputMappingWidget.generated.h"

UCLASS(Abstract)
class INSIDEUS_API UUI_GameInputMappingWidget : public UUI_UserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRebindKeyFailed, FKey, _failedKey, FString, _reason);
	
protected:
	//Input Selector
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UInputKeySelector* MappingIKS = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUI_GameInputInfoWidget* InputInfoWidget = nullptr;
	
	UPROPERTY()
	bool AllowGamepadKey = false;
	UPROPERTY()
	FOnRebindKeyFailed onRebindKeyFailed;
	UPROPERTY()
	UM_InputManager* InputManager = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Mapping Widget | Settings")
	FString FailedKeyTypeReasonText = "Key is not allowed here !";
	UPROPERTY(EditAnywhere, Category = "Mapping Widget | Settings")
	FString FailedKeyUsedReasonText = "Key already used !";

private:
	UPROPERTY(EditAnywhere, Category = "Mapping Widget | Settings")
	FString SelectionText = "...";
	UPROPERTY(EditAnywhere, Category = "Mapping Widget | Settings")
	FString SelectionEmptyText = "NONE";
	
public:
	FORCEINLINE FOnRebindKeyFailed& OnRebindKeyFailed() { return onRebindKeyFailed; }
	
private:
	virtual void InitializeWidget() override;
	
protected:
	UFUNCTION() virtual void RebindKey(FInputChord _key);

	UFUNCTION() void UpdateKeyVisual();
	
private:
	UFUNCTION() void UpdateSelection();
};
