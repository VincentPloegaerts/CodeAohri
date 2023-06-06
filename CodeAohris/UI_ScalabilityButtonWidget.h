#pragma once

#include "CoreMinimal.h"
#include "UI_ScalabilityWidget.h"
#include "UI_ScalabilityButtonWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_ScalabilityButtonWidget : public UUI_ScalabilityWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FLinearColor unselectedButtonColor = FLinearColor::Black;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FLinearColor selectedButtonColor = FLinearColor(1.0f, 0.0f, 1.0f, 1.0f);
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* lowButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* medButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* highButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* epicButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* cinButton = nullptr;

public:
	virtual void InitializeScalabilityWidget(const EScalabilityType& _type, const int32 _defaultValue) override;

private:
	virtual void NativeConstruct() override;
	
	UFUNCTION() void LowSelect();
	UFUNCTION() void MediumSelect();
	UFUNCTION() void HighSelect();
	UFUNCTION() void EpicSelect();
	UFUNCTION() void CinematicSelect();

	void UpdateButtons(UButton* _selectedButton);
};
