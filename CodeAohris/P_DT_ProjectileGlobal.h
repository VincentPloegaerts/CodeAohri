#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "STRUCT_ProjectileSettings.h"

#include "P_DT_ProjectileGlobal.generated.h"

USTRUCT()
struct FProjectileGlobalDataJSON
{
	GENERATED_BODY()

	UPROPERTY()
	FProjectileGlobalSettings globalSettings;

	void InitGlobalJSON(FProjectileGlobalDataJSON _json)
	{
		globalSettings = _json.globalSettings;
	}
};

UCLASS(Abstract)
class INSIDEUS_API UP_DT_ProjectileGlobal : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
		FProjectileGlobalSettings globalSettings;
	
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
		FKnockbackSettings projectileKnockbackSettings;

public:
	FORCEINLINE const FProjectileGlobalSettings& GetProjectileGlobalSettings() const { return globalSettings; }
	
public:
	virtual void GetProjectileSettings(FProjectileSettings& _projectileSettings) const;

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Save") virtual void SaveProjectileData();
	UFUNCTION(CallInEditor, Category = "Save") virtual void LoadProjectileData();

	FProjectileGlobalDataJSON GetProjectileGlobalDataJSON() const;
	void LoadProjectileGlobalDataJSON(const FProjectileGlobalDataJSON& _json);	
#endif
};
