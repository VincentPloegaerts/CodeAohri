#pragma once

#include "CoreMinimal.h"
#include "P_Pattern.h"

#include "P_PatternShoot.generated.h"

class AM_ProjectileManager;

class AP_Projectile;

class UP_PB_ShootPoint;
class UP_PB_ShootCircle;
class UP_PB_ShootLine;
class UP_PB_ShootArc;
class UP_PB_ShootCross;

class UP_PB_Random;
class UP_PB_Random2D;
class UP_PB_Random3D;

UCLASS(Blueprintable)
class INSIDEUS_API AP_PatternShoot : public AP_Pattern
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPatternShoot);
	
private:
	UPROPERTY(VisibleAnywhere)
	UP_PB_ShootPoint* shootPointBehavior = nullptr;
	UPROPERTY(VisibleAnywhere)
	UP_PB_ShootCircle* shootCircleBehavior = nullptr;
	UPROPERTY(VisibleAnywhere)
	UP_PB_ShootLine* shootLineBehavior = nullptr;
	UPROPERTY(VisibleAnywhere)
	UP_PB_ShootArc* shootArcBehavior = nullptr;
	UPROPERTY(VisibleAnywhere)
	UP_PB_ShootCross* shootCrossBehavior = nullptr;

	UPROPERTY(VisibleAnywhere)
	UP_PB_Random2D* uRandom2D = nullptr;
	UPROPERTY(VisibleAnywhere)
	UP_PB_Random3D* uRandom3D = nullptr;

	UPROPERTY()
	UP_PB_Random* uRandomizer = nullptr;
	
	UPROPERTY()
	AM_ProjectileManager* projectileManager = nullptr;
	
	UPROPERTY()
	FOnPatternShoot onPatternShoot;
	
public:
	FORCEINLINE FOnPatternShoot& OnPatternShoot() { return onPatternShoot; }
	
	FORCEINLINE UP_PB_ShootPoint* GetShootPointBehavior() const { return shootPointBehavior; }
	FORCEINLINE UP_PB_ShootCircle* GetShootCircleBehavior() const { return shootCircleBehavior; }
	FORCEINLINE UP_PB_ShootLine* GetShootLineBehavior() const { return shootLineBehavior; }
	FORCEINLINE UP_PB_ShootArc* GetShootArcBehavior() const { return shootArcBehavior; }
	FORCEINLINE UP_PB_ShootCross* GetShootCrossBehavior() const { return shootCrossBehavior; }
	
	FORCEINLINE UP_PB_Random2D* GetRandom2DComponent() const { return uRandom2D; }
	FORCEINLINE UP_PB_Random3D* GetRandom3DComponent() const { return uRandom3D; }

	FORCEINLINE UP_PB_Random* GetRandomizer() const { return uRandomizer; }
	
public:
	AP_PatternShoot();

private:
	virtual void BeginPlay() override;
	void InitializePatternShoot();

	UFUNCTION() void PatternBehaviorOnShoot();
	
public:
	//Called by TaskNode from boss BehaviorTree
	virtual void InitializePattern(UP_DT_BaseDataTable* _dataTable) override;
	virtual void InitializePatternData(FPatternData& _patternData) override;
	//Used to deactivate every pattern behavior
	virtual void DeActivatePatternBehavior() override;
};