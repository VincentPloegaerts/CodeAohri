#include "P_DT_ProjectileGuided.h"

#include "M_SaveManager.h"

void UP_DT_ProjectileGuided::GetProjectileSettings(FProjectileSettings& _projectileSettings) const
{
	Super::GetProjectileSettings(_projectileSettings);
	_projectileSettings.guidedSettings = guidedSettings;
}

#if WITH_EDITOR
void UP_DT_ProjectileGuided::SaveProjectileData()
{
	FProjectileGuidedDataJSON _json = GetProjectileGuidedDataJSON();
	
	UM_SaveManager::Save(_json, Save_Projectile, GetName());
}
void UP_DT_ProjectileGuided::LoadProjectileData()
{	
	FProjectileGuidedDataJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Projectile, GetName());
	if (!_loadSuccess) return;

	LoadProjectileGuidedDataJSON(_json);
}

FProjectileGuidedDataJSON UP_DT_ProjectileGuided::GetProjectileGuidedDataJSON() const
{
	FProjectileGuidedDataJSON _json;
    _json.InitGlobalJSON(GetProjectileGlobalDataJSON());
	
	_json.guidedSettings = guidedSettings;

	return _json;
}
void UP_DT_ProjectileGuided::LoadProjectileGuidedDataJSON(const FProjectileGuidedDataJSON& _json)
{
	LoadProjectileGlobalDataJSON(_json);
	
	guidedSettings = _json.guidedSettings;	
}
#endif
