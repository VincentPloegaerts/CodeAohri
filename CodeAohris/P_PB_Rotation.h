#pragma once

#include "CoreMinimal.h"
#include "P_PatternBehavior.h"

#include "STRUCT_RotationSettings.h"

#include "P_PB_Rotation.generated.h"

class UP_DT_PatternDataTable;

UCLASS()
class INSIDEUS_API UP_PB_Rotation : public UP_PatternBehavior
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties, EditCondition = "bIsEnable"))
	FRotationSettings sRotationSettings;

public:
	virtual void ActivateBehavior(UP_DT_BaseDataTable* _dataTable) override;
	virtual void ActivateBehavior(FPatternData& _patternData) override;
	virtual void TickBehavior(const float& _deltaTime) override;

protected:
	virtual void InitializeRotation(UP_DT_PatternDataTable* _dataTable);
	virtual void InitializeRotation(FPatternData& _patternData);
	virtual void RotationBehavior(const float& _deltaTime);
};