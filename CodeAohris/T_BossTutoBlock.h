#pragma once
#include "CoreMinimal.h"
#include "T_TutoLogic.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/Actor.h"
#include "T_BossTutoBlock.generated.h"


UCLASS()
class INSIDEUS_API AT_BossTutoBlock : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True),Category = "Boss Tuto Block|Components")
		UBoxComponent* boxComponent = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True),Category = "Boss Tuto Block|Components")
		UBillboardComponent* billBoard = nullptr;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		AT_TutoLogic* tutoLogicRef = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True),Category = "Boss Tuto Block| Actor To Spawn")
	TSubclassOf<AActor> toSpawn = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True),Category = "Boss Tuto Block| Actor Spawn Location")
	USceneComponent* enterObstacleSpawnLocation = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True),Category = "Boss Tuto Block| Actor Spawn Location")
	USceneComponent* exitObstacleSpawnLocation = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True),Category = "Boss Tuto Block|Actor Spawn Location Billboard")
	USceneComponent* playerTeleportationLocation = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True), Category = "Boss Tuto Block| Values")
		float fMaxOpenHeight = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True), Category = "Boss Tuto Block| Values")
		float fDoorOpenSpeed = 1.f;
	
	UPROPERTY()
	AActor* enterObstacle = nullptr;
	UPROPERTY()
	AActor* exitObstacle = nullptr;
	

	UPROPERTY()
	AIU_Player* playerRef = nullptr;

	UPROPERTY(EditAnywhere)
	bool bCanOpen = false;
	UPROPERTY()
	FVector startPosition = FVector::Zero();

public:	
	AT_BossTutoBlock();

	FORCEINLINE void SetCanOpen(bool _status) { bCanOpen = _status; }
	FORCEINLINE FVector GetExitLocation() { return exitObstacleSpawnLocation->GetComponentLocation(); }

private:	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable) void Init();
	UFUNCTION(BlueprintCallable) void OpenDoor();
	UFUNCTION(BlueprintCallable) void OnActorOverlap(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _index, bool _sweep, const FHitResult& _result);
public:
	UFUNCTION(BlueprintCallable) void DestroyObstacle();
	UFUNCTION(BlueprintCallable) void TeleportPlayer();
};
