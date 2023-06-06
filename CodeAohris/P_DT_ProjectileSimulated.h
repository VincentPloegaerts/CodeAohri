#pragma once

#include "CoreMinimal.h"
#include "P_DT_ProjectileGlobal.h"
#include "P_DT_ProjectileSimulated.generated.h"

USTRUCT()
struct FProjectileSimulatedDataJSON : public FProjectileGlobalDataJSON
{
	GENERATED_BODY()

	UPROPERTY()
	FProjectileSimulatedSettings simulatedSettings;
};

UCLASS()
class INSIDEUS_API UP_DT_ProjectileSimulated : public UP_DT_ProjectileGlobal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FProjectileSimulatedSettings simulatedSettings;

public:
	FORCEINLINE const FProjectileSimulatedSettings& GetProjectileSimulatedSettings() const { return simulatedSettings; }
	
public:
	virtual void GetProjectileSettings(FProjectileSettings& _projectileSettings) const override;

#if WITH_EDITOR
	virtual void SaveProjectileData() override;
	virtual void LoadProjectileData() override;

	FProjectileSimulatedDataJSON GetProjectileSimulatedDataJSON() const;
	void LoadProjectileSimulatedDataJSON(const FProjectileSimulatedDataJSON& _json);
#endif	
};
