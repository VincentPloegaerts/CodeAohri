#pragma once

#include "STRUCT_UI_AttackWidgetData.generated.h"

USTRUCT(BlueprintType)
struct FAttackWidgetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Widget Data")
	UTexture2D* crosshairTexture = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Widget Data")
	FLinearColor shotColor = FLinearColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Widget Data")
	FVector2D position = FVector2D::ZeroVector;
};
