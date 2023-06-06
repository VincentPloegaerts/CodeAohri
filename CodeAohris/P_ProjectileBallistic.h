#pragma once
#include "CoreMinimal.h"
#include "P_Projectile.h"
#include "P_ProjectileBallistic.generated.h"

USTRUCT()
struct FProjectileBallisticJSON : public FProjectileGlobalJSON
{
	GENERATED_BODY()

	UPROPERTY()
	FProjectileBallisticSettings ballisticSettings;
};

UCLASS(Blueprintable)
class INSIDEUS_API AP_ProjectileBallistic : public AP_Projectile
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		float fTime = 0.0f;

	UPROPERTY()
		FVector vStartLocation = FVector(0.0f);
	
	UPROPERTY()
		FVector vMidLocation = FVector(0.0f);

	UPROPERTY()
		FVector vEndLocation = FVector(0.0f);

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
		FProjectileBallisticSettings ballisticSettings = FProjectileBallisticSettings();

public:
	FORCEINLINE const FProjectileBallisticSettings& GetProjectileBallisticSettings() const { return ballisticSettings; }
	
public:
	AP_ProjectileBallistic();
	
private:
	virtual void InitializeProjectile(const FProjectileSettings& _settings) override;
	virtual void ResetProjectile() override;
	virtual void MovementBehavior(const float& _deltaTime) override;

	void InitializeBallistic();

#if WITH_EDITOR
	virtual void SaveProjectile() override;
	virtual void LoadProjectile() override;

	FProjectileBallisticJSON GetProjectileBallisticJSON() const;
	void LoadProjectileBallisticJSON(const FProjectileBallisticJSON& _json);
#endif
};