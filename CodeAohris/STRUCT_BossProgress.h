#pragma once

#include "STRUCT_BossProgress.generated.h"

USTRUCT(BlueprintType)
struct FBossProgress
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossProgress")
		bool boss1Done = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossProgress")
		bool boss2Done = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossProgress")
		bool boss3Done = false;
};

