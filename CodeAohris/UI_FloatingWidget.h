#pragma once
#include "CoreMinimal.h"

#include "UI_FloatingText.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "UI_FloatingWidget.generated.h"

UCLASS(Abstract)
class INSIDEUS_API AUI_FloatingWidget : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Floating widget values",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UStaticMeshComponent* staticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Floating widget values",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UWidgetComponent* widget = nullptr;

protected:
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UUI_FloatingText* floatingText = nullptr;

	UPROPERTY(EditAnywhere, Category = "Floating widget values",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float fLifeSpan = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Floating widget values",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FVector impulseMin = FVector::Zero();

	UPROPERTY(EditAnywhere, Category = "Floating widget values",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FVector impulseMax = FVector::Zero();

	
public:	
	AUI_FloatingWidget();

private:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable) void InitFloatingText();
	UFUNCTION(BlueprintCallable) void AddImpulse() const;
};