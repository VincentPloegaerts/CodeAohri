#include "P_PB_MovementOnAxe.h"

#include "GPE_Laser.h"
#include "P_DT_PatternDataTable.h"

void UP_PB_MovementOnAxe::InitializeMovement(UP_DT_PatternDataTable* _dataTable)
{
	Super::InitializeMovement(_dataTable);
	if (!owner) return;
	
	movementOnAxeSettings = _dataTable->GetMovementOnAxeSettings();

	const EAxisMode& _axis = movementOnAxeSettings.axis;
	const float& _distance = movementOnAxeSettings.fDistance;

	vStartLocation = owner->GetActorLocation();
	
	if (_axis == EAxisMode::UP_AXIS)
	{
		const FVector& _up = owner->GetActorUpVector();

		if (!movementOnAxeSettings.bSpawnIsNotCenter)
			vStartLocation -= _up * (_distance / 2.0f);
			
		vEndLocation = vStartLocation + _up * _distance;
	}
	
	else if (_axis == EAxisMode::RIGHT_AXIS)
	{
		const FVector& _right = owner->GetActorRightVector();
		
		if (!movementOnAxeSettings.bSpawnIsNotCenter)
			vStartLocation -= _right * (_distance / 2.0f);
		
		vEndLocation = vStartLocation + _right * _distance;
	}
	
	else // (_axis == EAxisMode::FORWARD_AXIS)
	{
		const FVector& _fwd = owner->GetActorForwardVector();
		
		if (!movementOnAxeSettings.bSpawnIsNotCenter)
			vStartLocation -= _fwd * (_distance / 2.0f);
		
		vEndLocation = vStartLocation + _fwd * _distance;
	}
		
	vTargetLocation = vEndLocation;
	
	owner->SetActorLocation(vStartLocation);
}

void UP_PB_MovementOnAxe::InitializeMovement(FPatternData& _patternData)
{
	Super::InitializeMovement(_patternData);
	if (!owner) return;
	
	movementOnAxeSettings = _patternData.GetMovementOnAxeSettings();

	const EAxisMode& _axis = movementOnAxeSettings.axis;
	const float& _distance = movementOnAxeSettings.fDistance;

	vStartLocation = owner->GetActorLocation();
	
	if (_axis == EAxisMode::UP_AXIS)
	{
		const FVector& _up = owner->GetActorUpVector();

		if (!movementOnAxeSettings.bSpawnIsNotCenter)
			vStartLocation -= _up * (_distance / 2.0f);
			
		vEndLocation = vStartLocation + _up * _distance;
	}
	
	else if (_axis == EAxisMode::RIGHT_AXIS)
	{
		const FVector& _right = owner->GetActorRightVector();
		
		if (!movementOnAxeSettings.bSpawnIsNotCenter)
			vStartLocation -= _right * (_distance / 2.0f);
		
		vEndLocation = vStartLocation + _right * _distance;
	}
	
	else // (_axis == EAxisMode::FORWARD_AXIS)
	{
		const FVector& _fwd = owner->GetActorForwardVector();
		
		if (!movementOnAxeSettings.bSpawnIsNotCenter)
			vStartLocation -= _fwd * (_distance / 2.0f);
		
		vEndLocation = vStartLocation + _fwd * _distance;
	}
		
	vTargetLocation = vEndLocation;
	
	owner->SetActorLocation(vStartLocation);
}

void UP_PB_MovementOnAxe::MovementBehavior(const float& _deltaTime)
{
	if (!bIsEnable || !owner) return;
	
	const FVector& _location = FMath::VInterpConstantTo(owner->GetActorLocation(), vTargetLocation, _deltaTime, movementOnAxeSettings.fMovementSpeed);

	owner->SetActorLocation(_location);

	if (IsAtLocation(_location))
	{
		++iRepeatTime;
		bMoveToEndLocation = !bMoveToEndLocation;
		vTargetLocation = bMoveToEndLocation ? vEndLocation : vStartLocation;
		if (iRepeatTime >= movementOnAxeSettings.iRepeat)
		{
			bIsEnable = false;
			onBehaviorComplete.Broadcast();
		}
	}
}
