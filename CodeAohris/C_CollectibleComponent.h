#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "C_CollectibleComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UC_CollectibleComponent : public UActorComponent
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCollectibleUpdated, int, _current, int, _max);
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Collectible Component")
	int iCurrentCollectible = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "Collectible Component")
	int iMaxCollectible = 0;

	UPROPERTY()
	FOnCollectibleUpdated onCollectibleUpdated = FOnCollectibleUpdated();

public:
	FORCEINLINE int GetCurrentCollectible() {return iCurrentCollectible;}
	FORCEINLINE FOnCollectibleUpdated* OnCollectibleUpdated() { return &onCollectibleUpdated; }

	private :
		FORCEINLINE void EventBroadcast() {onCollectibleUpdated.Broadcast(iCurrentCollectible, iMaxCollectible);}

private:
	virtual void BeginPlay() override;
	void InitMaxCollectible();
	
public:	
	UC_CollectibleComponent();
	UFUNCTION(BlueprintCallable) void AddCollectible(const int _number = 1);
		
};
