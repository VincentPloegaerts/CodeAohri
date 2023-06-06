// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_TutoElementWidget.h"
#include "UI_TutoCountElementWidget.generated.h"

/**
 * 
 */
UCLASS()
class INSIDEUS_API UUI_TutoCountElementWidget : public UUI_TutoElementWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountUpdate);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnCountUpdate"))
	FOnCountUpdate onCountUpdate = FOnCountUpdate();
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Settings")
	int currentCount = 0;
	
protected:
	virtual void InitializeWidget() override;
	
public:
	virtual void InitializeKeyboardVisual() override;
	virtual void InitializeGamepadVisual() override;



private:
	void UpdateWidgetsVisual();

public :
	UFUNCTION(BlueprintCallable) void UpdateCurrentCount(float _newCount);
	UFUNCTION(BlueprintCallable) void ResetCurrentCount();
	
};
