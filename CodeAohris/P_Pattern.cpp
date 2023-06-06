#include "P_Pattern.h"

#include "IU_Macros.h"

#include "P_PB_RotationRotator.h"
#include "P_PB_RotationInstant.h"
#include "P_PB_RotationAngle.h"
#include "P_PB_RotationTarget.h"

#include "P_PB_MovementOnAxe.h"
#include "P_PB_MovementTeleport.h"
#include "P_PB_MovementToTarget.h"

#include "P_DT_PatternDataTable.h"

AP_Pattern::AP_Pattern()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	//Rotation
	rotatorBehavior = CreateDefaultSubobject<UP_PB_RotationRotator>("Rotator");
	AddOwnedComponent(rotatorBehavior);
	
	instantBehavior = CreateDefaultSubobject<UP_PB_RotationInstant>("InstantRotation");
	AddOwnedComponent(instantBehavior);
	
	angleBehavior = CreateDefaultSubobject<UP_PB_RotationAngle>("AngleRotation");
	AddOwnedComponent(angleBehavior);
	
	targetBehavior = CreateDefaultSubobject<UP_PB_RotationTarget>("TargetRotation");
	AddOwnedComponent(targetBehavior);

	//Movement
	movementOnAxe = CreateDefaultSubobject<UP_PB_MovementOnAxe>("MovementOnAxe");
	AddOwnedComponent(movementOnAxe);
	
	movementToTarget = CreateDefaultSubobject<UP_PB_MovementToTarget>("MovementToTarget");
	AddOwnedComponent(movementToTarget);
	
	movementTeleport = CreateDefaultSubobject<UP_PB_MovementTeleport>("MovementTeleport");
	AddOwnedComponent(movementTeleport);
}

void AP_Pattern::InitializePattern(UP_DT_BaseDataTable* _dataTable)
{
	Super::InitializePattern(_dataTable);
	
	UP_DT_PatternDataTable* _patternDataTable = Cast<UP_DT_PatternDataTable>(_dataTable);
	if (!_patternDataTable) return;
	
	AM_PatternsTickManager::FOnTick& _onTick = patternsTickManager->GetOnTick();
	
	//Activate Behavior Component depends on booleans define in DataTable
	//Rotation
	InitializeBehavior(_patternDataTable->GetInstantState(), instantBehavior, _patternDataTable, _onTick);
	InitializeBehavior(_patternDataTable->GetAngleState(), angleBehavior, _patternDataTable, _onTick);
	InitializeBehavior(_patternDataTable->GetRotatorState(), rotatorBehavior, _patternDataTable, _onTick);
	InitializeBehavior(_patternDataTable->GetTargetState(), targetBehavior, _patternDataTable, _onTick);
	
	//Movement
	InitializeBehavior(_patternDataTable->GetMovementOnAxeState(), movementOnAxe, _patternDataTable, _onTick);
	InitializeBehavior(_patternDataTable->GetMovementToTargetState(), movementToTarget, _patternDataTable, _onTick);
	InitializeBehavior(_patternDataTable->GetMovementTeleportState(), movementTeleport, _patternDataTable, _onTick);
}

void AP_Pattern::InitializePatternData(FPatternData& _patternData)
{
	Super::InitializePatternData(_patternData);
	
	AM_PatternsTickManager::FOnTick& _onTick = patternsTickManager->GetOnTick();
	
	InitializeBehavior(_patternData.GetInstantState(), instantBehavior, _patternData, _onTick);
	InitializeBehavior(_patternData.GetAngleState(), angleBehavior, _patternData, _onTick);
	InitializeBehavior(_patternData.GetRotatorState(), rotatorBehavior, _patternData, _onTick);
	InitializeBehavior(_patternData.GetTargetState(), targetBehavior, _patternData, _onTick);
	
	InitializeBehavior(_patternData.GetMovementOnAxeState(), movementOnAxe, _patternData, _onTick);
	InitializeBehavior(_patternData.GetMovementToTargetState(), movementToTarget, _patternData, _onTick);
	InitializeBehavior(_patternData.GetMovementTeleportState(), movementTeleport, _patternData, _onTick);
}

void AP_Pattern::DeActivatePatternBehavior()
{
	AM_PatternsTickManager::FOnTick& _onTick = patternsTickManager->GetOnTick();

	DeActivateBehavior(instantBehavior, _onTick);
	DeActivateBehavior(angleBehavior, _onTick);
	DeActivateBehavior(rotatorBehavior, _onTick);
	DeActivateBehavior(targetBehavior, _onTick);
	
	DeActivateBehavior(movementOnAxe, _onTick);
	DeActivateBehavior(movementToTarget, _onTick);
	DeActivateBehavior(movementToTarget, _onTick);
}
