#pragma once
#include "CoreMinimal.h"
#include "UI_SaveMenuWidget.h"

#include "ENUM_DifficultyType.h"

#include "UI_NewSaveMenuWidget.generated.h"

class UUI_GameSaveWidget;

UCLASS()
class INSIDEUS_API UUI_NewSaveMenuWidget : public UUI_SaveMenuWidget
{
	GENERATED_BODY() 

private:
	UPROPERTY(EditAnywhere, Category = "Settings | Confirm Box")
	FString createNewGameTitle = "Create New Game";
	UPROPERTY(EditAnywhere, Category = "Settings | Confirm Box")
	FString createNewGameText = "";

	UPROPERTY(EditAnywhere, Category = "Settings | Confirm Box")
	FString crushGameSaveTitle = "Crush Game Save";
	UPROPERTY(EditAnywhere, Category = "Settings | Confirm Box")
	FString crushGameSaveText = "This action will delete the existing save, are you sure ? (Can NOT be undone)";
	
	bool bSaveExist = false;
	int iGameIndex = -1;
	
private:
	virtual void InitializeGameSaveWidget(UUI_GameSaveWidget* _widget, const int& _index, const US_GameSave* _gameSave) override;
	virtual void SelectGame(const int& _gameIndex) override;

	void AskSelectGame(const FString& _title, const FString& _text);
	
	UFUNCTION() void ConfirmSelectGame();
	UFUNCTION() void InitializeNewSave(const EDifficultyType& _difficulty);

	virtual void Back() override;
};
