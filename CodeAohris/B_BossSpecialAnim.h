#pragma once

#include "CoreMinimal.h"
#include "IU_Macros.h"
#include "Components/ActorComponent.h"
#include "B_BossSpecialAnim.generated.h"

class AM_FeedbackManager;
class UP_PatternCaster;
class AP_PatternBase;
class UP_DT_BaseDataTable;
class AB_BossAnimatedStone;
class AB_Boss;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UB_BossSpecialAnim : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()	AB_Boss* boss = nullptr;
	UPROPERTY()	UP_PatternCaster* patternCaster = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AB_BossAnimatedStone> bossStoneToSpawn;

	UPROPERTY(VisibleAnywhere)
	TArray<AB_BossAnimatedStone*> allBossStone;


	UPROPERTY() AM_FeedbackManager* feedBackManager = nullptr;
	UPROPERTY() UWorld* world = nullptr;
	FTimerManager* timerManager = nullptr;
	
	UPROPERTY()	FTimerHandle initTimer = FTimerHandle();

public:
	UB_BossSpecialAnim();

private:
	virtual void BeginPlay() override;
	UFUNCTION() void Init();

	UFUNCTION() void AddNewSpecialAnimation(const UP_DT_BaseDataTable* _dataTable, AP_PatternBase* _pattern);
	UFUNCTION() void RemoveSpecialAnimation(AP_PatternBase* _pattern);

	UFUNCTION() void DestroyBossStone(AB_BossAnimatedStone* _bossStone);
	UFUNCTION() void DestroyEveryBossStone();

	AB_BossAnimatedStone* GetSameIDBossStone(const uint& _id) const;
};