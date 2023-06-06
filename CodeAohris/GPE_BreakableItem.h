#pragma once

#include "CoreMinimal.h"
#include "GPE_PlatformWithHealth.h"
#include "GPE_BreakableItem.generated.h"



UCLASS()
class INSIDEUS_API AGPE_BreakableItem : public AGPE_PlatformWithHealth
{
	GENERATED_BODY()
	//Array of protected objects, once this actor gets destroyed every actor in this list will become destroyable 
	UPROPERTY(EditAnywhere,Category = "GPE|Breakable Item",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		TArray<AGPE_PlatformWithHealth*> protectedObjects = TArray<AGPE_PlatformWithHealth*>();

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable) void Init();
	UFUNCTION(BlueprintCallable) void SetObjectsDestructible();
};
