#pragma once

#include "CoreMinimal.h"
#include "P_DT_BaseDataTable.h"

#include "P_Pattern.h"

#include "STRUCT_RotationSettings.h"
#include "STRUCT_RotatorSettings.h"
#include "STRUCT_InstantSettings.h"
#include "STRUCT_AngleSettings.h"
#include "STRUCT_TargetSettings.h"

#include "STRUCT_MovementOnAxeSettings.h"
#include "STRUCT_MovementToTargetSettings.h"
#include "STRUCT_MovementTeleportSettings.h"

#include "P_DT_PatternDataTable.generated.h"

USTRUCT()
struct FPatternDataTableJSON : public FBaseDataTableJSON
{
	GENERATED_BODY()

	UPROPERTY()
	FRotationSettings rotationSettings;

	UPROPERTY()
	bool bRotator = false;
	UPROPERTY()
	FRotatorSettings rotatorSettings;

	UPROPERTY()
	bool bInstant = false;
	UPROPERTY()
	FInstantSettings instantSettings;

	UPROPERTY()
	bool bAngle = false;
	UPROPERTY()
	FAngleSettings angleSettings;

	UPROPERTY()
	bool bTarget = false;
	UPROPERTY()
	FTargetSettings targetSettings;
	
	UPROPERTY()
	bool bEditMovementOnAxe = false;
	UPROPERTY()
	FMovementOnAxeSettings movementOnAxeSetting;

	UPROPERTY()
	bool bEditMovementToTarget = false;
	UPROPERTY()
	FMovementToTargetSettings movementToTargetSettings;
	
	UPROPERTY()
	bool bEditMovementTeleport = false;
	UPROPERTY()
	FMovementTeleportSettings movementTeleportSettings;
	
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

UCLASS(Abstract)
class INSIDEUS_API UP_DT_PatternDataTable : public UP_DT_BaseDataTable
{
	GENERATED_BODY()
	
	//Rotation
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings", meta = (EditCondition = "bRotator || bInstant || bAngle || bTarget"))
	FRotationSettings rotationSettings;

	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Enable")
	bool bRotator = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings", meta = (EditCondition = "bRotator"))
	FRotatorSettings rotatorSettings;

	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Enable")
	bool bInstant = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings", meta = (EditCondition = "bInstant"))
	FInstantSettings instantSettings;

	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Enable")
	bool bAngle = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings", meta = (EditCondition = "bAngle"))
	FAngleSettings angleSettings;

	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Enable")
	bool bTarget = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Rotation | Settings", meta = (EditCondition = "bTarget"))
	FTargetSettings targetSettings;

	//Movement
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Enable")
	bool bEditMovementOnAxe = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings", meta = (EditCondition = "bEditMovementOnAxe"))
	FMovementOnAxeSettings movementOnAxeSetting;

	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Enable")
	bool bEditMovementToTarget = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings", meta = (EditCondition = "bEditMovementToTarget"))
	FMovementToTargetSettings movementToTargetSettings;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Enable")
	bool bEditMovementTeleport = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Movement | Settings", meta = (EditCondition = "bEditMovementTeleport"))
	FMovementTeleportSettings movementTeleportSettings;
	
public:	
	FORCEINLINE const bool& GetRotatorState() const { return bRotator; }
	FORCEINLINE const bool& GetInstantState() const { return bInstant; }
	FORCEINLINE const bool& GetAngleState() const { return bAngle; }
	FORCEINLINE const bool& GetTargetState() const { return bTarget; }

	FORCEINLINE const bool& GetMovementOnAxeState() const { return bEditMovementOnAxe; }
	FORCEINLINE const bool& GetMovementToTargetState() const { return bEditMovementToTarget; }
	FORCEINLINE const bool& GetMovementTeleportState() const { return bEditMovementTeleport; }
	
	FORCEINLINE const FRotationSettings& GetRotationSettings() const { return rotationSettings; }
	FORCEINLINE const FRotatorSettings& GetRotatorSettings() const { return rotatorSettings; }
	FORCEINLINE const FInstantSettings& GetInstantSettings() const { return instantSettings; }
	FORCEINLINE const FAngleSettings& GetAngleSettings() const { return angleSettings; }
	FORCEINLINE const FTargetSettings& GetTargetSettings() const { return targetSettings; }
	
	FORCEINLINE const FMovementOnAxeSettings& GetMovementOnAxeSettings() const { return movementOnAxeSetting; }
	FORCEINLINE const FMovementToTargetSettings& GetMovementToTargetSettings() const { return movementToTargetSettings; }
	FORCEINLINE const FMovementTeleportSettings& GetMovementTeleportSettings() const { return movementTeleportSettings; }

#if WITH_EDITOR
public:
	FPatternDataTableJSON GetPatternDataTableJSON() const;
	void LoadPatternDataTableJSON(const FPatternDataTableJSON& _json);
	
	virtual void InitializePatternData(FPatternData& _data) const override;
#endif
};