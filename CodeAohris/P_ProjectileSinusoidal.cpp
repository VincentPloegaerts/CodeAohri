#include "P_ProjectileSinusoidal.h"

#include "M_SaveManager.h"

AP_ProjectileSinusoidal::AP_ProjectileSinusoidal()
{
	projectileType = Sinusoidal;

	if (staticMesh)
	{
		const ConstructorHelpers::FObjectFinder<UMaterialInterface> _material(TEXT("/Game/Graph/03_FX/02_Material/MFx_RedProjectile.MFx_RedProjectile"));
		if (_material.Succeeded()) staticMesh->SetMaterial(0, _material.Object);
	}
}

void AP_ProjectileSinusoidal::InitializeProjectile(const FProjectileSettings& _settings)
{
	Super::InitializeProjectile(_settings);
	
	sinusoidalSettings = _settings.sinusoidalSettings;
	
	AddActorLocalRotation(sinusoidalSettings.localRotation);
	fTime = sinusoidalSettings.fStartGap;
	
	InitializeSinusoidal();
}

void AP_ProjectileSinusoidal::ResetProjectile()
{
	Super::ResetProjectile();

	fTime = 0.0f;
	vStartLocation = FVector(0.0f);
	vMidLocation = FVector(0.0f);
	vEndLocation = FVector(0.0f);
}

void AP_ProjectileSinusoidal::MovementBehavior(const float& _deltaTime)
{
	fTime += _deltaTime * (globalSettings.fProjectileSpeed/100);
	
	const FVector& _lerp1 = FMath::Lerp(vStartLocation, vMidLocation, fTime);
	const FVector& _lerp2 = FMath::Lerp(vMidLocation, vEndLocation, fTime);
	const FVector& _location = FMath::Lerp(_lerp1, _lerp2, fTime);
	
	SetActorLocation(_location);

	CheckSinusoidalProgression();
}

void AP_ProjectileSinusoidal::InitializeSinusoidal()
{
	vStartLocation = GetActorLocation();
	vEndLocation = vStartLocation + GetActorForwardVector() * sinusoidalSettings.fPeriodRange;
	vMidLocation = FMath::Lerp(vStartLocation, vEndLocation, 0.5f) + GetActorUpVector() * sinusoidalSettings.fAmplitude;
}

void AP_ProjectileSinusoidal::CheckSinusoidalProgression()
{
	if (fTime > 1.0f)
	{
		fTime = 0.0f;
		sinusoidalSettings.fAmplitude *= -1;
		InitializeSinusoidal();
	}
}

#if WITH_EDITOR
void AP_ProjectileSinusoidal::SaveProjectile()
{
	FProjectileSinusoidalJSON _json = GetProjectileSinusoidalJSON();
	
	UM_SaveManager::Save(_json, Save_Projectile, GetName());
}

void AP_ProjectileSinusoidal::LoadProjectile()
{
	FProjectileSinusoidalJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Projectile, GetName());
	if (!_loadSuccess) return;
	
	LoadProjectileSinusoidalJSON(_json);
}

FProjectileSinusoidalJSON AP_ProjectileSinusoidal::GetProjectileSinusoidalJSON() const
{
	FProjectileSinusoidalJSON _json;
	_json.InitProjectileGlobalJSON(GetProjectileGlobalJSON());

	_json.sinusoidalSettings = sinusoidalSettings;
	return _json;
}

void AP_ProjectileSinusoidal::LoadProjectileSinusoidalJSON(const FProjectileSinusoidalJSON& _json)
{
	LoadProjectileGlobalJSON(_json);
	
	sinusoidalSettings = _json.sinusoidalSettings;
}
#endif