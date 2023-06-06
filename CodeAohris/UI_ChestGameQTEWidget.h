#pragma once
#include "CoreMinimal.h"
#include "UI_ChestQTEKeyWidget.h"
#include "UI_Widget.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "UI_ChestGameQTEWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_ChestGameQTEWidget : public UUI_Widget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* timePB = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UHorizontalBox* horizontalBox = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUI_ChestQTEKeyWidget> keyWidgetType;

	UPROPERTY()
	TArray<UUI_ChestQTEKeyWidget*> keyWidgets = { };
	float fMaxTime = 0;
	UPROPERTY()
	float fTime = 0;
	
public:
	UFUNCTION(BlueprintCallable) void SetTime(const float _time) { fMaxTime = _time; fTime = fMaxTime; } 
	UFUNCTION(BlueprintCallable) void FillHorizontalBox(const TArray<FKey>& _keys);
	UFUNCTION(BlueprintCallable) void EmptyBox();

	UFUNCTION(BlueprintCallable) void UpdateCurrentQTEKey(const int& _index);
	
private:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
