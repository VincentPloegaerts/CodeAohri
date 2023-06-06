#pragma once

#include "CoreMinimal.h"
#include "P_PB_Movement.h"

#include "STRUCT_MovementToTargetSettings.h"

#include "P_PB_MovementToTarget.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_MovementToTarget : public UP_PB_Movement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FMovementToTargetSettings movementToTargetSettings;

	UPROPERTY(VisibleAnywhere, Category = "Behavior | Movement | Settings")
	float fRange = 1;
	
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Movement | Values")
	bool bMoveToEndLocation = true;
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Movement | Values")
	FVector vStartLocation = FVector(0);
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Movement | Values")
	FVector vEndLocation = FVector(0);
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Movement | Values")
	FVector vTargetLocation = FVector(0);
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Movement | Values")
	unsigned int iRepeatTime = 0;

public:
	FORCEINLINE bool IsAtLocation(const FVector& _location) const {
		return FVector::Dist(_location, vTargetLocation) < fRange;
	}
	
private:
	virtual void InitializeMovement(UP_DT_PatternDataTable* _dataTable) override;
	virtual void InitializeMovement(FPatternData& _patternData) override;

	virtual void MovementBehavior(const float& _deltaTime) override;

	const FVector GetPlayerLocation() const;
	const FVector GetBossLocation() const;
	const FVector GetArenaPointLocation(const FString& _pointName) const;
};