#include "P_DT_ProjectileDataTable.h"

#include "M_SaveManager.h"
#include "P_Projectile.h"

#if WITH_EDITOR
void UP_DT_ProjectileDataTable::D_SaveProjectileDataTable()
{
	FProjectileDatatableJSON _json;
	_json.projectileBP = projectileBP;
	_json.projectileData = projectileData;
	UM_SaveManager::Save(_json, Save_Projectile, GetName());
}
void UP_DT_ProjectileDataTable::D_LoadProjectileDataTable()
{
	FProjectileDatatableJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Projectile, GetName());
	if (!_loadSuccess) return;
	
	projectileBP = _json.projectileBP;
	projectileData = _json.projectileData;
}

void UP_DT_ProjectileDataTable::P_SaveProjectileBP()
{
	const int& _max = projectileBP.Num();
	for (int i = 0; i < _max; i++)
	{
		AP_Projectile* _projectile = projectileBP[i].GetDefaultObject();
		if (!_projectile) continue;
		_projectile->SaveProjectile();
	}
}
void UP_DT_ProjectileDataTable::P_LoadProjectileBP()
{
	const int& _max = projectileBP.Num();
	for (int i = 0; i < _max; i++)
	{
		AP_Projectile* _projectile = projectileBP[i].GetDefaultObject();
		if (!_projectile) continue;
		_projectile->LoadProjectile();
	}
}

void UP_DT_ProjectileDataTable::P_SaveProjectileData()
{
	const int& _max = projectileData.Num();
	for (int i = 0; i < _max; i++)
	{
		UP_DT_ProjectileGlobal* _projectile = projectileData[i];
		if (!_projectile) continue;
		_projectile->SaveProjectileData();
	}
}
void UP_DT_ProjectileDataTable::P_LoadProjectileData()
{
	const int& _max = projectileData.Num();
	for (int i = 0; i < _max; i++)
	{
		UP_DT_ProjectileGlobal* _projectile = projectileData[i];
		if (!_projectile) continue;
		_projectile->LoadProjectileData();
	}
}
#endif
