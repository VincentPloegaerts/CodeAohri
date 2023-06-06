#pragma once
#include "CoreMinimal.h"
#include "UI_FloatingWidget.h"
#include "ENUM_HitFeedbackEnum.h"
#include "UI_DamageFloatingWidget.generated.h"

UCLASS()
class INSIDEUS_API AUI_DamageFloatingWidget : public AUI_FloatingWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Damage floating values", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FColor bossColor = FColor::White;
	
	UPROPERTY(EditAnywhere, Category = "Damage floating values", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FColor gpeColor = FColor::White;
	
	UPROPERTY(EditAnywhere, Category = "Damage floating values", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FColor playerColor = FColor::White;

public:
	UFUNCTION(BlueprintCallable) void SetType(const float _damage, const EHitFeedbackEnum _feedbackType) const;
};