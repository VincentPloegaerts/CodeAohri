#pragma once

#include "CoreMinimal.h"
#include "P_DT_ProjectileGlobal.h"
#include "P_DT_ProjectileBallistic.generated.h"

USTRUCT()
struct FProjectileBallisticDataJSON : public FProjectileGlobalDataJSON
{
	GENERATED_BODY()

	UPROPERTY()
	FProjectileBallisticSettings ballisticSettings;
};

UCLASS()
class INSIDEUS_API UP_DT_ProjectileBallistic : public UP_DT_ProjectileGlobal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FProjectileBallisticSettings ballisticSettings;

public:
	FORCEINLINE const FProjectileBallisticSettings& GetProjectileBallisticSettings() const { return ballisticSettings; }
	
public:
	virtual void GetProjectileSettings(FProjectileSettings& _projectileSettings) const override;

#if WITH_EDITOR
	virtual void SaveProjectileData() override;
	virtual void LoadProjectileData() override;

	FProjectileBallisticDataJSON GetProjectileBallisticDataJSON() const;
	void LoadProjectileBallisticDataJSON(const FProjectileBallisticDataJSON& _json);
#endif	
};
