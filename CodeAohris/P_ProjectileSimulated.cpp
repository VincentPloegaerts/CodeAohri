#include "P_ProjectileSimulated.h"

#include "M_SaveManager.h"

AP_ProjectileSimulated::AP_ProjectileSimulated()
{
	projectileType = Simulated;

	if (staticMesh)
	{
		const ConstructorHelpers::FObjectFinder<UMaterialInterface> _material(TEXT("/Game/Graph/03_FX/02_Material/MFx_OrangeProjectile.MFx_OrangeProjectile"));
		if (_material.Succeeded()) staticMesh->SetMaterial(0, _material.Object);
	}
}

void AP_ProjectileSimulated::InitializeProjectile(const FProjectileSettings& _settings)
{
	Super::InitializeProjectile(_settings);
	
	simulatedSettings = _settings.simulatedSettings;

	rScaledGravity = FRotator(-1  * GetGravityScale(simulatedSettings.fGravityScaleMin,  simulatedSettings.fGravityScaleMax), 0.0f, 0.0f);
	bRotate = true;
}

void AP_ProjectileSimulated::ResetProjectile()
{
	Super::ResetProjectile();

	rScaledGravity = FRotator(0.0f);
	bRotate = false;
}

void AP_ProjectileSimulated::MovementBehavior(const float& _deltaTime)
{
	AddActorWorldOffset(GetActorForwardVector() * globalSettings.fProjectileSpeed * _deltaTime);
	if (!bRotate) return;
	
	RotateProjectileDown(_deltaTime);
}

void AP_ProjectileSimulated::RotateProjectileDown(const float& _deltaTime)
{	
	AddActorLocalRotation(rScaledGravity * _deltaTime);

	const FRotator& _rotation = GetActorRotation();
	if (_rotation.Pitch < minLookDown)
	{
		bRotate = false;
		SetActorRotation(FRotator(minLookDown, _rotation.Yaw, _rotation.Roll));
	}
}

float AP_ProjectileSimulated::GetGravityScale(const float& _min, const float& _max) const
{
	return FMath::RandRange(_min, _max);
}

#if WITH_EDITOR
void AP_ProjectileSimulated::SaveProjectile()
{
	FProjectileSimulatedJSON _json = GetProjectileSimulatedJSON();
	
	UM_SaveManager::Save(_json, Save_Projectile, GetName());
}

void AP_ProjectileSimulated::LoadProjectile()
{
	FProjectileSimulatedJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Projectile, GetName());
	if (!_loadSuccess) return;
	
	LoadProjectileSimulatedJSON(_json);
}

FProjectileSimulatedJSON AP_ProjectileSimulated::GetProjectileSimulatedJSON() const
{
	FProjectileSimulatedJSON _json;
	_json.InitProjectileGlobalJSON(GetProjectileGlobalJSON());

	_json.simulatedSettings = simulatedSettings;
	return _json;
}

void AP_ProjectileSimulated::LoadProjectileSimulatedJSON(const FProjectileSimulatedJSON& _json)
{
	LoadProjectileGlobalJSON(_json);
	
	simulatedSettings = _json.simulatedSettings;
}
#endif