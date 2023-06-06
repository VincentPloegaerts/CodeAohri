#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/Actor.h"
#include "M_LevelStreamerDeluxeEdition.generated.h"

class AIU_Player;
class UBoxComponent;

UCLASS()
class INSIDEUS_API AM_LevelStreamerDeluxeEdition : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UBoxComponent* colliderToGoForward = nullptr;

	UPROPERTY(EditAnywhere)
	UBoxComponent* colliderToGoBackward = nullptr;
	
	UPROPERTY(EditAnywhere)
	UBillboardComponent* billBoard = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<FName> levelsToLoad = TArray<FName>();

	UPROPERTY(EditAnywhere)
	TArray<FName> levelsToUnload = TArray<FName>();

	UPROPERTY()
	AIU_Player* player = nullptr;
	
	UPROPERTY()
	FTimerHandle initPlayerTimer = FTimerHandle();
	
public:
	AM_LevelStreamerDeluxeEdition();
public:
	UFUNCTION(BlueprintCallable) FORCEINLINE UBoxComponent* GetColliderToGoForward() {return colliderToGoForward;}
	UFUNCTION(BlueprintCallable) FORCEINLINE UBoxComponent* GetColliderToGoBackward() {return colliderToGoBackward;}
	UFUNCTION(BlueprintCallable) FORCEINLINE TArray<FName> GetLevelsToLoad() {return levelsToLoad;}
	UFUNCTION(BlueprintCallable) FORCEINLINE TArray<FName> GetLevelsToUnload() {return levelsToUnload;}
protected:
	virtual void BeginPlay() override;

	UFUNCTION() void InitPlayer();

	UFUNCTION() void CollideForward(UPrimitiveComponent* _mine, AActor* _otherActor, UPrimitiveComponent* _otherComponent, int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult);
	UFUNCTION() void CollideBackward(UPrimitiveComponent* _mine, AActor* _otherActor, UPrimitiveComponent* _otherComponent, int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult);

public:
	UFUNCTION() void ActivateArena();
};