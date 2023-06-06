#pragma once
#include "Engine/DataTable.h"
#include "STRUCT_ProjectileGlobalSettings.generated.h"

USTRUCT(BlueprintType)
struct FProjectileGlobalSettings : public FTableRowBase
{
	GENERATED_BODY()
	
	//Speed movement of the Projectile
	UPROPERTY(EditAnywhere, Category = "Projectile | Global", meta = (UIMin = "0.0", UIMax = "10000.0", ClampMin = "0.0", ClampMax = "10000.0"))
		float fProjectileSpeed = 1000.0f;
	
	//Damage of the Projectile
	UPROPERTY(EditAnywhere, Category = "Projectile | Global")
		float fDamage = 1.0f;

	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
};

inline void FProjectileGlobalSettings::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
		
}