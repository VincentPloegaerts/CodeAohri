#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/Spacer.h"

#include "UI_ScalabilityWidget.generated.h"

UENUM()
enum EScalabilityType
{
	Scalability_None,
	Global_Quality,
	View_Distance_Quality,
	Anti_Aliasing_Quality,
	Shadow_Quality,
	Illumination_Quality,
	Reflection_Quality,
	Post_Process_Quality,
	Texture_Quality,
	Effects_Quality,
	Foliage_Quality,
	Shading_Quality
};

UCLASS(Abstract)
class INSIDEUS_API UUI_ScalabilityWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScalabilitySelect, const EScalabilityType&, type, int32, _value);

protected:
	UPROPERTY()
	TEnumAsByte<EScalabilityType> scalabilityType;
	UPROPERTY()
	TMap<int32, FString> scalabilityName =
	{
		{-1 , "CUSTOM"},
		{0 , "LOW"},
		{1 , "MEDIUM"},
		{2 , "HIGH"},
		{3 , "EPIC"},
		{4 , "CINEMATIC"},
	};
    UPROPERTY()
    int32 scalabilityValue = -1;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* scalabilityText = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpacer* spacerTop = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpacer* spacerBottom = nullptr;

	UPROPERTY()
	FOnScalabilitySelect onScalabilitySelect;
	
public:
	FORCEINLINE FOnScalabilitySelect& OnScalabilitySelect() { return onScalabilitySelect; }
	
public:
	virtual void InitializeScalabilityWidget(const EScalabilityType& _type, const int32 _defaultValue);
	void SetSpacerSize(const FVector2D& _size) const;

protected:
	void ScalabilitySelect(const int32& _value) const;
};
