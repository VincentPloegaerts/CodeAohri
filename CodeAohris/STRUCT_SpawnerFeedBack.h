#pragma once
#include "FMODEvent.h"
#include "NiagaraSystem.h"
#include "STRUCT_SongFeedBackData.h"
#include "STRUCT_SpawnerFeedBack.generated.h"

USTRUCT()
struct FSpawnerFeedBack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "FeedBack")
	bool bRaycastFXOnGround = false;

	UPROPERTY(EditAnywhere, Category = "FeedBack")
	TArray<TEnumAsByte<EObjectTypeQuery>> hitGroundLayer;

	UPROPERTY(EditAnywhere, Category = "FeedBack")
	FSongFeedBackData soundFeedbackData = FSongFeedBackData();

	UPROPERTY(EditAnywhere, Category = "FeedBack")
	TObjectPtr<UNiagaraSystem> fxFeedBack;

	UPROPERTY(EditAnywhere, Category = "FeedBack")
	bool attachFX = true;
	
	UPROPERTY(EditAnywhere, Category = "FeedBack")
	float fDelayFX = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "FeedBack")
	FVector feedbackOffsetFX = FVector::ZeroVector;
};