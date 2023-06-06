#pragma once

#include "CoreMinimal.h"
#include "P_PB_Random.h"

#include "STRUCT_ShootRandom2DSettings.h"

#include "P_PB_Random2D.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_Random2D : public UP_PB_Random
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties, EditCondition = "bIsEnable"))
	FShootRandom2DSettings sRandom2DSettings;

private:
	virtual void InitializeRandom(UP_DT_ShootDataTable* _dataTable) override;
	virtual void InitializeRandom(FPatternData& _patternData) override;

public:
	virtual void Randomize(FVector& _direction) override;
};