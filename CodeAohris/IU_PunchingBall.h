
#pragma once

#include "CoreMinimal.h"
#include "IU_HealthSystemComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "IU_PunchingBall.generated.h"

class AB_Boss;

UCLASS()
class INSIDEUS_API AIU_PunchingBall : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		UStaticMeshComponent* staticMesh = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		UBoxComponent* mycollider = nullptr;
	UPROPERTY(EditAnywhere, Category = "Loriel")
	UIU_HealthSystemComponent* healthComp = nullptr;
	UPROPERTY(EditAnywhere, Category = "Loriel",BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		float timeBeforeDpsCountStop = 5;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		float timeBeforeLastDamage = 0;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		float globalTimer = 0;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		bool canGlobalTimerIncrease = false;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		bool canCalculate = false;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		float totalDamage = 0;

	UPROPERTY(EditAnywhere)
	AB_Boss* bossOwner = nullptr;
	
public:
	FORCEINLINE UIU_HealthSystemComponent* GetHealthComponent() const { return healthComp; }
	
public:	
	
	AIU_PunchingBall();
	void StartDamageCount(float _damage);

	void SetHarpoonData(int _iStackNeededToStun, float _fHarpoonStunDuration);
	void ResetHarpoonStack();
	
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	//UFUNCTION() void OnOverlap(AActor* _me, AActor* _other);
	UFUNCTION(BlueprintCallable) void DestroyPunchingBall();
	UFUNCTION(BlueprintCallable) void IncreaseTimer();
	UFUNCTION(BlueprintCallable) void IncreaseGlobalTimer();
};
