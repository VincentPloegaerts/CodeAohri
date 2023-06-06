
#pragma once

#include "CoreMinimal.h"
#include "P_PB_ShootArc.h"

#include "STRUCT_ShootCrossSettings.h"

#include "P_PB_ShootCross.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_ShootCross : public UP_PB_ShootArc
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties, EditCondition = "bIsEnable"))
	FShootCrossSettings sShootCrossSettings;

private:
	virtual void InitializeShoot(UP_DT_ShootDataTable* _shootDataTable) override;
	virtual void InitializeShoot(FPatternData& _patternData) override;

	virtual void Shoot() override;
};