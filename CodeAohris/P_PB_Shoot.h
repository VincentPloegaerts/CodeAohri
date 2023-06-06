#pragma once
#include "CoreMinimal.h"

#include "P_PatternBehavior.h"

#include "STRUCT_ShootSettings.h"
#include "P_PB_Shoot.generated.h"

class AM_FeedbackManager;
class AM_BlackBoardManager;
class UP_PB_Random;
class UP_DT_ShootDataTable;
class AM_ProjectileManager;
class AP_PatternBase;

UCLASS(Abstract)
class INSIDEUS_API UP_PB_Shoot : public UP_PatternBehavior
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShoot);

protected:
	UPROPERTY()
	FProjectileSettings projectileSettings;
	
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties, EditCondition = "bIsEnable"))
		FShootSettings shootSettings;
	
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Shoot | Values")
		UP_PB_Random* uRandom = nullptr;

private:
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Shoot | Values")
		float fElapsedTime = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Shoot | Values")
		float fTime = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Shoot | Values")
		int iProjectileFired = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Shoot | Values")
		UP_DT_ShootDataTable* shootDataTable = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Shoot | Values")
		AM_ProjectileManager* projectileManager = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Shoot | Values")
		AM_FeedbackManager* feedbackManager = nullptr;

	UPROPERTY()
	AM_BlackBoardManager* blackBoardManager = nullptr;

protected:
	UPROPERTY()
	FOnShoot onShoot;
	
public:
	FORCEINLINE FOnShoot& OnShoot() { return onShoot; }
	
public:
	virtual void ActivateBehavior(UP_DT_BaseDataTable* _dataTable) override;
	virtual void ActivateBehavior(FPatternData& _patternData) override;
	virtual void TickBehavior(const float& _deltaTime) override;

protected:
	virtual void InitializeShoot(UP_DT_ShootDataTable* _shootDataTable);
	virtual void InitializeShoot(FPatternData& _patternData);
	virtual void Shoot();
	void ShootProjectile(const FVector& _location, FVector& _defaultDirection) const;

private:
	UFUNCTION(BlueprintCallable) void CheckShotComplete();
	UFUNCTION(BlueprintCallable) bool IsFireDurationComplete();
	UFUNCTION(BlueprintCallable) bool IsFireQuantityComplete();
	UFUNCTION() void DisableShootBehavior(AP_PatternBase* _pattern);
};