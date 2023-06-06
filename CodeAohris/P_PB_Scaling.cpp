#include "P_PB_Scaling.h"

#include "STRUCT_ScalingSettings.h"
#include "IU_Macros.h"
#include "P_DT_ZoneDataTable.h"
#include "Kismet/KismetMathLibrary.h"

void UP_PB_Scaling::ActivateBehavior(UP_DT_BaseDataTable* _dataTable)
{
	Super::ActivateBehavior(_dataTable);

	if (const UP_DT_ZoneDataTable* _zoneDataTable = Cast<UP_DT_ZoneDataTable>(_dataTable))
	{
		const FScalingSettings& scalingSettings = _zoneDataTable->GetScalingSettings();
		
		bEndPatternOnCompleteScale = scalingSettings.bEndPatternOnCompleteScale;
		
		fPersistantTime = scalingSettings.fPersistantTime;
		fScaleSpeed = scalingSettings.fScaleSpeed;
		
		vOptimizedScale = scalingSettings.vDefaultScale * fScaleSpeed;
		vCurrentScale = owner->GetActorScale();

		const bool& _useScaleTime = scalingSettings.bUseTime;
		
		if (_useScaleTime || scalingSettings.bUseGoal)
		{
			if (_useScaleTime)
			{
				fMaxScaleTime = scalingSettings.fScaleTime;
				onBehaviorTick.AddDynamic(this, &UP_PB_Scaling::TimeScale);
			}
			else
			{
				vGoalScale = scalingSettings.vGoalScale;
				onBehaviorTick.AddDynamic(this, &UP_PB_Scaling::GoalScale);
			}
		}
		else
			onBehaviorTick.AddDynamic(this, &UP_PB_Scaling::DefaultScale);
	}
	else
	{
		LOG("UP_PB_Scaling::ActivateBehavior -> Failed to cast UP_DT_PatternDataTable to UP_DT_ZoneDataTable");
	}
}

void UP_PB_Scaling::ActivateBehaviorScaling(FScalingSettings& _patternData)
{
	const FScalingSettings& _scalingSettings = _patternData;
		
	bEndPatternOnCompleteScale = _scalingSettings.bEndPatternOnCompleteScale;
		
	fPersistantTime = _scalingSettings.fPersistantTime;
	fScaleSpeed = _scalingSettings.fScaleSpeed;
	fTimeBeforeActive = _scalingSettings.fTimeBeforeActive;
	
	vOptimizedScale = _scalingSettings.vDefaultScale * fScaleSpeed;
	vCurrentScale = owner->GetActorScale();

	const bool& _useScaleTime = _scalingSettings.bUseTime;
		
	if (_useScaleTime || _scalingSettings.bUseGoal)
	{
		if (_useScaleTime)
		{
			fMaxScaleTime = _scalingSettings.fScaleTime;
			onBehaviorTick.AddDynamic(this, &UP_PB_Scaling::TimeScale);
		}
		else
		{
			vGoalScale = _scalingSettings.vGoalScale;
			onBehaviorTick.AddDynamic(this, &UP_PB_Scaling::GoalScale);
		}
	}
	else
		onBehaviorTick.AddDynamic(this, &UP_PB_Scaling::DefaultScale);
}

void UP_PB_Scaling::TickBehavior(const float& _deltaTime)
{
	fCurrentTimeBeforeActive += _deltaTime;
	if (fCurrentTimeBeforeActive >= fTimeBeforeActive)
		onBehaviorTick.Broadcast(_deltaTime);
}

void UP_PB_Scaling::DefaultScale(const float& _deltaTime)
{
	vCurrentScale += vOptimizedScale * _deltaTime;
	owner->SetActorScale3D(vCurrentScale);
}

void UP_PB_Scaling::TimeScale(const float& _deltaTime)
{
	vCurrentScale += vOptimizedScale * _deltaTime;
	owner->SetActorScale3D(vCurrentScale);

	fCurrentScaleTime += _deltaTime;
	if (fCurrentScaleTime > fMaxScaleTime)
	{
		DeactivateScaling();
	}
}

void UP_PB_Scaling::GoalScale(const float& _deltaTime)
{
	vCurrentScale = Get_VLerpConst(vCurrentScale, vGoalScale, _deltaTime, fScaleSpeed);
	owner->SetActorScale3D(vCurrentScale);
	
	if (FVector::Dist(vCurrentScale, vGoalScale) < 0.01f)
	{
		DeactivateScaling();
	}
}

void UP_PB_Scaling::DeactivateScaling()
{
	onBehaviorTick.Clear();
	
	const float& _time = fPersistantTime;
	if (_time > 0 && bEndPatternOnCompleteScale)
		GetWorld()->GetTimerManager().SetTimer(timer, this, &UP_PB_Scaling::EndBehavior, _time);
	else
		EndBehavior();
}

void UP_PB_Scaling::EndBehavior()
{
	if (bEndPatternOnCompleteScale)
		onBehaviorComplete.Broadcast();
}