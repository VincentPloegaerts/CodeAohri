#pragma once

#include "CoreMinimal.h"
#include "P_PB_Shoot.h"

#include "STRUCT_ShootPointSettings.h"

#include "P_PB_ShootPoint.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_ShootPoint : public UP_PB_Shoot
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties, EditCondition = "bIsEnable"))
	FShootPointSettings sShootPointSettings;

private:
	virtual void InitializeShoot(UP_DT_ShootDataTable* _shootDataTable) override;
	virtual void InitializeShoot(FPatternData& _patternData) override;

	virtual void Shoot() override;
};