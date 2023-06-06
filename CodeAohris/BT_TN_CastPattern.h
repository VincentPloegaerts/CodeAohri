#pragma once

#include "CoreMinimal.h"
#include "BT_TN_Cast.h"
#include "STRUCT_PatternMovementData.h"
#include "BT_TN_CastPattern.generated.h"

class UB_BossMovement;
class UP_DT_BaseDataTable;
class AM_BlackBoardManager;

UCLASS()
class INSIDEUS_API UBT_TN_CastPattern : public UBT_TN_Cast
{
	GENERATED_BODY()
	
	friend class AM_BlackBoardManager;

	UPROPERTY()
	bool bHasBeenReplacedWithDislodge = false;

	UPROPERTY()
	UP_DT_BaseDataTable* replacedPattern = nullptr;

	UPROPERTY()
	bool replacedWaitEndOfPattern = false;
	
	UPROPERTY()
	FSpawnSettings replacedSpawnSettings;

	const FTripleDeluxeFeedBack* overridedFeedBack = nullptr;
	
protected:
	/*The current Data Table used for instancing boss attack.*/
	UPROPERTY(EditAnywhere, Category = "Cast | Pattern")
	UP_DT_BaseDataTable* pattern = nullptr;

	UPROPERTY(EditAnywhere, Category = "Cast | Pattern")
	bool bWaitEndOfPattern = false;
	
	/*Parameters used for the spawn location and rotation.*/
	UPROPERTY(EditAnywhere, Category = "Cast | Spawn")
	FSpawnSettings spawnSettings;
	
	UPROPERTY()
	bool hasTryFinishButMovementIsNotEnd = false;
	
	UPROPERTY()
	bool bIsSelfActorBoss = false;
	
	UPROPERTY()
	UB_BossMovement* bossMovement = nullptr;
	
	UPROPERTY()
	AM_BlackBoardManager* blackBoardManager = nullptr;
	
	UPROPERTY()
	FTimerHandle delayTimer;

public:
	UBT_TN_CastPattern();
	
protected:
	virtual EBTNodeResult::Type InitCastTask() override;
	UFUNCTION() void PostStunInitTask();

	UFUNCTION() void InitCastBehavior();

	/*Function used to finish this task and un-sub to the pattern caster.*/
	UFUNCTION() void OnPatternBehaviourFinishedAndCheckMove();
	UFUNCTION() void WaitBossAnimAnimationEnd();
	
	UFUNCTION() void SetMovementHasFinishTrue();
	
public:
	UFUNCTION() void SkipPattern();
	UFUNCTION() void ReplacePatternDataAndCastIt(const FPatternMovementData& _patternMovementData);
	void OverrideTripleFeedBack(const FTripleDeluxeFeedBack* _overrideFeedBack);
	
protected:
	virtual void PostInitProperties() override;
	virtual FString GetStaticDescription() const override;
};