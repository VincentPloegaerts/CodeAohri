#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "STRUCT_GameInput.h"

#include "DT_InputDataTable.generated.h"

USTRUCT()
struct FInputDataTableJSON
{
	GENERATED_BODY()
	
	UPROPERTY()
	FGameInput GameInputAZERTY;
	UPROPERTY()
	FGameInput GameInputQWERTY;
};

UCLASS()
class INSIDEUS_API UDT_InputDataTable : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Inputs AZERTY", meta = (ShowOnlyInnerProperties))
	FGameInput GameInputAZERTY = FGameInput(false);
	UPROPERTY(EditAnywhere, Category = "Inputs QWERTY", meta = (ShowOnlyInnerProperties))
	FGameInput GameInputQWERTY = FGameInput(true);
public:
	FORCEINLINE const FGameInput& GetGameInput(const bool _qwerty) { return _qwerty ? GameInputQWERTY : GameInputAZERTY; }

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Inputs") void ResetInput();
	UFUNCTION(CallInEditor, Category = "Save") void Save();
	UFUNCTION(CallInEditor, Category = "Save") void Load();
#endif
};
