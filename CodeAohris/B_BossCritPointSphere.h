#pragma once

#include "CoreMinimal.h"
#include "B_BossCritPoint.h"
#include "Components/SphereComponent.h"
#include "B_BossCritPointSphere.generated.h"

UCLASS()
class INSIDEUS_API AB_BossCritPointSphere : public AB_BossCritPoint
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* sphereCollider = nullptr;

public:
	AB_BossCritPointSphere();

private:
	virtual void GetCritPointSettings(FCritPointSettings& _critPointSettings) const override;
	virtual void SetCritPointSettings(const FCritPointSettings& _critPointSettings) override;

	virtual void DrawCritPointDebug() override;
};
