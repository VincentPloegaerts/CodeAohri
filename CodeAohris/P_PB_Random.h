#pragma once

#include "CoreMinimal.h"
#include "P_PatternBehavior.h"
#include "P_PB_Random.generated.h"

class UP_DT_ShootDataTable;

UCLASS(Abstract)
class INSIDEUS_API UP_PB_Random : public UP_PatternBehavior
{
	GENERATED_BODY()

protected:
	//Initialize Child class with casted DataTable
	virtual void InitializeRandom(UP_DT_ShootDataTable* _dataTable);
	virtual void InitializeRandom(FPatternData& _patternData);

public:
	virtual void ActivateBehavior(UP_DT_BaseDataTable* _dataTable) override;
	virtual void ActivateBehavior(FPatternData& _patternData) override;

	//Will randomize the given direction in 2D or 3D 
	virtual void Randomize(FVector& _direction);
};