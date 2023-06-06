#pragma once

#include "ENUM_Pivot.h"

#include "STRUCT_LaserSettings.generated.h"

USTRUCT()
struct FLaserSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings")
	bool bDetachLaserFromSocket = false;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings")
	bool bKeepFollowSocketLocation = false;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings")
	TEnumAsByte<EPivot> laserPivot = EPivot::CENTER;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings")
	bool bLookPlayerOnEndFeedback = false;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings")
	bool bLookPitch = false;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings")
	FRotator rLookOffset = FRotator::ZeroRotator;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings", meta = (UIMin = "1", ClampMin = "1"))
	float fLaserDamage = 1;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings", meta = (UIMin = "100", UIMax = "1000000", ClampMin = "100", ClampMax = "10000"))
	float fLaserLength = 500;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings", meta = (UIMin = "1", UIMax = "359", ClampMin = "1", ClampMax = "359"))
	float fLaserRadius = 10;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings", meta = (UIMin = "1", UIMax = "120", ClampMin = "1", ClampMax = "120"))
	float fLaserDuration = 5.0f;
};
