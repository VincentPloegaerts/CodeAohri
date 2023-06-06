#pragma once
#include "STRUCT_GPE_ProjectileSettings.generated.h"

USTRUCT()
struct FGPE_ProjectileSettings
{
	GENERATED_BODY()

	/**
	 * If false, the collider will move from ZOffset to Z rootComponent.
	 * Else the collider will move from rootComponent to ZOffset.
	 */
	UPROPERTY(EditAnywhere, Category = "Behavior | GPE_Projectile | Settings")
	bool bMoveUp = false;

	//The collider spawn location or the goal depending of bMoveUp.
	UPROPERTY(EditAnywhere, Category = "Behavior | GPE_Projectile | Settings", meta = (ClampMin = "0"))
	float fZOffset = 1000;

	//The amount of damage dealt by the collider.
	UPROPERTY(EditAnywhere, Category = "Behavior | GPE_Projectile | Settings", meta = (ClampMin = "0"))
	float fDamage = 0;

	//The collider move speed.
	UPROPERTY(EditAnywhere, Category = "Behavior | GPE_Projectile | Settings", meta = (ClampMin = "0"))
	float fMoveSpeed = 100;
	
	//The time of showing the decal to prevent the player to move.
	UPROPERTY(EditAnywhere, Category = "Behavior | GPE_Projectile | Settings", meta = (ClampMin = "0"))
	float fPreviewTime = 2;
};