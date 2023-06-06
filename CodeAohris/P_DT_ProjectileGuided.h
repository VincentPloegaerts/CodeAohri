#pragma once

#include "CoreMinimal.h"
#include "P_DT_ProjectileGlobal.h"
#include "P_DT_ProjectileGuided.generated.h"

USTRUCT()
struct FProjectileGuidedDataJSON : public FProjectileGlobalDataJSON
{
	GENERATED_BODY()

	UPROPERTY()
	FProjectileGuidedSettings guidedSettings;
};

UCLASS()
class INSIDEUS_API UP_DT_ProjectileGuided : public UP_DT_ProjectileGlobal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FProjectileGuidedSettings guidedSettings;

public:
	FORCEINLINE const FProjectileGuidedSettings& GetProjectileGuidedSettings() const { return guidedSettings; }
public:
	virtual void GetProjectileSettings(FProjectileSettings& _projectileSettings) const override;

#if WITH_EDITOR
	virtual void SaveProjectileData() override;
	virtual void LoadProjectileData() override;

	FProjectileGuidedDataJSON GetProjectileGuidedDataJSON() const;
	void LoadProjectileGuidedDataJSON(const FProjectileGuidedDataJSON& _json);
#endif
};
