#pragma once
#include "CoreMinimal.h"
#include "UI_GameInputInfoWidget.h"
#include "UI_ChestQTEKeyWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_ChestQTEKeyWidget : public UUI_GameInputInfoWidget
{
	GENERATED_BODY()
public:
	void ToInitKey();
	void ToClearedKey();
	void ToCurrentKey();

protected:
	UFUNCTION(BlueprintImplementableEvent) void OnInitKey();
	UFUNCTION(BlueprintImplementableEvent) void OnClearedKey();
	UFUNCTION(BlueprintImplementableEvent) void OnCurrentKey();
	
};
