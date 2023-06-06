#pragma once

UENUM(BlueprintType)
enum ELevelType
{
	NONE_LEVEL UMETA(DisplayName = "None"),
	UI_LEVEL UMETA(DisplayName = "UI"),
	GAME_LEVEL UMETA(DisplayName = "Game"),
	RUSH_LEVEL UMETA(DisplayName = "Rush")
};