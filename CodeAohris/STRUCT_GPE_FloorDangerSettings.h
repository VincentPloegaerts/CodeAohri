#pragma once
#include "STRUCT_GPE_FloorDangerSettings.generated.h"

USTRUCT()
struct FGPE_FloorDangerSettings
{
	GENERATED_BODY()

	//The time to prevent the player to move away.
	UPROPERTY(EditAnywhere, Category = "Behavior | GPE_FloorDanger | Settings", meta = (ClampMin = "0"))
	float fPreviewTime = 2;

	//The time after complete his behavior.
	UPROPERTY(EditAnywhere, Category = "Behavior | GPE_FloorDanger | Settings", meta = (ClampMin = "0"))
	float fPersistantTime = 2;

	//Amount of damage dealt during.
	UPROPERTY(EditAnywhere, Category = "Behavior | GPE_FloorDanger | Settings", meta = (ClampMin = "0"))
	float fDamage = 1;
};