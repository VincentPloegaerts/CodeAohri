#pragma once
#include "STRUCT_AnimFX.generated.h"

class UNiagaraSystem;
class USceneComponent;

USTRUCT()
struct FAnimFX
{
	GENERATED_BODY()

	UPROPERTY()
	UNiagaraSystem* fx = nullptr;

	UPROPERTY()
	USceneComponent* attachedTo = nullptr;

	UPROPERTY()
	FVector offsetPosition = FVector::ZeroVector;

	UPROPERTY()
	FRotator rotation = FRotator::ZeroRotator;

	UPROPERTY()
	bool attachFX = false;
};