#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M_WorldTimeManager.generated.h"

class AM_GPE_PatternTickManager;
class AM_PatternsTickManager;
class AM_ProjectileManager;
class AM_GameMode;

UCLASS()
class INSIDEUS_API AM_WorldTimeManager : public AActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldSpeedChange, const float&, _speed);

	UPROPERTY()	AM_GPE_PatternTickManager* gpePatternTickManager = nullptr;
	UPROPERTY()	AM_PatternsTickManager* patternTickManager = nullptr;
	UPROPERTY()	AM_ProjectileManager* projectileManager = nullptr;

	UPROPERTY()
	bool bIsValid = false;

	//Slow world speed.
	UPROPERTY(EditAnywhere, Category = "World Time Manager", meta = (ClampMin = "0", ClampMax = "1"))
	float fSlowSpeed = 0.5f;

	//Slow world speed.
	UPROPERTY(EditAnywhere, Category = "World Time Manager", meta = (ClampMin = "0", ClampMax = "1"))
	float fCurrentWorldSpeed = 1.0f;
	
	UPROPERTY()	FTimerHandle timer = FTimerHandle();
	UPROPERTY()	UWorld* world = nullptr;
	UPROPERTY()	AM_GameMode* gameMode = nullptr;
	
	FTimerManager* timerManager = nullptr;
	
	UPROPERTY()	FOnWorldSpeedChange onWorldSpeedChange = FOnWorldSpeedChange();

public:
	FORCEINLINE FOnWorldSpeedChange& OnWorldSpeedChange() { return onWorldSpeedChange; }
	FORCEINLINE const float& GetCurrentWorldSpeed() const { return fCurrentWorldSpeed; }

public:	
	AM_WorldTimeManager();

protected:
	virtual void BeginPlay() override;
	/*virtual void Tick(float DeltaSeconds) override;*/

private:
	UFUNCTION() void InitializeManagers();

public:
	//Make the game run slower.
	void SetWorldInSlowMotion();

	//Make the game run at _speed value. Use SetWorldInSlowMotion() instead.
	void SetWorldClampedSpeed(float _speed);

	//Make the game tick at normal speed.
	void SetWorldDefaultSpeed();
};