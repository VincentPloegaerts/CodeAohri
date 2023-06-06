
#include "Cam_CameraArena.h"

#include "IU_Camera.h"
#include "IU_Macros.h"
#include "UC_PlayerAttackComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void ACam_CameraArena::BeginPlay()
{
	Super::BeginPlay();
}

void ACam_CameraArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
void ACam_CameraArena::MovePitchSuper(const float& _pitchValue,const float& _sens,const bool& _ignoreAcceptance,const bool& _ignoreInvert)
{
	if (FMath::Abs(_pitchValue) < axisAcceptance && !_ignoreAcceptance) //reset camera to correct distance after being on ground event if there's no input
		{
			movingCamera = false;
			return;
		}

	if (!_ignoreAcceptance)
	{
		movingCamera = true;
	}

	float _result = _pitchValue * pitchSpeed * GetWorld()->DeltaTimeSeconds * (invertYAxis && !_ignoreInvert ? -1 : 1) * aimAssistSpeed * _sens *
		(target->GetCharacterAttackComponent()->IsPlayerChargingShoot() ? aimRotateFactor : 1.0f)/* - aimAssistVector.Z*/;
	
	const float& _pitchInRad = FMath::DegreesToRadians(pitch + _result);
	const FVector& _targetLocationWithOffset = TargetLocationWithCameraOffset(positionOffset);
	const float& _newLocationZ = FMath::Sin(_pitchInRad) * currentDistanceFromTarget + _targetLocationWithOffset.Z;
	
	if (_newLocationZ < cameraMinHeight + rampHeight  && _result < 0.0f)
	{
		if (rampHeight > 0.0f)
		{
			rampHeight += _result * rampUpFactor;				
		}
		else
		{
			additionalPitch += _result;
			additionalPitch =  FMath::Clamp<float>(pitch+additionalPitch, minPitch, maxPitch) - pitch;
		}
	}
	else
	{
		if (additionalPitch <0 && _result > 0.0f)
		{
			rampHeight += _result*rampUpFactor;
			if (additionalPitch + _result >0.0f)
			{				
				_result -= additionalPitch;
				additionalPitch = 0.0f;
				pitch += _result * rampUpFactor;
				pitch = FMath::ClampAngle(pitch, minPitch, maxPitch);
			}
			else
			{
				additionalPitch += _result * rampUpFactor / 1.5f;
				additionalPitch = FMath::ClampAngle(additionalPitch + pitch,minPitch,maxPitch) - pitch;			
			}
			//LOG("%f",currentDistanceFromTarget);
		}
		else
		{
			if (rampHeight > 0.0f && _result >0.0f)
			{
				 // if (rampHeight - _result < 0.0f)
				 // {
					// LOG("5");
				 // 	_result += rampHeight;
				 // 	rampHeight = 0.0f;
				 // }
				pitch += _result;
				pitch = FMath::ClampAngle(pitch, minPitch, maxPitch);
			}
			else
			{
				pitch +=  _result;
				pitch = FMath::ClampAngle(pitch, minPitch, maxPitch);
				rampHeight = 0.0f;
			}
		}
	}
	
}

void ACam_CameraArena::UpdateLocation(const float& _deltaTime)
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))return;
	//currentDistanceFromTarget = FMath::Lerp(currentDistanceFromTarget,distanceFromTarget,_deltaTime * testSpeed);
	//LOG("Dist = %f,from %s to %s",currentDistanceFromTarget,*GetActorLocation().ToString(),*TargetLocation().ToString());
	const FVector& _targetLocation = ComputeNewPosition(currentDistanceFromTarget,pitch + additionalPitch,yaw,TargetLocationWithCameraOffset(positionOffset),cameraMinHeight + rampHeight);
	//currentDistanceFromTarget = (GetActorLocation() - _targetLocation).Size();
	SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), _targetLocation, _deltaTime, moveSpeed));

	// Look at to the target
	const FRotator& _newRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(), TargetLocationWithCameraOffset(lookAtOffset + positionOffset));
	SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), _newRotation, _deltaTime,
												   rotateSpeed));
	
	//LOG("%s",*GetActorLocation().ToString());
}