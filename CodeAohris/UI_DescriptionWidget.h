#pragma once

#include "CoreMinimal.h"
#include "UI_UserWidget.h"

#include "Components/VerticalBox.h"

#include "UI_DescriptionWidget.generated.h"

class UUI_DescriptionElementWidget;

UCLASS()
class INSIDEUS_API UUI_DescriptionWidget : public UUI_UserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UUI_DescriptionElementWidget> descriptionElementWidgetType;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* descriptionsVB = nullptr;

public:
	void InitializeDescriptionWidget(const FString& _principalDescription, const TArray<FString>& _extraDescription);
};
