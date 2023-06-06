#pragma once
#include "CoreMinimal.h"
#include "P_Projectile.h"
#include "P_ProjectileSimulated.generated.h"

USTRUCT()
struct FProjectileSimulatedJSON : public FProjectileGlobalJSON
{
	GENERATED_BODY()

	UPROPERTY()
	FProjectileSimulatedSettings simulatedSettings;
};

UCLASS(Blueprintable)
class INSIDEUS_API AP_ProjectileSimulated : public AP_Projectile
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		bool bRotate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Simulated", meta = (UIMin = "-89", UIMax = "-75", ClampMin = "-89", ClampMax = "-75"))
		float minLookDown = -80.0f;

	UPROPERTY()
		FRotator rScaledGravity = FRotator(0, 0, 0);
	
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
		FProjectileSimulatedSettings simulatedSettings = FProjectileSimulatedSettings();

public:
	FORCEINLINE const FProjectileSimulatedSettings& GetProjectileSimulatedSettings() const { return simulatedSettings; }
	FORCEINLINE const float& GetProjectileSimulatedMinLookDown() const { return minLookDown; }
	
public:
	AP_ProjectileSimulated();

private:
	virtual void InitializeProjectile(const FProjectileSettings& _settings) override;
	virtual void ResetProjectile() override;
	virtual void MovementBehavior(const float& _deltaTime) override;

	void RotateProjectileDown(const float& _deltaTime);
	float GetGravityScale(const float& _min, const float& _max) const;

#if WITH_EDITOR
	virtual void SaveProjectile() override;
	virtual void LoadProjectile() override;

	FProjectileSimulatedJSON GetProjectileSimulatedJSON() const;
	void LoadProjectileSimulatedJSON(const FProjectileSimulatedJSON& _json);
#endif
};
