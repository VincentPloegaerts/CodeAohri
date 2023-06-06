#include "P_DT_ProjectileBallistic.h"

#include "M_SaveManager.h"

void UP_DT_ProjectileBallistic::GetProjectileSettings(FProjectileSettings& _projectileSettings) const
{
	Super::GetProjectileSettings(_projectileSettings);
	_projectileSettings.ballisticSettings = ballisticSettings;
}

#if WITH_EDITOR
void UP_DT_ProjectileBallistic::SaveProjectileData()
{
	FProjectileBallisticDataJSON _json = GetProjectileBallisticDataJSON();
	
	UM_SaveManager::Save(_json, Save_Projectile, GetName());	
}
void UP_DT_ProjectileBallistic::LoadProjectileData()
{
	FProjectileBallisticDataJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Projectile, GetName());
	if (!_loadSuccess) return;

	LoadProjectileBallisticDataJSON(_json);	
}

FProjectileBallisticDataJSON UP_DT_ProjectileBallistic::GetProjectileBallisticDataJSON() const
{
	FProjectileBallisticDataJSON _json;
	_json.InitGlobalJSON(GetProjectileGlobalDataJSON());

	_json.ballisticSettings = ballisticSettings;

	return _json;
}
void UP_DT_ProjectileBallistic::LoadProjectileBallisticDataJSON(const FProjectileBallisticDataJSON& _json)
{
	LoadProjectileGlobalDataJSON(_json);

	ballisticSettings = _json.ballisticSettings;
}
#endif
