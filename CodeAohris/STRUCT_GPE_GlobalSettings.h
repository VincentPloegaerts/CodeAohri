#pragma once
#include "STRUCT_GPE_FloorDangerSettings.h"
#include "STRUCT_GPE_HuntRiftSettings.h"
#include "STRUCT_GPE_ProjectileSettings.h"
#include "STRUCT_ScalingSettings.h"
#include "STRUCT_GPE_GlobalSettings.generated.h"

USTRUCT()
struct FGPE_GlobalSettings
{
	GENERATED_BODY()

	UPROPERTY()
	bool bScaling = false;
	UPROPERTY() 
	FScalingSettings scalingSettings;
	
	UPROPERTY()
	bool bEditProjectileGPE = false;
	UPROPERTY() 
	FGPE_ProjectileSettings projectileSettings;

	UPROPERTY()
	bool bEditFloorGPE = false;
	UPROPERTY() 
	FGPE_FloorDangerSettings floorDangerSettings;

	UPROPERTY()
	bool bEditHuntRiftGPE = false;
	UPROPERTY() 
	FGPE_HuntRiftSettings huntRiftSettings;
};