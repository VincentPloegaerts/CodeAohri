#pragma once
#include "Components/WidgetComponent.h"

#include "STRUCT_WidgetComponentSettings.generated.h"

USTRUCT()
struct FWidgetComponentSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Widget Component Settings")
		FVector location = FVector::Zero();
	
	UPROPERTY(EditAnywhere, Category = "Widget Component Settings")
		FRotator rotation = FRotator::ZeroRotator;
	
	UPROPERTY(EditAnywhere, Category = "Widget Component Settings")
		FVector2D size = FVector2D::Zero();
	
	UPROPERTY(EditAnywhere, Category = "Widget Component Settings")
		TSubclassOf<UUserWidget> widget = TSubclassOf<UUserWidget>();

	void ApplySettingsTo(UWidgetComponent* _widgetComponent)
	{
		_widgetComponent->SetRelativeLocation(location);
		_widgetComponent->SetRelativeRotation(rotation);
		_widgetComponent->SetDrawSize(size);
		_widgetComponent->SetWidgetClass(widget);
	}
};