#pragma once

#include "CoreMinimal.h"
#include "P_PB_Shoot.h"

#include "STRUCT_ShootArcSettings.h"

#include "P_PB_ShootArc.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_ShootArc : public UP_PB_Shoot
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties, EditCondition = "bIsEnable"))
	FShootArcSettings sShootArcSettings;

protected:
	virtual void InitializeShoot(UP_DT_ShootDataTable* _shootDataTable) override;
	virtual void InitializeShoot(FPatternData& _patternData) override;
	
	virtual void Shoot() override;

	void ShootArcAngle(const float& _angleRange, const float& _radius, const int& _max, const FVector& _axe, const bool& _keepPatternForward = false,  const bool& _moveBackPoints = false) const;
};