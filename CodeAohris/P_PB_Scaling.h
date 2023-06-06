#pragma once

#include "CoreMinimal.h"
#include "P_PatternBehavior.h"
#include "P_PB_Scaling.generated.h"

UCLASS(Blueprintable)
class INSIDEUS_API UP_PB_Scaling : public UP_PatternBehavior
{
	GENERATED_BODY()	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBehaviorTick, const float&, _onTick);

	//When the pattern finish his persistant time, it allow to end the pattern.
	UPROPERTY(VisibleAnywhere, Category = "Scaling")
	bool bEndPatternOnCompleteScale = false;

	//Only used for time spend.
	UPROPERTY(VisibleAnywhere, Category = "Scaling")
	float fCurrentScaleTime = 0.0f;

	
	UPROPERTY(VisibleAnywhere, Category = "Scaling")
	float fCurrentTimeBeforeActive = 0.0f;
	
	//Current owner scale.
	UPROPERTY(VisibleAnywhere, Category = "Scaling")
	FVector vCurrentScale = FVector::ZeroVector;

#pragma region Optimized Struct Values
	
	UPROPERTY()
	float fTimeBeforeActive = 0.0f;
	
	//The time to call the EndBehavior after stop moving.
	UPROPERTY()
	float fPersistantTime = 0.0f;

	//The maximum of time spend in "TimeScale" behavior.
	UPROPERTY()
	float fMaxScaleTime = 0.0f;

	//The speed.
	UPROPERTY()
	float fScaleSpeed = 0.0f;

	//The Default scale * the speed. 
	UPROPERTY()
	FVector vOptimizedScale = FVector::ZeroVector;

	//The goal scale used only in "GoalScale" behavior.
	UPROPERTY()
	FVector vGoalScale = FVector::ZeroVector;
#pragma endregion Optimized Struct Values
	
	UPROPERTY()
	FTimerHandle timer;
	
	UPROPERTY()
	FOnBehaviorTick onBehaviorTick;
	
public:
	virtual void ActivateBehavior(UP_DT_BaseDataTable* _dataTable) override;
	void ActivateBehaviorScaling(FScalingSettings& _patternData);
	virtual void TickBehavior(const float& _deltaTime) override;

private:
	//Scale indefinitely.
	UFUNCTION() void DefaultScale(const float& _deltaTime);

	//Scale until Max time reached.
	UFUNCTION() void TimeScale(const float& _deltaTime);
	
	//Scale until reach his goal.
	UFUNCTION() void GoalScale(const float& _deltaTime);

	//Deactivate scaling behavior and call EndBehavior.
	UFUNCTION() void DeactivateScaling();
	UFUNCTION() void EndBehavior();
};