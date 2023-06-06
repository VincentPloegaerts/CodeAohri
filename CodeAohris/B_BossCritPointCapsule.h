#pragma once

#include "CoreMinimal.h"
#include "B_BossCritPoint.h"
#include "Components/CapsuleComponent.h"
#include "B_BossCritPointCapsule.generated.h"

UCLASS()
class INSIDEUS_API AB_BossCritPointCapsule : public AB_BossCritPoint
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCapsuleComponent* capsuleCollider = nullptr;

public:
	AB_BossCritPointCapsule();
	
private:
	virtual void GetCritPointSettings(FCritPointSettings& _critPointSettings) const override;
	virtual void SetCritPointSettings(const FCritPointSettings& _critPointSettings) override;

	virtual void DrawCritPointDebug() override;
};
