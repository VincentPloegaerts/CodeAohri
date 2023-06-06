#pragma once

UENUM(BlueprintType)
enum EHitFeedbackEnum
{
    HIT_BOSS UMETA(DisplayName = "Boss"),
    HIT_GPE UMETA(DisplayName = "Gpe"),
    HIT_PLAYER UMETA(DisplayName = "Player")
};