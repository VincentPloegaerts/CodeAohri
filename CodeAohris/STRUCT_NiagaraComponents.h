#pragma once
#include "NiagaraSystem.h"
#include "STRUCT_NiagaraComponents.generated.h"

USTRUCT(BlueprintType)
struct FNiagaraComps
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Niagara Components", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	UNiagaraSystem* niagaraCharaHit = nullptr;

	UPROPERTY(EditAnywhere, Category = "Niagara Components", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	UNiagaraSystem* niagaraDeath = nullptr;

	UPROPERTY(EditAnywhere, Category = "Niagara Components", BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	UNiagaraSystem* niagaraDeathFall = nullptr;
	
	FNiagaraComps() {}
};