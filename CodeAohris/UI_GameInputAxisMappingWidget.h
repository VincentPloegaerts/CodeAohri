#pragma once

#include "CoreMinimal.h"
#include "UI_GameInputMappingWidget.h"
#include "UI_GameInputAxisMappingWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_GameInputAxisMappingWidget : public UUI_GameInputMappingWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Mapping Widget | Settings")
	FString FailedKeyGamepadReasonText = "can't rebind Gamepad Axis !";
	
	UPROPERTY()
	FInputAxisKeyMapping InputAxisMapping = FInputAxisKeyMapping();

public:
	void InitializeAxisMappingWidget(UM_InputManager* _inputManager, const FInputAxisKeyMapping& _axisMapping);

private:
	virtual void RebindKey(FInputChord _key) override;
};
