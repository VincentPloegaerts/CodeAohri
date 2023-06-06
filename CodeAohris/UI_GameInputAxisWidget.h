#pragma once

#include "CoreMinimal.h"
#include "UI_GameInputWidget.h"

#include "UI_GameInputAxisMappingWidget.h"

#include "UI_GameInputAxisWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_GameInputAxisWidget : public UUI_GameInputWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input Widget | Settings")
	TSubclassOf<UUI_GameInputAxisMappingWidget> AxisMappingWidgetType;
	
	UPROPERTY()
	FInputAxisData InputAxisData = FInputAxisData();

public:
	void InitializeGameInputAxisWidget(UM_InputManager* _inputManager, const FInputAxisData& _axisInput);

private:
	void InitializeAxisMappingWidgets();

	UFUNCTION() void OnResetInputAxis(const TArray<FInputAxisData>& _axisInputs);
	
	UFUNCTION() void RebindAxisFailed(FKey _failedKey, FString _reason);
};
