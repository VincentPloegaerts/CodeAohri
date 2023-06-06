#pragma once
#include "CoreMinimal.h"

#include "Components/CapsuleComponent.h"

#include "P_Projectile.h"
#include "P_ProjectileGuided.generated.h"

USTRUCT()
struct FProjectileGuidedJSON : public FProjectileGlobalJSON
{
	GENERATED_BODY()

	UPROPERTY()
	FProjectileGuidedSettings guidedSettings;
};

UCLASS(Blueprintable)
class INSIDEUS_API AP_ProjectileGuided : public AP_Projectile
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* projectileGuidedCollider = nullptr;
	
protected:
	UPROPERTY()
		bool bAtTarget = false;
	
	UPROPERTY()
		bool bRotate = false;
	
	UPROPERTY()
		float fELapsedTime = 0.0f;
	
	UPROPERTY()
		float fSpeed = 0.0f;

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
		FProjectileGuidedSettings guidedSettings = FProjectileGuidedSettings();
	
	UPROPERTY()
		AActor* target = nullptr;

public:
	FORCEINLINE const FProjectileGuidedSettings& GetProjectileGuidedSettings() const { return guidedSettings; }
	
public:
	AP_ProjectileGuided();

	virtual void EnableProjectile() const override;
	virtual void DisableProjectile() const override;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void InitializeProjectile(const FProjectileSettings& _settings) override;
	virtual void ResetProjectile() override;
	UFUNCTION(BlueprintCallable) virtual void MovementBehavior(const float& _deltaTime) override;

private:
	void RotateToTarget(const float& _deltaTime);
	bool IsAtTarget() const;
	void CheckWaitTime(const float& _deltaTime);

#if WITH_EDITOR
	virtual void SaveProjectile() override;
	virtual void LoadProjectile() override;

	FProjectileGuidedJSON GetProjectileGuidedJSON() const;
	void LoadProjectileGuidedJSON(const FProjectileGuidedJSON& _json);
#endif
};
