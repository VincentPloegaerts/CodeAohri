#pragma once
#include "CoreMinimal.h"
#include "UI_UserWidget.h"
#include "UI_Widget.generated.h"

//Managed Widget class
UCLASS(Abstract)
class INSIDEUS_API UUI_Widget : public UUI_UserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Widget | Animation", Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* showAnimation = nullptr;
	UPROPERTY()
	class AHUD_Main* hud = nullptr;
		
protected:
	virtual void InitializeWidget() override;

public:
	UFUNCTION(BlueprintCallable) void PlayShowAnimation();
};