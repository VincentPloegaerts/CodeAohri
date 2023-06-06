#pragma once

#include "CoreMinimal.h"
#include "P_PB_Rotation.h"

#include "STRUCT_AngleSettings.h"

#include "P_PB_RotationAngle.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_RotationAngle : public UP_PB_Rotation
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties, EditCondition = "bIsEnable"))
	FAngleSettings sAngleSettings;

	UPROPERTY(VisibleAnywhere, Category = "Behavior | Rotation | Values")
	FName socketParentName = "";
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Rotation | Values")
	USkeletalMeshComponent* bossSkeletal = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Rotation | Values")
	FRotator rRotation = FRotator(0);
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Rotation | Values")
	float fCurrentRotation = 0;
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Rotation | Values")
	unsigned int iCurrent = 0;

public:
	virtual void InitializeRotation(UP_DT_PatternDataTable* _dataTable) override;
	virtual void InitializeRotation(FPatternData& _patternData) override;

private:
	virtual void RotationBehavior(const float& _deltaTime) override;

	//Initialize variable data for rotation and initial rotation of the Owner
	void InitializeAngleRotation();
	//Check the Rotation progress, if the rotation need to be reversed or stop (disable component)
	void CheckAngleRotationProgress();
};