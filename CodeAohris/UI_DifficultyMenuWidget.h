#pragma once
#include "CoreMinimal.h"
#include "UI_BackWidget.h"

#include "ENUM_DifficultyType.h"
#include "M_DifficultyManager.h"

#include "Components/GridPanel.h"
#include "Components/HorizontalBox.h"

#include "UI_DifficultyMenuWidget.generated.h"

class UUI_DifficultyWidget;
class UUI_DescriptionWidget;

UCLASS()
class INSIDEUS_API UUI_DifficultyMenuWidget : public UUI_BackWidget
{
	GENERATED_BODY() 

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultySelected, const EDifficultyType&, _difficulty);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* difficultyHB = nullptr;
	
private:
	UPROPERTY(EditAnywhere, Category ="Settings")
	TSubclassOf<UUI_DifficultyWidget> difficultyWidgetType;
	UPROPERTY(EditAnywhere, Category ="Settings")
	TSubclassOf<UUserWidget> difficultySeparatorType;

	UPROPERTY()
		UM_DifficultyManager* difficultyManager = nullptr;
	
	UPROPERTY()
	TArray<UUI_DifficultyWidget*> widgets = { };
	
	UPROPERTY()
	FOnDifficultySelected onDifficultySelected;

public:
	FORCEINLINE FOnDifficultySelected& OnDifficultySelected() { return onDifficultySelected; }
	
private:
	virtual void InitializeWidget() override;
	virtual void InitializeFocus() override;
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	UFUNCTION() void InitializeWidgets();

	void CreateDifficultyWidget(const EDifficultyType& _difficulty, const FString& _description, const TArray<FString>& _extraDescription, UTexture2D* _texture);
	void CreateSeparator();
	
	UFUNCTION(BlueprintCallable) void SelectDifficulty(const EDifficultyType& _difficulty);
};