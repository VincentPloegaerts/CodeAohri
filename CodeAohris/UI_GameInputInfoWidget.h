#pragma once

#include "CoreMinimal.h"
#include "UI_UserWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "DT_InputInfoDataTable.h"

#include "UI_GameInputInfoWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_GameInputInfoWidget : public UUI_UserWidget
{
	GENERATED_BODY()

	//Input Key Selector Background
	UPROPERTY(meta = (BindWidget))
	UImage* InputBackgroundImage = nullptr;
	UPROPERTY(meta = (BindWidget))
	UImage* InputImage = nullptr;
	//Input Text
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InputText = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	UDT_InputInfoDataTable* InputInfoDataTable = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	UTexture2D* GamepadBackgroundTexture = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	UTexture2D* KeyboardBackgroundTexture = nullptr;

	UPROPERTY()
	FKey Key = FKey(); 

public:
	FORCEINLINE const FKey& GetKey() const { return Key; }
	
public:
	UFUNCTION(BlueprintCallable) void InitializeInputInfoWidget(const FKey& _key);

	void ClearInputInfoWidget(const FString& _text = "") const;
};
