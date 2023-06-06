#pragma once

#include "CoreMinimal.h"
#include "P_PB_Movement.h"

#include "STRUCT_MovementOnAxeSettings.h"

#include "P_PB_MovementOnAxe.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_MovementOnAxe : public UP_PB_Movement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FMovementOnAxeSettings movementOnAxeSettings;

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
};