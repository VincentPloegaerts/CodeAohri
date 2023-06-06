#pragma once

#include "CoreMinimal.h"
#include "P_DT_PatternDataTable.h"
#include "P_DT_ProjectileBallistic.h"
#include "P_DT_ProjectileDirectional.h"
#include "P_DT_ProjectileGlobal.h"
#include "P_DT_ProjectileGuided.h"
#include "P_DT_ProjectileSimulated.h"
#include "P_DT_ProjectileSinusoidal.h"

#include "STRUCT_ShootSettings.h"
#include "STRUCT_ShootPointSettings.h"
#include "STRUCT_ShootLineSettings.h"
#include "STRUCT_ShootCircleSettings.h"
#include "STRUCT_ShootArcSettings.h"
#include "STRUCT_ShootCrossSettings.h"
#include "STRUCT_ShootRandom2DSettings.h"
#include "STRUCT_ShootRandom3DSettings.h"

#include "P_DT_ShootDataTable.generated.h"

USTRUCT()
struct FShootDataTableJSON : public FPatternDataTableJSON
{
	GENERATED_BODY()

	UPROPERTY()
	FShootSettings shootSettings;
	
	UPROPERTY()
	bool bShootPoint = false;
	UPROPERTY()
	FShootPointSettings shootPointSettings;

	UPROPERTY()
	bool bShootLine = false;
	UPROPERTY()
	FShootLineSettings shootLineSettings;

	UPROPERTY()
	bool bShootCircle = false;
	UPROPERTY()
	FShootCircleSettings shootCircleSettings;
	
	UPROPERTY()
	bool bShootArc = false;
	UPROPERTY()
	FShootArcSettings shootArcSettings;

	UPROPERTY()
	bool bShootCross = false;
	UPROPERTY()
	FShootCrossSettings shootCrossSettings;
	
	UPROPERTY()
	bool bShootRandom2D = false;
	UPROPERTY()
	FShootRandom2DSettings shootRandom2DSettings;

	UPROPERTY()
	bool bShootRandom3D = false;
	UPROPERTY()
	FShootRandom3DSettings shootRandom3DSettings;

	UPROPERTY()
	FProjectileSettings projectileSettings;

	UPROPERTY()
	UP_DT_ProjectileDirectional* directional = nullptr;
	UPROPERTY()
	UP_DT_ProjectileGuided* guided = nullptr;
	UPROPERTY()
	UP_DT_ProjectileSimulated* simulated = nullptr;
	UPROPERTY()
	UP_DT_ProjectileBallistic* ballistic = nullptr;
	UPROPERTY()
	UP_DT_ProjectileSinusoidal* sinusoidal = nullptr;
		
	void InitPatternJSON(FPatternDataTableJSON _patternJSON)
	{
		rotationSettings = _patternJSON.rotationSettings;
		bRotator = _patternJSON.bRotator;
		rotatorSettings = _patternJSON.rotatorSettings;
		bInstant = _patternJSON.bInstant;
		instantSettings = _patternJSON.instantSettings;
		bAngle = _patternJSON.bAngle;
		angleSettings = _patternJSON.angleSettings;
		bTarget = _patternJSON.bTarget;
		targetSettings = _patternJSON.targetSettings;
		bEditMovementOnAxe = _patternJSON.bEditMovementOnAxe;
		movementOnAxeSetting = _patternJSON.movementOnAxeSetting;
		bEditMovementToTarget = _patternJSON.bEditMovementToTarget;
		movementToTargetSettings = _patternJSON.movementToTargetSettings;
		bEditMovementTeleport = _patternJSON.bEditMovementTeleport;
		movementTeleportSettings = _patternJSON.movementTeleportSettings;
	}
};

