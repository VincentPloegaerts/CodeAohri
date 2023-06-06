#include "P_DT_ProjectileSinusoidal.h"

#include "M_SaveManager.h"

void UP_DT_ProjectileSinusoidal::GetProjectileSettings(FProjectileSettings& _projectileSettings) const
{
	Super::GetProjectileSettings(_projectileSettings);
	_projectileSettings.sinusoidalSettings = sinusoidalSettings; 
}

#if WITH_EDITOR
void UP_DT_ProjectileSinusoidal::SaveProjectileData()
{
	FProjectileSinusoidalDataJSON _json = GetProjectileSinusoidalDataJSON();
	
	UM_SaveManager::Save(_json, Save_Projectile, GetName());
}

void UP_DT_ProjectileSinusoidal::LoadProjectileData()
{
	FProjectileSinusoidalDataJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Projectile, GetName());
	if (!_loadSuccess) return;

	LoadProjectileSinusoidalDataJSON(_json);
}

FProjectileSinusoidalDataJSON UP_DT_ProjectileSinusoidal::GetProjectileSinusoidalDataJSON() const
{
	FProjectileSinusoidalDataJSON _json;
	_json.InitGlobalJSON(GetProjectileGlobalDataJSON());

	_json.sinusoidalSettings = sinusoidalSettings;
	
	return _json;
}
void UP_DT_ProjectileSinusoidal::LoadProjectileSinusoidalDataJSON(const FProjectileSinusoidalDataJSON& _json)
{
	LoadProjectileGlobalDataJSON(_json);

	sinusoidalSettings = _json.sinusoidalSettings;
}
#endif
