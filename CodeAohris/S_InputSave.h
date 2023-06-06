#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "STRUCT_GameInput.h"

#include "S_InputSave.generated.h"

UCLASS()
class INSIDEUS_API US_InputSave : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY()
	FGameInput SavedGameInput = FGameInput();

public:
	FORCEINLINE const FGameInput& GetSavedGameInput() const { return SavedGameInput; }
	FORCEINLINE void SetSavedGameInput(const FGameInput& _gameInput) { SavedGameInput = _gameInput; }
};
