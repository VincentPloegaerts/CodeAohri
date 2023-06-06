#include "P_PB_RotationAngle.h"

#include "B_Boss.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "M_GameMode.h"
#include "P_DT_PatternDataTable.h"
#include "Kismet/KismetMathLibrary.h"

void UP_PB_RotationAngle::InitializeRotation(UP_DT_PatternDataTable* _dataTable)
{
	Super::InitializeRotation(_dataTable);
	if (!_dataTable) return;
	sAngleSettings = _dataTable->GetAngleSettings();

	InitializeAngleRotation();
}

void UP_PB_RotationAngle::InitializeRotation(FPatternData& _patternData)
{
	Super::InitializeRotation(_patternData);
	sAngleSettings = _patternData.GetAngleSettings();

	InitializeAngleRotation();
}

void UP_PB_RotationAngle::RotationBehavior(const float& _deltaTime)
{
	Super::RotationBehavior(_deltaTime);
	if (!bIsEnable || !owner) return;

	fCurrentRotation += _deltaTime * sRotationSettings.fRotationSpeed;
	const FRotator& _rotation = rRotation * sRotationSettings.fRotationSpeed * (sAngleSettings.bReverse ? -1 : 1) * _deltaTime;
	owner->AddActorLocalRotation(_rotation);

	CheckAngleRotationProgress();
}

void UP_PB_RotationAngle::InitializeAngleRotation()
{
	if (!bIsEnable || !owner) return;

	const int& _reverse = sAngleSettings.bReverse ? 1 : -1;
	const float& _angleRange = sAngleSettings.fAngle / 2.0f;
	
	const float& _angle = _angleRange * _reverse;
	
	const EAxisMode& _axisEnum = sAngleSettings.eAxis;
	rRotation = FRotator(_axisEnum == EAxisMode::RIGHT_AXIS,
						_axisEnum == EAxisMode::UP_AXIS,
						_axisEnum == EAxisMode::FORWARD_AXIS);
	
	const FRotator& _rotation = rRotation * _angle;
	
	owner->AddActorLocalRotation(_rotation);
}

void UP_PB_RotationAngle::CheckAngleRotationProgress()
{
	if (fCurrentRotation > sAngleSettings.fAngle)
	{
		sAngleSettings.bReverse = !sAngleSettings.bReverse;
		fCurrentRotation = 0;
		iCurrent++;
		if (iCurrent >= sAngleSettings.iCount)
		{
			bIsEnable = false;
			onBehaviorComplete.Broadcast();
		}
	}
}