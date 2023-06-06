#pragma once
#include "STRUCT_BossPhase.generated.h"

class AA_Arena;
class AB_Boss;

USTRUCT()
struct FBossPhase
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere)
	AA_Arena* arena = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	AB_Boss* boss = nullptr;
};