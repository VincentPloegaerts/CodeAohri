#pragma once
#include "NiagaraSystem.h"

#include "STRUCT_OverrideFeedBack.generated.h"

USTRUCT()
struct FOverrideFeedBack
{
	GENERATED_BODY()

	//Force override of animation even if it's nullptr
	UPROPERTY(EditAnywhere)
	bool bForceOverrideAnimation = false;
	UPROPERTY(EditAnywhere)
	UAnimationAsset* overrideAnimation = nullptr;
	
	UPROPERTY(EditAnywhere)
	bool bForceOverrideSoundBase = false;
	UPROPERTY(EditAnywhere)
	USoundBase* overrideSoundBase = nullptr;

	UPROPERTY(EditAnywhere)
	bool bForceOverrideFX = false;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* overrideFX = nullptr;

	//UPROPERTY(EditAnywhere)
	//bool bForceOverride
};