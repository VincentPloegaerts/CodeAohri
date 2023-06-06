#pragma once

#include "CoreMinimal.h"
#include "P_PoolingTrigger.h"
#include "Components/SphereComponent.h"
#include "P_PoolingTriggerSphere.generated.h"

UCLASS()
class INSIDEUS_API AP_PoolingTriggerSphere : public AP_PoolingTrigger
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* sphereComponent = nullptr;
	
public:
	AP_PoolingTriggerSphere();

	virtual void EnablePoolingTrigger() override;
	virtual void DisablePoolingTrigger() override;

	//Mainly called by the GPE Shot to set Sphere Radius
	void SetTriggerSphereRadius(const float& _radius) const;

#if WITH_EDITOR
	virtual void DrawPoolingTriggerDebug() override;
#endif
};
