
#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "C_PowerOrb.generated.h"

UCLASS()
class INSIDEUS_API AC_PowerOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	AC_PowerOrb();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	UStaticMeshComponent* staticMesh = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	USphereComponent* orbCollider = nullptr;
	
	UPROPERTY()
	AC_PowerOrb* twin = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult &SweepResult);

	UFUNCTION()
	void MyDestroyed(AActor* Act);

public:
	UFUNCTION() void SetTwin(AC_PowerOrb* _twin);
};
