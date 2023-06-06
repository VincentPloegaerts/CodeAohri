#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M_PatternsTickManager.generated.h"

/**
 * Class used for optimizing Tick on PatternBehavior components by
 * registering and Broadcast() there tick function when they are enabled.
 */
UCLASS()
class INSIDEUS_API AM_PatternsTickManager : public AActor
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTick, const float&, _deltaTime);
	
private:
	UPROPERTY()
	float fWorldSpeed = 1;
	
	UPROPERTY()
	FOnTick onTick;

public:
	FORCEINLINE FOnTick& GetOnTick() { return onTick; }
	FORCEINLINE void SetWorldSpeed(const float& _speed) { fWorldSpeed = _speed; }
	
public:
	AM_PatternsTickManager();

protected:
	virtual void Tick(float DeltaTime) override;
};