#pragma once
#include "ENUM_ProjectileType.generated.h"

UENUM()
enum EProjectileType
{
	Directional,
	Guided,
	Simulated,
	Ballistic,
	Sinusoidal,
	FatGuided UMETA(DisplayName = "Fat Guided pour le Loriel")
};