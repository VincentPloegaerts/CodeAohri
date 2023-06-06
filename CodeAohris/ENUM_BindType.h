#pragma once

UENUM(BlueprintType)
enum EBindType
{
	BIND_NONE UMETA(DisplayName = "None"),
	BIND_JUMP UMETA(DisplayName = "Jump"),
	BIND_DASH UMETA(DisplayName = "Dash"),
	BIND_PRIMARY_SHOOT UMETA(DisplayName = "Primary shoot"),
	BIND_SECONDARY_SHOOT UMETA(DisplayName = "Secondary shoot")
};