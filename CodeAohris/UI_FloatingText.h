#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UI_FloatingText.generated.h"

UCLASS()
class INSIDEUS_API UUI_FloatingText : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Interface", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UTextBlock* floatingText = nullptr;

public:
	UFUNCTION(BlueprintImplementableEvent) void SetTime(const float _time);
	UFUNCTION(BlueprintCallable) void SetText(const FString& _text) const;
	UFUNCTION(BlueprintCallable) void SetColor(const FColor& _color) const;
};