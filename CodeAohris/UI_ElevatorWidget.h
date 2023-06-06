#pragma once
#include "CoreMinimal.h"
#include "UI_GameInputInfoWidget.h"
#include "UI_Widget.h"
#include "Components/Image.h"
#include "UI_ElevatorWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_ElevatorWidget : public UUI_Widget
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,meta = (BindWidget))
		UUI_GameInputInfoWidget* leftKeyWidget;
	UPROPERTY(EditAnywhere,meta = (BindWidget))
		UUI_GameInputInfoWidget* rightKeyWidget;
	UPROPERTY(EditAnywhere,meta = (BindWidget))
		UUI_GameInputInfoWidget* gamepadWidget;
		

public:
	UFUNCTION(BlueprintCallable) void SetLeftKeyImage(const FKey& _key);
	UFUNCTION(BlueprintCallable) void SetRightKeyImage(const FKey& _key);
	UFUNCTION(BlueprintCallable) void SetGamepadKeyImage(const FKey& _key);
	UFUNCTION(BlueprintCallable) void HideLeftKeyImage();
	UFUNCTION(BlueprintCallable) void HideRightKeyImage();
	UFUNCTION(BlueprintCallable) void HideGamepadKeyImage();
};
