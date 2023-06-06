#pragma once
#include "FMODEvent.h"
#include "STRUCT_SongFeedBackData.h"
#include "STRUCT_FeedBackData.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FFeedBackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "FeedBack")
	bool bRaycastFXOnGround = false;

	UPROPERTY(EditAnywhere, Category = "FeedBack")
	TArray<TEnumAsByte<EObjectTypeQuery>> hitGroundLayer;
	
	UPROPERTY(EditAnywhere, Category = "FeedBack")
	float fBehaviorActivationDelay = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "FeedBack")
	FSongFeedBackData soundFeedbackData = FSongFeedBackData();

	UPROPERTY(EditAnywhere, Category = "FeedBack")
	TObjectPtr<UAnimSequenceBase> animationFeedback;
	
	UPROPERTY(EditAnywhere, Category = "FeedBack")
	TObjectPtr<UNiagaraSystem> fxFeedBack;
	
	UPROPERTY(EditAnywhere, Category = "FeedBack")
	bool attachFX = true;
	
	UPROPERTY(EditAnywhere, Category = "FeedBack")
	bool bLinkFXToAnimation = false;
	
	UPROPERTY(EditAnywhere, Category = "FeedBack")
	float fDelayFX = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "FeedBack")
	FVector feedbackOffsetFX = FVector(0);

	UPROPERTY()
	unsigned iPatternIndex = 0;
		
	FFeedBackData() { }
};