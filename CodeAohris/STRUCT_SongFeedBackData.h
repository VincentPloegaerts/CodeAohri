#pragma once
#include "STRUCT_SongFeedBackData.generated.h"

class UFMODEvent;

USTRUCT()
struct FSongFeedBack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UFMODEvent* soundFeedback = nullptr;
	
	UPROPERTY(EditAnywhere)
	bool bAttachToRequestingActor = false;
	
	UPROPERTY(EditAnywhere)
	FVector vSoundOffset = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere)
	bool bIsLoopingSong = false;
};

USTRUCT()
struct FSongFeedBackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "10"))
	float fActivationDelay = 0.f;

	UPROPERTY(EditAnywhere)
	TArray<FSongFeedBack> soundFeedBack = TArray<FSongFeedBack>();
};