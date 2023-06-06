#include "P_ProjectileBallistic.h"

#include "M_SaveManager.h"

AP_ProjectileBallistic::AP_ProjectileBallistic()
{
	projectileType = Ballistic;
}

void AP_ProjectileBallistic::InitializeProjectile(const FProjectileSettings& _settings)
{
	Super::InitializeProjectile(_settings);
	
	ballisticSettings = _settings.ballisticSettings;
	InitializeBallistic();	
}

void AP_ProjectileBallistic::ResetProjectile()
{
	Super::ResetProjectile();
	
	vStartLocation = FVector(0.0f);
	vMidLocation = FVector(0.0f);
	vEndLocation = FVector(0.0f);
	fTime = 0.0f;
}

void AP_ProjectileBallistic::MovementBehavior(const float& _deltaTime)
{
	fTime += _deltaTime * (globalSettings.fProjectileSpeed / 100);

	const FVector& _lerp1 = FMath::Lerp(vStartLocation, vMidLocation, fTime);
	const FVector& _lerp2 = FMath::Lerp(vMidLocation, vEndLocation, fTime);
	const FVector& _location = FMath::Lerp(_lerp1, _lerp2, fTime);

	SetActorLocation(_location);
}

void AP_ProjectileBallistic::InitializeBallistic()
{
	vStartLocation = GetActorLocation();
	vEndLocation = GetActorForwardVector() * ballisticSettings.fRangeBallistic + vStartLocation;
	vMidLocation = FMath::Lerp(vStartLocation, vEndLocation, 0.5f) + FVector(0.0f, 0.0f, ballisticSettings.fOffsetZ);
}

#if WITH_EDITOR
void AP_ProjectileBallistic::SaveProjectile()
{
	FProjectileBallisticJSON _json = GetProjectileBallisticJSON();
	
	UM_SaveManager::Save(_json, Save_Projectile, GetName());	
}

void AP_ProjectileBallistic::LoadProjectile()
{
	FProjectileBallisticJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Projectile, GetName());
	if (!_loadSuccess) return;
	
	LoadProjectileBallisticJSON(_json);
}

FProjectileBallisticJSON AP_ProjectileBallistic::GetProjectileBallisticJSON() const
{
	FProjectileBallisticJSON _json;
	_json.InitProjectileGlobalJSON(GetProjectileGlobalJSON());

	_json.ballisticSettings = ballisticSettings;
	return _json;
}

void AP_ProjectileBallistic::LoadProjectileBallisticJSON(const FProjectileBallisticJSON& _json)
{
	LoadProjectileGlobalJSON(_json);
	
	ballisticSettings = _json.ballisticSettings;
}
#endif