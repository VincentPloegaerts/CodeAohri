#pragma once
#include "STRUCT_ScalingSettings.generated.h"

USTRUCT()
struct FScalingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Behavior | Scale | Properties", meta = (ClampMin = "0"))
	float fTimeBeforeActive = 0.0f;
	
	//Use scale time
	UPROPERTY(EditAnywhere, Category = "Behavior | Scale | Properties", meta = (EditCondition = "!bUseGoal", EditConditionHides))
	bool bUseTime = false;

	//The time to scale or if bUseSmooth is active, the time to reach vGoalScale.
	UPROPERTY(EditAnywhere, Category = "Behavior | Scale | Properties", meta = (ClampMin = "0", EditCondition = "bUseTime", EditConditionHides))
	float fScaleTime = 5;

	
	//Use lerp to vGoalScale
	UPROPERTY(EditAnywhere, Category = "Behavior | Scale | Properties", meta = (EditCondition = "!bUseTime", EditConditionHides))
	bool bUseGoal = false;
	
	//Scale goal, stop scaling when reached.
	UPROPERTY(EditAnywhere, Category = "Behavior | Scale | Properties", meta = (EditCondition = "bUseGoal", EditConditionHides))
	FVector vGoalScale = FVector(2);

	
	//Scale Axis.
	UPROPERTY(EditAnywhere, Category = "Behavior | Scale | General")
	FVector vDefaultScale = FVector(1);

	//Scale speed.
	UPROPERTY(EditAnywhere, Category = "Behavior | Scale | General", meta = (ClampMin = "0"))
	float fScaleSpeed = 1;
	
	//Time after finishing scale and complete Behavior.
	UPROPERTY(EditAnywhere, Category = "Behavior | Scale | General", meta = (ClampMin = "0"))
	float fPersistantTime = 5;

	//When this parameter is enable, it will call BehaviorComplete event
	UPROPERTY(EditAnywhere, Category = "Behavior | Scale | General")
	bool bEndPatternOnCompleteScale = false;
};