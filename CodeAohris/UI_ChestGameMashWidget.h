#pragma once
#include "CoreMinimal.h"
#include "UI_Widget.h"
#include "Components/ProgressBar.h"
#include "UI_ChestGameMashWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_ChestGameMashWidget : public UUI_Widget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* progressMash = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* timePB = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isUsingKeyboard = true;
	UPROPERTY()
	float fTime = 0;
	UPROPERTY()
	float fMaxTime = 0;
	

public:
	FORCEINLINE void SetTime(const float _time) { fMaxTime = _time; fTime = fMaxTime; }
	FORCEINLINE void SetProgressMash(float _value) const {progressMash->SetPercent(_value);}
	FORCEINLINE void SetUsingKeyboard(bool _value) {isUsingKeyboard = _value;}

private:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
