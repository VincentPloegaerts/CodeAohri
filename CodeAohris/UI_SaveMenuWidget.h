#pragma once

#include "CoreMinimal.h"
#include "UI_BackWidget.h"

#include "Components/ScrollBox.h"
#include "Components/UniformGridPanel.h"

#include "UI_SaveMenuWidget.generated.h"

class UUI_GameSaveWidget;

UCLASS(Abstract)
class INSIDEUS_API UUI_SaveMenuWidget : public UUI_BackWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bGameSave3D = true;
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition = "!bGameSave3D", EditConditionHides))
	TSubclassOf<UUI_GameSaveWidget> gameSaveWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (UIMin = "0", UIMax = "10", ClampMin = "0", ClampMax = "10", EditCondition = "!bGameSave3D", EditConditionHides))
	int iMaxGameSaveInUniformGrid = 5;
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* gameSaveSB = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUniformGridPanel* gameSaveUG = nullptr;
	
	UPROPERTY()
	TArray<UUI_GameSaveWidget*> gameSaveWidgets = { };
	UPROPERTY()
	class UM_GameInstance* gameInstance = nullptr;
	
private:
	void InitializeGameSaveUniformGrid(const int& _maxGameSave);
	void InitializeGameSaveScrollBox(const int& _maxGameSave);
	
	virtual void InitializeWidget() override;
	void InitializeWidgets();

	virtual void InitializeFocus() override;

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	
protected:
	virtual void InitializeGameSaveWidget(UUI_GameSaveWidget* _widget, const int& _index, const class US_GameSave* _gameSave);

	virtual void Back() override;
	
	UFUNCTION() virtual void RefreshGameSave();
	
public:
	UFUNCTION() virtual void SelectGame(const int& _gameIndex);
};
