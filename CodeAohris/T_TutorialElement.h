#pragma once
#include "CoreMinimal.h"

#include "ENUM_BindType.h"
#include "IU_State.h"
#include "T_AdditionalCondition.h"

#include "Engine/DataAsset.h"
#include "T_TutorialElement.generated.h"

UCLASS()
class INSIDEUS_API UT_TutorialElement : public UDataAsset
{
	GENERATED_BODY()
	
	// Name of this tutorial
	UPROPERTY(EditAnywhere, Category = "Tutorial element values")
		FName tutorialName = FName();
	
	// Keys to execute
	UPROPERTY(EditAnywhere, Category = "Tutorial element values")
		TArray<FKey> keys = TArray<FKey>();

	// Current state to the player will be
	UPROPERTY(EditAnywhere, Category = "Tutorial element values")
		TEnumAsByte<EStateType> stateType = TEnumAsByte<EStateType>();
	
	// Widget to spawn
	UPROPERTY(EditAnywhere, Category = "Tutorial element values")
		TSubclassOf<UUserWidget> widgetType = TSubclassOf<UUserWidget>();

	// Widget
	UPROPERTY()
		UUserWidget* widget = nullptr;
	
	// Additional condition
	UPROPERTY(EditAnywhere, Category = "Tutorial element values")
		UT_AdditionalCondition* additionalCondition = nullptr;

public:
	FORCEINLINE FName GetTutorialName() const { return FName(tutorialName.ToString() + "Tutorial"); }
	FORCEINLINE TArray<FInputActionKeyMapping> GetBindsAction() const
	{
		TArray<FInputActionKeyMapping> _bindsAction;

		const int _keysCount = keys.Num();
		for (int _keyIndex = 0; _keyIndex < _keysCount; _keyIndex++)
		{
			_bindsAction.Add(FInputActionKeyMapping(GetTutorialName(), keys[_keyIndex]));
		}
		
		return _bindsAction;
	}
	FORCEINLINE const TEnumAsByte<EStateType>& GetStateType() const { return stateType; } 
	FORCEINLINE UT_AdditionalCondition* GetAdditionalCondition() const { return additionalCondition; }

public:
	UUserWidget* GetWidget(UWorld* _world) const;
};