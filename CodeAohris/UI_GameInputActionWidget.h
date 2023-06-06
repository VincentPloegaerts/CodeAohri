#pragma once

#include "CoreMinimal.h"
#include "UI_GameInputWidget.h"

#include "STRUCT_InputData.h"

#include "UI_GameInputActionMappingWidget.h"

#include "UI_GameInputActionWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_GameInputActionWidget : public UUI_GameInputWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input Widget | Settings")
	TSubclassOf<UUI_GameInputActionMappingWidget> ActionMappingWidgetType; 
	
	UPROPERTY()
	FInputActionData InputActionData = FInputActionData();

public:
	void InitializeGameInputActionWidget(UM_InputManager* _inputManager, const FInputActionData& _actionInput);

private:
	void InitializeActionMappingWidgets();

	UFUNCTION() void OnResetInputAction(const TArray<FInputActionData>& _actionInputs);

	UFUNCTION() void RebindActionFailed(FKey _failedKey, FString _reason);
};
