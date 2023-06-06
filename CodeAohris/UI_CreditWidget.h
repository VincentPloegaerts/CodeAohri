#pragma once

#include "CoreMinimal.h"
#include "UI_BackWidget.h"
#include "Components/ScrollBox.h"
#include "UI_CreditWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_CreditWidget : public UUI_BackWidget
{
	GENERATED_BODY()
   
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreditFinished);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* CreditSB = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ClampMin = 1))
	float fNormalScrollSpeed = 100;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ClampMin = 1))
	float fGamepadScrollSpeed = 150;

	float fScrollSpeed = 0;
	
	UPROPERTY()
	FOnCreditFinished onCreditFinished;
	
public:
	FORCEINLINE FOnCreditFinished& OnCreditFinished() { return onCreditFinished; }

	FORCEINLINE bool CanScroll() const { return IsVisible(); }
	FORCEINLINE bool IsScrollComplete() const { return CreditSB->GetScrollOffset() >= CreditSB->GetScrollOffsetOfEnd(); }

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	
private:
	virtual void InitializeWidget() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateScrolling(const float& _deltaTime);

	void ResetScroll();

	UFUNCTION() void GamepadScroll(float _axis);
};
