#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "C_Collectible.generated.h"

UCLASS()
class INSIDEUS_API AC_Collectible : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Collectible values")
		UStaticMeshComponent* mesh = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Collectible values")
		UBoxComponent* collider = nullptr;
	
public:	
	AC_Collectible();

private:
	virtual void BeginPlay() override;
	void RegsiterToManager();

	UFUNCTION(BlueprintCallable) void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

};
