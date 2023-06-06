#pragma once

#include "CoreMinimal.h"
#include "UI_UserWidget.h"
#include "Components/TextBlock.h"
#include "UI_RebindFailInfoWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_RebindFailInfoWidget : public UUI_UserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FailInfoText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ReasonText = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	class UDT_InputInfoDataTable* InputInfoDataTable = nullptr;

public:
	void InitializeText(const FName& _inputName, const FKey& _key, const FString& _reason) const;
};
