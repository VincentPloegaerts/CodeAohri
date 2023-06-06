#include "P_PB_RotationTarget.h"

#include "IU_Macros.h"
#include "M_GameMode.h"
#include "IU_Player.h"
#include "P_DT_PatternDataTable.h"
#include "STRUCT_CustomMath.h"

#include "Kismet/KismetMathLibrary.h"

void UP_PB_RotationTarget::InitializeRotation(UP_DT_PatternDataTable* _dataTable)
{
	Super::InitializeRotation(_dataTable);
	if (!_dataTable) return;
	targetSettings = _dataTable->GetTargetSettings();

	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		target = _gameMode->GetPlayer();
}

void UP_PB_RotationTarget::InitializeRotation(FPatternData& _patternData)
{
	Super::InitializeRotation(_patternData);
	targetSettings = _patternData.GetTargetSettings();

	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		target = _gameMode->GetPlayer();
}

void UP_PB_RotationTarget::RotationBehavior(const float& _deltaTime)
{
	Super::RotationBehavior(_deltaTime);
	if (!bIsEnable && !owner && !target) return;

	const FRotator& _ownerRotation = owner->GetActorRotation();

	const FVector& _lookDirection = target->GetActorLocation() - owner->GetActorLocation();
	FRotator _rotation (Get_LookRotation(_lookDirection));
	
	if (targetSettings.bLerp)
	{
		float _speed = sRotationSettings.fRotationSpeed;
		
		if (targetSettings.bSpeedUpWhenTargetFar)
		{
			const float& _maxAngle = targetSettings.fRangeAngleMaxSpeed;
			const float& _minAngle = targetSettings.fRangeAngleMinSpeed;
			const float& _angleBetween = To_Deg(FCustomMath::AngleBetweenVectors(_lookDirection, owner->GetActorForwardVector()));
			if (_maxAngle < _angleBetween)
			{
				_speed += targetSettings.fBonusSpeed;
			}
			else if (_minAngle < _angleBetween)
			{
				_speed += targetSettings.fBonusSpeed * (_angleBetween - _minAngle) / (_maxAngle - _minAngle);
			}
		}

		_rotation = Get_RLerpConst(_ownerRotation, _rotation, _deltaTime, _speed);
	}
	
	_rotation.Roll = _ownerRotation.Roll;
	
	if (!targetSettings.bLookPitchPlayer)
		_rotation.Pitch = _ownerRotation.Pitch;
	
	owner->SetActorRotation(_rotation);
}