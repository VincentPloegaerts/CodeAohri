#pragma once

#include "STRUCT_ProjectileGlobalSettings.h"
#include "STRUCT_ProjectileGuidedSettings.h"
#include "STRUCT_ProjectileSimulatedSettings.h"
#include "STRUCT_ProjectileBallisticSettings.h"
#include "STRUCT_ProjectileSinusoidalSettings.h"

#include "STRUCT_ProjectileSettings.generated.h"

USTRUCT(BlueprintType)
struct FKnockbackSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Projectile | Global")
	bool bActivateKnockBack = false;

	UPROPERTY(EditAnywhere, Category = "Projectile | Global", meta = (ClampMin = "0.01", ClampMax = "9999"))
	float fKnockBackStrenght = 10.f;

	UPROPERTY(EditAnywhere, Category = "Projectile | Global", meta = (ClampMin = "0.01", ClampMax = "10"))
	float fKnockBackDuration = 1.f;
};
USTRUCT(BlueprintType)
struct FProjectileSettings
{
	GENERATED_BODY()

#pragma region Global
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Global")
		bool bEditGlobalSettings = false;
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Global", meta = (EditCondition = "bEditGlobalSettings", ShowOnlyInnerProperties))
		FProjectileGlobalSettings globalSettings = FProjectileGlobalSettings();

#pragma endregion

#pragma region Knockback
	UPROPERTY(EditAnywhere, Category = "Projectile | Knockback")
	bool bEditProjectileKnockbackSettings = false;
	UPROPERTY(EditAnywhere, Category = "Projectile | Knockback", meta = (EditCondition = "bEditProjectileKnockbackSettings", ShowOnlyInnerProperties))
	FKnockbackSettings projectileKnockbackSettings;
#pragma endregion

#pragma region Ballistic
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Ballistic")
		bool bEditBallisticSettings = false;
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Ballistic", meta = (EditCondition = "bEditBallisticSettings"))
		FProjectileBallisticSettings ballisticSettings = FProjectileBallisticSettings();

#pragma endregion

#pragma region Guided
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Guided")
		bool bEditGuidedSettings = false;
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Guided", meta = (EditCondition = "bEditGuidedSettings"))
		FProjectileGuidedSettings guidedSettings = FProjectileGuidedSettings();

#pragma endregion

#pragma region Simulated
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Simulated")
		bool bEditSimulatedSettings = false;
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Simulated", meta = (EditCondition = "bEditSimulatedSettings"))
		FProjectileSimulatedSettings simulatedSettings = FProjectileSimulatedSettings();

#pragma endregion

#pragma region Sinusoidal
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Sinusoidal")
		bool bEditSinusoidalSettings = false;
	
	UPROPERTY(EditAnywhere, Category = "Projectile | Sinusoidal", meta = (EditCondition = "bEditSinusoidalSettings"))
		FProjectileSinusoidalSettings sinusoidalSettings = FProjectileSinusoidalSettings();

#pragma endregion 
};