#pragma once

UENUM(BlueprintType)
enum EDifficultyType
{
	DIFFICULTY_NONE UMETA(DisplayName = "None"),
	DIFFICULTY_INITIATED UMETA(DisplayName = "Initiated"),
	DIFFICULTY_BORN UMETA(DisplayName = "Born"),
	DIFFICULTY_ELITE UMETA(DisplayName = "Elite")
};