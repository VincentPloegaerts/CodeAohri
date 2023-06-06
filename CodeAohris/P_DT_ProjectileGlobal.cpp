#include "P_DT_ProjectileGlobal.h"

void UP_DT_ProjectileGlobal::GetProjectileSettings(FProjectileSettings& _projectileSettings) const
{
	_projectileSettings.globalSettings = globalSettings;

	if (!_projectileSettings.bEditProjectileKnockbackSettings)
		_projectileSettings.projectileKnockbackSettings = projectileKnockbackSettings;
}

#if WITH_EDITOR
void UP_DT_ProjectileGlobal::SaveProjectileData()
{
	//Declaration in child class...
	
}
void UP_DT_ProjectileGlobal::LoadProjectileData()
{
	//Declaration in child class...	
}

FProjectileGlobalDataJSON UP_DT_ProjectileGlobal::GetProjectileGlobalDataJSON() const
{
	FProjectileGlobalDataJSON _json;
	_json.globalSettings = globalSettings;
	return _json;
}
void UP_DT_ProjectileGlobal::LoadProjectileGlobalDataJSON(const FProjectileGlobalDataJSON& _json)
{
	globalSettings = _json.globalSettings;
}
#endif