#pragma once

#include "CoreMinimal.h"
#include "STRUCT_GameInput.h"
#include "STRUCT_InputData.h"
#include "UI_GameInputInfoWidget.h"
#include "UI_UserWidget.h"
#include "UI_TutoElementWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_TutoElementWidget : public UUI_UserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UUI_GameInputInfoWidget> GameInputInfoWidgetType;

	UPROPERTY()
	TArray<UUI_GameInputInfoWidget*> InputInfoWidgets = { };
	
protected:
	virtual void InitializeWidget() override;

public:
	virtual void InitializeKeyboardVisual() override;
	virtual void InitializeGamepadVisual() override;
	
	UFUNCTION(BlueprintImplementableEvent) void OnGameInput(FGameInput _gameInput);

protected:
	UFUNCTION(BlueprintCallable) TArray<UUI_GameInputInfoWidget*> CreateWidgetsFromAction(const FInputActionData& _inputAction);
	UFUNCTION(BlueprintCallable) TArray<UUI_GameInputInfoWidget*> CreateWidgetsFromAxis(const FInputAxisData& _inputAxis);
	UFUNCTION(BlueprintCallable) void ClearWidgets();
	
private:
	void UpdateWidgetsVisual();
};
