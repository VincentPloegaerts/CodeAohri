#include "P_DT_ProjectileSimulated.h"

#include "M_SaveManager.h"

void UP_DT_ProjectileSimulated::GetProjectileSettings(FProjectileSettings& _projectileSettings) const
{
	Super::GetProjectileSettings(_projectileSettings);
	_projectileSettings.simulatedSettings = simulatedSettings;
}

#if WITH_EDITOR
void UP_DT_ProjectileSimulated::SaveProjectileData()
{
	FProjectileSimulatedDataJSON _json = GetProjectileSimulatedDataJSON();
	
	UM_SaveManager::Save(_json, Save_Projectile, GetName());
}
void UP_DT_ProjectileSimulated::LoadProjectileData()
{
	FProjectileSimulatedDataJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Projectile, GetName());
	if (!_loadSuccess) return;

	LoadProjectileSimulatedDataJSON(_json);
}

FProjectileSimulatedDataJSON UP_DT_ProjectileSimulated::GetProjectileSimulatedDataJSON() const
{
	FProjectileSimulatedDataJSON _json;
	_json.InitGlobalJSON(GetProjectileGlobalDataJSON());

	_json.simulatedSettings = simulatedSettings;
	
	return _json;
}
void UP_DT_ProjectileSimulated::LoadProjectileSimulatedDataJSON(const FProjectileSimulatedDataJSON& _json)
{
	LoadProjectileGlobalDataJSON(_json);

	simulatedSettings = _json.simulatedSettings;
}
#endif