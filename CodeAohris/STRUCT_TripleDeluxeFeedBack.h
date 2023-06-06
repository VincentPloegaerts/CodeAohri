#pragma once
#include "ENUM_AnimRigMode.h"
#include "STRUCT_FeedBackData.h"
#include "STRUCT_BossAnimatedStoneData.h"
#include "STRUCT_TripleDeluxeFeedBack.generated.h"

USTRUCT()
struct FTripleDeluxeFeedBack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Anim Mode")
	TEnumAsByte<EAnimRigMode> animRigMode = EAnimRigMode::BOSS__;

	
	UPROPERTY(EditAnywhere, Category = "Stone Start", meta = (EditCondition = "animRigMode != EAnimRigMode::BOSS__"))
	FBossAnimatedStoneData animatedStoneData = FBossAnimatedStoneData();
	
	UPROPERTY(EditAnywhere, Category = "Stone Start", meta = (EditCondition = "animRigMode != EAnimRigMode::BOSS__"))
	FFeedBackData startStoneFeedback = FFeedBackData();
	
	UPROPERTY(EditAnywhere, Category = "Stone Update", meta = (EditCondition = "animRigMode != EAnimRigMode::BOSS__"))
	FFeedBackData loopStoneFeedback = FFeedBackData();
	
	UPROPERTY(EditAnywhere, Category = "Stone End", meta = (EditCondition = "animRigMode != EAnimRigMode::BOSS__"))
	FFeedBackData endStoneFeedback = FFeedBackData();
	
	
	UPROPERTY(EditAnywhere, Category = "Start", meta = (EditCondition = "animRigMode != EAnimRigMode::STONE"))
	FFeedBackData startFeedBack = FFeedBackData();
	
	UPROPERTY(EditAnywhere, Category = "Update", meta = (EditCondition = "animRigMode != EAnimRigMode::STONE"))
	FFeedBackData loopFeedBack = FFeedBackData();
	
	UPROPERTY(EditAnywhere, Category = "End", meta = (EditCondition = "animRigMode != EAnimRigMode::STONE"))
	FFeedBackData endFeedBack = FFeedBackData();

	
	UPROPERTY()	bool bIsGPEShotFeedBack = false;
	UPROPERTY()	bool bWaitEndOfPattern = true;
};