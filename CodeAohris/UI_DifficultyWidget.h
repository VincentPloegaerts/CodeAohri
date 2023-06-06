#pragma once

#include "CoreMinimal.h"
#include "UI_Widget.h"

#include "ENUM_DifficultyType.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Delegates/DelegateCombinations.h"

#include "UI_DifficultyWidget.generated.h"

class UUI_DescriptionWidget;
 
UCLASS()
class INSIDEUS_API UUI_DifficultyWidget : public UUI_UserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultySelect, const EDifficultyType&, _difficulty);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* difficultyButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* difficultyText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UImage* difficultyImage = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* currentText = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UUI_DescriptionWidget* descriptionWidget = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings | Confirm Box")
	FString changeDifficultyConfirmTitle = "Change Difficulty ?";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings | Confirm Box")
	FString changeDifficultyConfirmText = "This action will restart the Boss fight";

	UPROPERTY()
	bool isCurrent = false;
	UPROPERTY()
	TEnumAsByte<EDifficultyType> difficulty = DIFFICULTY_NONE;
	UPROPERTY()
	bool bFocused = false;
	UPROPERTY()
	class AHUD_Main* hud = nullptr;

	UPROPERTY()
	FOnDifficultySelect onDifficultySelect;
	
public:
	FORCEINLINE FOnDifficultySelect& OnDifficultySelect() { return onDifficultySelect; }
	
public:
	virtual void InitializeFocus() override;
	virtual void InitializeDifficultyWidget(const EDifficultyType& _difficulty, const FString& _description, const TArray<FString>& _extraDescription, UTexture2D* _image = nullptr);
	void Update(const EDifficultyType& _difficulty);
	
protected:
	virtual void InitializeWidget() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void CheckDifficultyDescription();
	
	UFUNCTION() void SelectDifficulty();
	UFUNCTION() void ConfirmSelectDifficulty();
	
	UFUNCTION() void HoverDifficulty();
	UFUNCTION() void UnhoverDifficulty();
};
