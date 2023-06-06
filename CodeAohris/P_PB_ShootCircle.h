#pragma once

#include "CoreMinimal.h"
#include "P_PB_ShootArc.h"

#include "STRUCT_ShootCircleSettings.h"

#include "P_PB_ShootCircle.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_ShootCircle : public UP_PB_ShootArc
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties, EditCondition = "bIsEnable"))
	FShootCircleSettings sShootCircleSettings;

private:
	virtual void InitializeShoot(UP_DT_ShootDataTable* _shootDataTable) override;
	virtual void InitializeShoot(FPatternData& _patternData) override;

	virtual void Shoot() override;
};