/*DataTable used for shooting properties.*/
UCLASS(Blueprintable)
class INSIDEUS_API UP_DT_ShootDataTable : public UP_DT_PatternDataTable
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FShootSettings shootSettings;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Enable")
	bool bShootPoint = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (EditCondition = "bShootPoint"))
	FShootPointSettings shootPointSettings;

	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Enable")
	bool bShootLine = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (EditCondition = "bShootLine"))
	FShootLineSettings shootLineSettings;

	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Enable")
	bool bShootCircle = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (EditCondition = "bShootCircle"))
	FShootCircleSettings shootCircleSettings;

	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Enable")
	bool bShootArc = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (EditCondition = "bShootArc"))
	FShootArcSettings shootArcSettings;

	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Enable")
	bool bShootCross = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (EditCondition = "bShootCross"))
	FShootCrossSettings shootCrossSettings;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Enable", meta = (EditCondition = "!bShootRandom3D", EditConditionHides))
	bool bShootRandom2D = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (EditCondition = "bShootRandom2D"))
	FShootRandom2DSettings shootRandom2DSettings;

	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Enable", meta = (EditCondition = "!bShootRandom2D", EditConditionHides))
	bool bShootRandom3D = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (EditCondition = "bShootRandom3D"))
	FShootRandom3DSettings shootRandom3DSettings;
	
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TEnumAsByte<EProjectileType> projectileType = EProjectileType::Directional;

	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Directional", EditConditionHides))
	UP_DT_ProjectileDirectional* directional = nullptr;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Guided || projectileType == EProjectileType::FatGuided", EditConditionHides))
	UP_DT_ProjectileGuided* guided = nullptr;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Simulated", EditConditionHides))
	UP_DT_ProjectileSimulated* simulated = nullptr;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Ballistic", EditConditionHides))
	UP_DT_ProjectileBallistic* ballistic = nullptr;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Sinusoidal", EditConditionHides))
	UP_DT_ProjectileSinusoidal* sinusoidal = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "bValidGlobal", EditConditionHides))
	bool bEditGlobalSettings = false;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "bEditGlobalSettings && bValidGlobal", EditConditionHides))
	FProjectileGlobalSettings globalSettings;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "bValidGlobal", EditConditionHides))
	bool bEditProjectileKnockbackSettings = false;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "bEditProjectileKnockbackSettings && bValidGlobal", EditConditionHides))
	FKnockbackSettings projectileKnockbackSettings;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Guided && bValidGuided", EditConditionHides))
	bool bEditGuidedSettings = false;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Guided && bEditGuidedSettings && bValidGuided", EditConditionHides))
	FProjectileGuidedSettings guidedSettings;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Simulated && bValidSimulated", EditConditionHides))
	bool bEditSimulatedSettings = false;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Simulated && bEditSimulatedSettings && bValidSimulated", EditConditionHides))
	FProjectileSimulatedSettings simulatedSettings;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Ballistic && bValidBallistic", EditConditionHides))
	bool bEditBallisticSettings = false;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Ballistic && bEditBallisticSettings && bValidBallistic", EditConditionHides))
	FProjectileBallisticSettings ballisticSettings;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Sinusoidal && bValidSinusoidal", EditConditionHides))
	bool bEditSinusoidalSettings = false;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (EditCondition = "projectileType == EProjectileType::Sinusoidal && bEditSinusoidalSettings && bValidSinusoidal", EditConditionHides))
	FProjectileSinusoidalSettings sinusoidalSettings;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	bool bValidGlobal = false;
	UPROPERTY()
	bool bValidDirectional = false;
	UPROPERTY()
	bool bValidGuided = false;
	UPROPERTY()
	bool bValidSimulated = false;
	UPROPERTY()
	bool bValidBallistic = false;
	UPROPERTY()
	bool bValidSinusoidal = false;
#endif
	
public:
	FORCEINLINE const bool& GetShootPointState() const { return bShootPoint; }
	FORCEINLINE const bool& GetShootLineState() const { return bShootLine; }
	FORCEINLINE const bool& GetShootCircleState() const { return bShootCircle; }
	FORCEINLINE const bool& GetShootArcState() const { return bShootArc; }
	FORCEINLINE const bool& GetShootCrossState() const { return bShootCross; }
	FORCEINLINE const bool& GetShootRandom2DState() const { return bShootRandom2D; }
	FORCEINLINE const bool& GetShootRandom3DState() const { return bShootRandom3D; }
	
	FORCEINLINE const FShootSettings& GetShootSettings() const { return shootSettings; }
	FORCEINLINE const FShootPointSettings& GetShootPointSettings() const { return shootPointSettings; }
	FORCEINLINE const FShootLineSettings& GetShootLineSettings() const { return shootLineSettings; }
	FORCEINLINE const FShootCircleSettings& GetShootCircleSettings() const { return shootCircleSettings; }
	FORCEINLINE const FShootArcSettings& GetShootArcSettings() const { return shootArcSettings; }
	FORCEINLINE const FShootCrossSettings& GetShootCrossSettings() const { return shootCrossSettings; }
	FORCEINLINE const FShootRandom2DSettings& GetShootRandom2DSettings() const { return shootRandom2DSettings; }
	FORCEINLINE const FShootRandom3DSettings& GetShootRandom3DSettings() const { return shootRandom3DSettings; }
	FProjectileSettings GetProjectileSettings() const;

#if WITH_EDITOR
private:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
public:
	virtual void SavePattern() override;
	virtual void LoadPattern() override;
	
	FShootDataTableJSON GetShootDataTableJSON() const;
	void LoadShootDataTableJSON(const FShootDataTableJSON& _json); 
	
	virtual void InitializePatternData(FPatternData& _data) const override;
#endif
};