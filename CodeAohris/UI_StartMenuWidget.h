#pragma once
#include "CoreMinimal.h"
#include "UI_Widget.h"

#include "Components/Button.h"

#include "UI_StartMenuWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_StartMenuWidget : public UUI_Widget
{
	GENERATED_BODY() 

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* continueButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* newGameButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* loadGameButton = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* optionButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* rushButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* extraButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* creditButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* quitButton = nullptr;

private:
	virtual void InitializeWidget() override;
	virtual void InitializeFocus() override;
	
	UFUNCTION(BlueprintCallable) void ContinueGame();
	UFUNCTION(BlueprintCallable) void OpenNewGameMenu();
	UFUNCTION(BlueprintCallable) void OpenLoadGameMenu();
	
	UFUNCTION(BlueprintCallable) void OpenOptionMenu();
	
	UFUNCTION(BlueprintCallable) void OpenRushModeMenu();
	UFUNCTION(BlueprintCallable) void OpenExtraMenu();
	UFUNCTION(BlueprintCallable) void OpenCredit();
	
	UFUNCTION(BlueprintCallable) void Quit();
};