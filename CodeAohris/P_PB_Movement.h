#pragma once

#include "CoreMinimal.h"
#include "P_PatternBehavior.h"
#include "P_PB_Movement.generated.h"

class UP_DT_PatternDataTable;

UCLASS(Abstract)
class INSIDEUS_API UP_PB_Movement : public UP_PatternBehavior
{
	GENERATED_BODY()

public:
	virtual void ActivateBehavior(UP_DT_BaseDataTable* _dataTable) override;
	virtual void ActivateBehavior(FPatternData& _patternData) override;
	virtual void TickBehavior(const float& _deltaTime) override;
	
protected:
	virtual void InitializeMovement(UP_DT_PatternDataTable* _dataTable);
	virtual void InitializeMovement(FPatternData& _patternData);
	
	virtual void MovementBehavior(const float& _deltaTime);
};