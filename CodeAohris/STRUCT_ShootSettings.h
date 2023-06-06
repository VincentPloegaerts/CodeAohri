#pragma once
#include "ENUM_ShootingMode.h"
#include "P_Projectile.h"

#include "STRUCT_ShootSettings.generated.h"

USTRUCT()
struct FShootSettings
{
	GENERATED_BODY()

	/*How the object will shoot.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings")
	TEnumAsByte<EShootingMode> eShootingMode = EShootingMode::DURATION;

	/*Type of projectile.*/
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Shoot | Settings")
	TEnumAsByte<EProjectileType> projectileType = EProjectileType::Directional;

	/*Amount of projectile spawned during one shot.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (UIMin = "1", UIMax = "500", ClampMin = "1", ClampMax = "500"))
	int iSpawnAtOnce = 1;

	/*Quantity of projectile that will be shot.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (UIMin = "1", UIMax = "200", ClampMin = "1", ClampMax = "200"))
	int iFireQuantity = 10;
	
	/*Time of projectile shot.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (UIMin = "1", UIMax = "120", ClampMin = "1", ClampMax = "120"))
	float fFireDuration = 5.0f;

	/*Rate of projectile shot.*/
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (UIMin = "0.05", UIMax = "10", ClampMin = "0.05", ClampMax = "10"))
	float fFireRate = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings")
	bool bDelayed = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (EditCondition = "bDelayed", EditConditionHides, UIMin = "0.01", UIMax = "3", ClampMin = "0.01", ClampMax = "3"))
	float minDelay = 0.1f;
	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings", meta = (EditCondition = "bDelayed", EditConditionHides, UIMin = "0.05", UIMax = "3", ClampMin = "0.05", ClampMax = "3"))
	float maxDelay = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Behavior | Shoot | Settings")
	FSongFeedBackData songFeedbackData = FSongFeedBackData();
};