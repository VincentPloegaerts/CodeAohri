#pragma once

#include "CoreMinimal.h"
#include "P_PB_Rotation.h"

#include "STRUCT_RotatorSettings.h"

#include "P_PB_RotationRotator.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_RotationRotator : public UP_PB_Rotation
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties, EditCondition = "bIsEnable"))
	FRotatorSettings rotatorSettings;

	UPROPERTY()
	FRotator rotation = FRotator(0);
	
public:
	virtual void InitializeRotation(UP_DT_PatternDataTable* _dataTable) override;
	virtual void InitializeRotation(FPatternData& _patternData) override;
private:
	virtual void RotationBehavior(const float& _deltaTime) override;
};