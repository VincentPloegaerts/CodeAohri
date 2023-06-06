#pragma once

#include "CoreMinimal.h"
#include "UI_Widget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "UI_CinematicWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_CinematicWidget : public UUI_Widget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* holdingPB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UImage* cinematicDisplayImage = nullptr;

	FTimerManager* timerManager = nullptr;
	FTimerHandle showTimer;
	
public:
	UFUNCTION(BlueprintImplementableEvent) void SetPassContentVisibility(ESlateVisibility _visibility);
	
	void ShowDisplayImage();
	void HideDisplayImage();
	
	FORCEINLINE void SetDisplayImage(UMaterialInstance* _material) const { cinematicDisplayImage->SetBrushFromMaterial(_material); }
	
	void UpdateProgressBar(const float& _current, const float& _max);
	void ResetProgressbar();

private:
	virtual void InitializeWidget() override;

	UFUNCTION() void ShowDisplayImageDelayed();
};
