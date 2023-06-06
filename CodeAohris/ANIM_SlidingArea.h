
#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ANIM_SlidingArea.generated.h"

UCLASS()
class INSIDEUS_API AANIM_SlidingArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AANIM_SlidingArea();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* boxCollider = nullptr;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult &SweepResult);
	
};
