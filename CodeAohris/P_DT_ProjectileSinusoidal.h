#pragma once

#include "CoreMinimal.h"
#include "P_DT_ProjectileGlobal.h"
#include "P_DT_ProjectileSinusoidal.generated.h"

USTRUCT()
struct FProjectileSinusoidalDataJSON : public FProjectileGlobalDataJSON
{
	GENERATED_BODY()

	UPROPERTY()
	FProjectileSinusoidalSettings sinusoidalSettings;
};

UCLASS()
class INSIDEUS_API UP_DT_ProjectileSinusoidal : public UP_DT_ProjectileGlobal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FProjectileSinusoidalSettings sinusoidalSettings;

public:
	FORCEINLINE const FProjectileSinusoidalSettings& GetProjectileSinusoidalSettings() const { return sinusoidalSettings; }
	
public:
	virtual void GetProjectileSettings(FProjectileSettings& _projectileSettings) const override;

#if WITH_EDITOR
	virtual void SaveProjectileData() override;
	virtual void LoadProjectileData() override;

	FProjectileSinusoidalDataJSON GetProjectileSinusoidalDataJSON() const;
	void LoadProjectileSinusoidalDataJSON(const FProjectileSinusoidalDataJSON& _json);
#endif
};
