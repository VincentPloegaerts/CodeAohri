#pragma once

#include "CoreMinimal.h"
#include "UI_UserWidget.h"
#include "Components/TextBlock.h"
#include "UI_DescriptionElementWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_DescriptionElementWidget : public UUI_UserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* descriptionText = nullptr;

public:
	void InitializeDescriptionElementWidget(const FString& _description);
};
