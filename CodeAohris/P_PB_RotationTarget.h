#pragma once

#include "CoreMinimal.h"
#include "P_PB_Rotation.h"

#include "STRUCT_TargetSettings.h"

#include "P_PB_RotationTarget.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_RotationTarget : public UP_PB_Rotation
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties, EditCondition = "bIsEnable"))
	FTargetSettings targetSettings;
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Rotation | Values")
	AActor* target = nullptr;

protected:
	virtual void InitializeRotation(UP_DT_PatternDataTable* _dataTable) override;
	virtual void InitializeRotation(FPatternData& _patternData) override;

private:
	virtual void RotationBehavior(const float& _deltaTime) override;
};