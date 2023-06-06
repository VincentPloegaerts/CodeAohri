#pragma once

#include "CoreMinimal.h"

#include "STRUCT_LaserSettings.h"

#include "P_PatternBehavior.h"
#include "P_PB_Laser.generated.h"

class AM_WorldTimeManager;
class AM_BlackBoardManager;
class AP_PatternBase;
class UP_DT_LaserDataTable;
class AIU_Player;
class UIU_HealthSystemComponent;

UCLASS()
class INSIDEUS_API UP_PB_Laser : public UP_PatternBehavior
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FLaserSettings laserSettings;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings")
	int iLaserPrecision = 6;
	
	//Total time since laser activation
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Laser | Values")
	float fElapsedTime = 0.0f;
	
	UPROPERTY()
	AIU_Player* player = nullptr;
	
	UPROPERTY()
	UIU_HealthSystemComponent* healthComponent = nullptr;

	UPROPERTY()
	bool bPlayerPresence = false;

	UPROPERTY()
	FName socketParentName = "";

	UPROPERTY()
	USkeletalMeshComponent* bossSkeletal = nullptr;
	
	UPROPERTY()
	const UP_DT_LaserDataTable* laserDataTable = nullptr;
	
	//Begin FX
	UPROPERTY()
	bool bRaycastPlayer = false;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings", meta = (ClampMin = "0"))
	float fFXLaserDistance = 50000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings")
	TArray<TEnumAsByte<EObjectTypeQuery>> fxLayer;

	UPROPERTY()
	UNiagaraComponent* niagaraEnvironment = nullptr;	

	UPROPERTY()
	UNiagaraComponent* niagaraPlayer = nullptr;	
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings")
	UNiagaraSystem* fxHitEnvironment = nullptr;

	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings")
	UNiagaraSystem* fxHitPlayer = nullptr;
	//End FX

	UPROPERTY()	AM_BlackBoardManager* blackBoardManager = nullptr;
	UPROPERTY()	AM_WorldTimeManager* worldTimeManager = nullptr;
	
	UPROPERTY()	const UWorld* world = nullptr;
	
public:
	virtual void ActivateBehavior(UP_DT_BaseDataTable* _dataTable) override;
	virtual void ActivateBehavior(FPatternData& _patternData) override;
	void InitializeLaser(const UP_DT_LaserDataTable* _dataTable);

	virtual void TickBehavior(const float& _deltaTime) override;
	
	UFUNCTION() void LaserBeginOverlap(AActor* _owner, AActor* _other);
	UFUNCTION() void LaserEndOverlap(AActor* _owner, AActor* _other);
	
	UFUNCTION() void DisableLaserBehavior(AP_PatternBase* _pattern);

	UFUNCTION() void RaycastLaser();
};