#include "P_ProjectileDirectional.h"

#include "M_SaveManager.h"

AP_ProjectileDirectional::AP_ProjectileDirectional()
{
	projectileType = Directional;

	if (staticMesh)
	{
		const ConstructorHelpers::FObjectFinder<UMaterialInterface> _material(TEXT("/Game/Graph/03_FX/02_Material/MFx_PurpleProjectile.MFx_PurpleProjectile"));
		if (_material.Succeeded()) staticMesh->SetMaterial(0, _material.Object);
	}
}

void AP_ProjectileDirectional::InitializeProjectile(const FProjectileSettings& _settings)
{
	Super::InitializeProjectile(_settings);
	vForward = GetActorForwardVector();
}

void AP_ProjectileDirectional::ResetProjectile()
{
	Super::ResetProjectile();
	vForward = FVector(0.0f);
}

void AP_ProjectileDirectional::MovementBehavior(const float& _deltaTime)
{
	AddActorWorldOffset(vForward * globalSettings.fProjectileSpeed * _deltaTime);
}

#if WITH_EDITOR
void AP_ProjectileDirectional::SaveProjectile()
{
	FProjectileGlobalJSON _json = GetProjectileGlobalJSON();
	
	UM_SaveManager::Save(_json, Save_Projectile, GetName());
}

void AP_ProjectileDirectional::LoadProjectile()
{
	FProjectileGlobalJSON _json;
	
	UM_SaveManager::Load(_json, Save_Projectile, GetName());
	LoadProjectileGlobalJSON(_json);
}
#endif