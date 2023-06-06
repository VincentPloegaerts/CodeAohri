#pragma once

#include "CoreMinimal.h"
#include "GPE_BaseGPE.h"
#include "GPE_TeleportBack.generated.h"

class AIU_Player;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class INSIDEUS_API AGPE_TeleportBack : public AGPE_BaseGPE
{
	GENERATED_BODY()

	// Collider used to kill the player
	UPROPERTY(EditAnywhere, Category = "GPE|Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UBoxComponent* triggerBox = nullptr;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* debugBox = nullptr;

	UPROPERTY(EditAnywhere)
	USceneComponent* teleportPlayerLocation = nullptr;

	UPROPERTY()	AIU_Player* player = nullptr;

	UPROPERTY() FTimerHandle initTimer = FTimerHandle();
	
	UPROPERTY() const UWorld* world = nullptr;
	
public:
	AGPE_TeleportBack();
	
private:
	virtual void BeginPlay() override;

	UFUNCTION() void InitPlayer();
	UFUNCTION(BlueprintCallable) void OnCollision(AActor* _self, AActor* _other);
};