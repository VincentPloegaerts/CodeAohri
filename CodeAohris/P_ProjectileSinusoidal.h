#pragma once
#include "CoreMinimal.h"
#include "P_Projectile.h"
#include "P_ProjectileSinusoidal.generated.h"

USTRUCT()
struct FProjectileSinusoidalJSON : public FProjectileGlobalJSON
{
	GENERATED_BODY()

	UPROPERTY()
	FProjectileSinusoidalSettings sinusoidalSettings;
};

UCLASS(Blueprintable)
class INSIDEUS_API AP_ProjectileSinusoidal : public AP_Projectile
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		float fTime = 0.0f;
	
	UPROPERTY()
		FVector vStartLocation = FVector(0.0f);
	
	UPROPERTY()
		FVector vMidLocation = FVector(0.0f);
	
	UPROPERTY()
		FVector vEndLocation = FVector(0.0f);

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
		FProjectileSinusoidalSettings sinusoidalSettings = FProjectileSinusoidalSettings();

public:
	FORCEINLINE const FProjectileSinusoidalSettings& GetProjectileSinusoidalSettings() const { return sinusoidalSettings; }
	
public:
	AP_ProjectileSinusoidal();
	
private:
	virtual void InitializeProjectile(const FProjectileSettings& _settings) override;
	virtual void ResetProjectile() override;
	virtual void MovementBehavior(const float& _deltaTime) override;

	void InitializeSinusoidal();
	void CheckSinusoidalProgression();

#if WITH_EDITOR
	virtual void SaveProjectile() override;
	virtual void LoadProjectile() override;

	FProjectileSinusoidalJSON GetProjectileSinusoidalJSON() const;
	void LoadProjectileSinusoidalJSON(const FProjectileSinusoidalJSON& _json);
#endif
};
