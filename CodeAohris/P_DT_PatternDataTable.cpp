#include "P_DT_PatternDataTable.h"

#include "STRUCT_PatternData.h"

#if WITH_EDITOR
FPatternDataTableJSON UP_DT_PatternDataTable::GetPatternDataTableJSON() const
{
	FPatternDataTableJSON _json;
	_json.rotationSettings = rotationSettings;
	_json.bRotator = bRotator;
	_json.rotatorSettings = rotatorSettings;
	_json.bInstant = bInstant;
	_json.instantSettings = instantSettings;
	_json.bAngle = bAngle;
	_json.angleSettings = angleSettings;
	_json.bTarget = bTarget;
	_json.targetSettings = targetSettings;
	_json.bEditMovementOnAxe = bEditMovementOnAxe;
	_json.movementOnAxeSetting = movementOnAxeSetting;
	_json.bEditMovementToTarget = bEditMovementToTarget;
	_json.movementToTargetSettings = movementToTargetSettings;
	_json.bEditMovementTeleport = bEditMovementTeleport;
	_json.movementTeleportSettings = movementTeleportSettings;
	return _json;
}

void UP_DT_PatternDataTable::LoadPatternDataTableJSON(const FPatternDataTableJSON& _json)
{
	rotationSettings = _json.rotationSettings;
	bRotator = _json.bRotator;
	rotatorSettings = _json.rotatorSettings;
	bInstant = _json.bInstant;
	instantSettings = _json.instantSettings;
	bAngle = _json.bAngle;
	angleSettings = _json.angleSettings;
	bTarget = _json.bTarget;
	targetSettings= _json.targetSettings;
	bEditMovementOnAxe = _json.bEditMovementOnAxe;
	movementOnAxeSetting = _json.movementOnAxeSetting;
	bEditMovementToTarget = _json.bEditMovementToTarget;
	movementToTargetSettings = _json.movementToTargetSettings;
	bEditMovementTeleport = _json.bEditMovementTeleport;
	movementTeleportSettings = _json.movementTeleportSettings;
}

void UP_DT_PatternDataTable::InitializePatternData(FPatternData& _data) const
{
	Super::InitializePatternData(_data);
	_data.SetRotationSettings(rotationSettings);

	_data.SetRotatorState(bRotator);
	_data.SetInstantState(bInstant);
	_data.SetAngleState(bAngle);
	_data.SetTargetState(bTarget);
	
	_data.SetRotatorSettings(rotatorSettings);
	_data.SetInstantSettings(instantSettings);
	_data.SetAngleSettings(angleSettings);
	_data.SetTargetSettings(targetSettings);

	_data.SetMovementOnAxeState(bEditMovementOnAxe);
	_data.SetMovementToTargetState(bEditMovementToTarget);
	_data.SetMovementTeleportState(bEditMovementTeleport);
	
	_data.SetMovementOnAxeSettings(movementOnAxeSetting);
	_data.SetMovementToTargetSettings(movementToTargetSettings);
	_data.SetMovementTeleportSettings(movementTeleportSettings);
}
#endif