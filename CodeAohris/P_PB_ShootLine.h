#pragma once

#include "CoreMinimal.h"
#include "P_PB_Shoot.h"

#include "STRUCT_ShootLineSettings.h"

#include "P_PB_ShootLine.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_ShootLine : public UP_PB_Shoot
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties, EditCondition = "bIsEnable"))
	FShootLineSettings sShootLineSettings;
	
private:
	virtual void InitializeShoot(UP_DT_ShootDataTable* _shootDataTable) override;
	virtual void InitializeShoot(FPatternData& _patternData) override;

	virtual void Shoot() override;
};