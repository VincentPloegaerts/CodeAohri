#pragma once

#include "CoreMinimal.h"
#include "UI_GameInputMappingWidget.h"
#include "UI_GameInputActionMappingWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_GameInputActionMappingWidget : public UUI_GameInputMappingWidget
{
	GENERATED_BODY()

	UPROPERTY()
	FInputActionKeyMapping InputActionKeyMapping = FInputActionKeyMapping();
	
public:
	void InitializeActionMappingWidget(UM_InputManager* _inputManager, const FInputActionKeyMapping& _actionMapping);

private:
	virtual void RebindKey(FInputChord _key) override;
};
