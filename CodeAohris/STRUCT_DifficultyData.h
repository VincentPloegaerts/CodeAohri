#pragma once
#include "ENUM_BossPhase.h"
#include "STRUCT_DifficultyData.generated.h"

USTRUCT(BlueprintType)
struct FDifficultyData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Difficulty Datas | Health")
		bool bCanRegen = false;

	UPROPERTY(EditAnywhere, Category = "Difficulty Datas | Health", meta = (EditCondition = "bCanRegen", EditConditionHides))
		float fRegenCooldown = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "Difficulty Datas | Health", meta = (EditCondition = "bCanRegen", EditConditionHides))
		float fRegenValue = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Difficulty Datas | Health")
		float fMaxLifeToRemove = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "Difficulty Datas | Attack")
		int iOrbConsumedBySlowMotion = 0;

	UPROPERTY(EditAnywhere, Category = "Difficulty Datas | Fall", meta = (ClampMin = "0"))
		float fFallDamage = 15;
	
	UPROPERTY(EditAnywhere, Category = "Difficulty Datas | Damages", meta = (ClampMin = "-100", ClampMax = "100"))
		float fProjectileExtraDamage = 0;
	UPROPERTY(EditAnywhere, Category = "Difficulty Datas | Damages", meta = (ClampMin = "-100", ClampMax = "100"))
		float fZoneExtraDamage = 0;
	
	UPROPERTY(EditAnywhere, Category = "Difficulty Datas | Boss")
		TMap<TEnumAsByte<EBossPhase>, float> bossesLife =
		{
			{EBossPhase::UN, 35000},
			{EBossPhase::DEUX, 35000},
			{EBossPhase::TROIS, 60000},
		};
	
	FDifficultyData() {}
};