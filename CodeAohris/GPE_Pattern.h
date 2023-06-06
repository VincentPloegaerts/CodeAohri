#pragma once

#include "CoreMinimal.h"
#include "GPE_BaseGPE.h"
#include "GPE_Pattern.generated.h"

UCLASS()
class INSIDEUS_API AGPE_Pattern : public AGPE_BaseGPE
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteBehavior, AGPE_Pattern*, _pattern);

private:
	//Use CompleteBehavior() instead.
	FOnCompleteBehavior onCompleteBehavior;

public:
	FORCEINLINE FOnCompleteBehavior& OnCompleteBehavior() { return onCompleteBehavior; }

public:
	AGPE_Pattern();

	UFUNCTION() virtual void BehaviorTick(const float& _deltaTime);

protected:
	UFUNCTION() virtual void CompleteBehavior();
};