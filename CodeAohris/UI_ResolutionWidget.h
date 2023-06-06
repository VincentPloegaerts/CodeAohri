#pragma once
#include "CoreMinimal.h"

#include "Components/Button.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UI_ResolutionWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_ResolutionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Resolution values")
		UButton* resolutionButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Resolution values")
		UTextBlock* resolutionText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Resolution values")
		UTextBlock* resolutionRatioText = nullptr;
	
private:
	UPROPERTY(EditAnywhere, Category = "Resolution values")
		FKey confirmKey = FKey();

	UPROPERTY(VisibleAnywhere, Category = "Resolution values")
		FString resolution = FString();

	UPROPERTY(EditAnywhere, Category = "Resolution values | Style")
		FButtonStyle normalButtonStyle = FButtonStyle();

	UPROPERTY(EditAnywhere, Category = "Resolution values | Style")
		FButtonStyle hoverButtonStyle = FButtonStyle();
	
private:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
	UFUNCTION(BlueprintCallable) void ExecuteResolutionCallback();

public:
	void Init(const FString& _resolution, const FString& _ratio);
	void SetActiveStatus(const bool _status) const;
};