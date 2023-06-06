#pragma once

#include "CoreMinimal.h"
#include "UI_ScalabilityWidget.h"
#include "UI_ScalabilitySelectorWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_ScalabilitySelectorWidget : public UUI_ScalabilityWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* higherButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* lowerButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* selectorText = nullptr;

public:
	virtual void InitializeScalabilityWidget(const EScalabilityType& _type, const int32 _defaultValue) override;
	
private:
	virtual void NativeConstruct() override;
	
	void SelectScalability(const int32& _value);
	UFUNCTION() void IncreaseScalability();
	UFUNCTION() void DecreaseScalability();
};
