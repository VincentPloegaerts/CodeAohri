#include "P_DT_ProjectileDirectional.h"

#include "M_SaveManager.h"

#if WITH_EDITOR
void UP_DT_ProjectileDirectional::SaveProjectileData()
{
	FProjectileGlobalDataJSON _json = GetProjectileGlobalDataJSON();
	
	UM_SaveManager::Save(_json, Save_Projectile, GetName());
}
void UP_DT_ProjectileDirectional::LoadProjectileData()
{	
	FProjectileGlobalDataJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Projectile, GetName());
	if (!_loadSuccess) return;

	LoadProjectileGlobalDataJSON(_json);
}
#endif