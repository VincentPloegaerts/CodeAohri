#pragma once

#include "CoreMinimal.h"
#include "P_PB_Rotation.h"

#include "STRUCT_InstantSettings.h"

#include "P_PB_RotationInstant.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_RotationInstant : public UP_PB_Rotation
{
	GENERATED_BODY()

	
private:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties, EditCondition = "bIsEnable"))
	FInstantSettings sInstantSettings;

	UPROPERTY(VisibleAnywhere, Category = "Behavior | Rotation | Values")
	FRotator rRotation = FRotator(0);
	
private:
	virtual void InitializeRotation(UP_DT_PatternDataTable* _dataTable) override;
	virtual void InitializeRotation(FPatternData& _patternData) override;
	
	virtual void RotationBehavior(const float& _deltaTime) override;

	void InitializeInstantRotation();
};