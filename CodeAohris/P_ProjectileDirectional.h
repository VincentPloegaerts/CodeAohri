#pragma once
#include "CoreMinimal.h"
#include "P_Projectile.h"
#include "P_ProjectileDirectional.generated.h"

UCLASS(Blueprintable)
class INSIDEUS_API AP_ProjectileDirectional : public AP_Projectile
{
	GENERATED_BODY()

	UPROPERTY()
		FVector vForward = FVector(0.0f);

public:
	AP_ProjectileDirectional();
	
protected:
	virtual void InitializeProjectile(const FProjectileSettings& _settings) override;
	virtual void ResetProjectile() override;
	virtual void MovementBehavior(const float& _deltaTime) override;

	
#if WITH_EDITOR
	virtual void SaveProjectile() override;
	virtual void LoadProjectile() override;
#endif
};