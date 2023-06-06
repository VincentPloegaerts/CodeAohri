#pragma once
#include "CoreMinimal.h"
#include "UI_SaveMenuWidget.h"
#include "UI_LoadSaveMenuWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_LoadSaveMenuWidget : public UUI_SaveMenuWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings | Confirm Widget")
	FString confirmTitle = "Load Save";
	UPROPERTY(EditAnywhere, Category = "Settings | Confirm Widget")
	FString confirmText = "";
	UPROPERTY(EditAnywhere, Category = "Settings | Popup Widget")
	FString popupTitle = "No Existing Save";
	UPROPERTY(EditAnywhere, Category = "Settings | Popup Widget")
	FString popupText = "";

	UPROPERTY()
	int iGameSave = -1;
	
private:
	virtual void SelectGame(const int& _gameIndex) override;

	UFUNCTION() void ConfirmSelectGame();
	
	virtual void InitializeGameSaveWidget(UUI_GameSaveWidget* _widget, const int& _index, const US_GameSave* _gameSave) override;
};