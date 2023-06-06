#pragma once

#include "CoreMinimal.h"
#include "P_DT_ProjectileGlobal.h"
#include "Engine/DataAsset.h"
#include "P_DT_ProjectileDataTable.generated.h"

class AP_Projectile;

USTRUCT()
struct FProjectileDatatableJSON
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TSubclassOf<AP_Projectile>> projectileBP;
	UPROPERTY()
	TArray<UP_DT_ProjectileGlobal*> projectileData;
};

UCLASS()
class INSIDEUS_API UP_DT_ProjectileDataTable : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TArray<TSubclassOf<AP_Projectile>> projectileBP;
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TArray<UP_DT_ProjectileGlobal*> projectileData;

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Save | DATA TABLE") void D_SaveProjectileDataTable();
	UFUNCTION(CallInEditor, Category = "Save | DATA TABLE") void D_LoadProjectileDataTable();

	UFUNCTION(CallInEditor, Category = "Save | BLUEPRINTS") void P_SaveProjectileBP();
	UFUNCTION(CallInEditor, Category = "Save | BLUEPRINTS") void P_LoadProjectileBP();

	UFUNCTION(CallInEditor, Category = "Save | PROJECTILE DATA") void P_SaveProjectileData();
	UFUNCTION(CallInEditor, Category = "Save | PROJECTILE DATA") void P_LoadProjectileData();
#endif
};
