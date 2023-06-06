#pragma once
#include "STRUCT_GPE_HuntRiftSettings.generated.h"

USTRUCT()
struct FGPE_HuntRiftSettings
{
	GENERATED_BODY()

	//The speed
	UPROPERTY(EditAnywhere, Category = "Behavior | GPE_HuntRift | Settings", meta = (ClampMin = "0"))
	float fMoveSpeed = 100;

	//The follow duration
	UPROPERTY(EditAnywhere, Category = "Behavior | GPE_HuntRift | Settings", meta = (ClampMin = "0"))
	float fFollowDuration = 10;

	//The damage
	UPROPERTY(EditAnywhere, Category = "Behavior | GPE_HuntRift | Settings", meta = (ClampMin = "0"))
	float fDamage = 1;
};