#pragma once

#include "CoreMinimal.h"
#include "P_PatternBase.h"

class UP_PatternBehavior;

class UP_PB_RotationRotator;
class UP_PB_RotationInstant;
class UP_PB_RotationAngle;
class UP_PB_RotationTarget;

class UP_PB_Laser;
class UP_PB_MovementOnAxe;
class UP_PB_MovementToTarget;
class UP_PB_MovementTeleport;

class UP_DT_PatternDataTable;

#include "P_Pattern.generated.h"

UCLASS(Abstract)
class INSIDEUS_API AP_Pattern : public AP_PatternBase
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComplete, AP_Pattern*, _pattern);
	
protected:
	UPROPERTY(VisibleAnywhere)
	UP_PB_RotationRotator* rotatorBehavior = nullptr;
	UPROPERTY(VisibleAnywhere)
	UP_PB_RotationInstant* instantBehavior = nullptr;
	UPROPERTY(VisibleAnywhere)
	UP_PB_RotationAngle* angleBehavior = nullptr;	
	UPROPERTY(VisibleAnywhere)
	UP_PB_RotationTarget* targetBehavior = nullptr;	

	UPROPERTY(VisibleAnywhere)
	UP_PB_MovementOnAxe* movementOnAxe = nullptr;
	UPROPERTY(VisibleAnywhere)
	UP_PB_MovementToTarget* movementToTarget = nullptr;
	UPROPERTY(VisibleAnywhere)
	UP_PB_MovementTeleport* movementTeleport = nullptr;

public:
	FORCEINLINE UP_PB_RotationRotator* GetRotatorBehavior() const { return  rotatorBehavior; }
	FORCEINLINE UP_PB_RotationInstant* GetInstantRotationBehavior() const { return  instantBehavior; }
	FORCEINLINE UP_PB_RotationAngle* GetAngleRotationBehavior() const { return  angleBehavior; }
	FORCEINLINE UP_PB_RotationTarget* GetTargetRotationBehavior() const { return  targetBehavior; }
	
	FORCEINLINE UP_PB_MovementOnAxe* GetMovementOnAxeBehavior() const { return  movementOnAxe; }
	FORCEINLINE UP_PB_MovementToTarget* GetMovementToTargetBehavior() const { return  movementToTarget; }
	FORCEINLINE UP_PB_MovementTeleport* GetMovementTeleportBehavior() const { return  movementTeleport; }
	
public:	
	AP_Pattern();
	
public:
	//Called by TaskNode from boss BehaviorTree
	virtual void InitializePattern(UP_DT_BaseDataTable* _dataTable) override;
	virtual void InitializePatternData(FPatternData& _patternData) override;
	
protected:	
	//Used to deactivate every pattern behavior
	virtual void DeActivatePatternBehavior() override;
};