#pragma once

#include "CoreMinimal.h"
#include "P_Pattern.h"

#include "ENUM_Pivot.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"

#include "P_PatternLaser.generated.h"

class UP_PB_Laser;

UCLASS()
class INSIDEUS_API AP_PatternLaser : public AP_Pattern
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* laserCollider = nullptr;

	UPROPERTY(VisibleAnywhere)
	UP_PB_Laser* laserBehavior = nullptr;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* niagaraLaserLeft = nullptr;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* niagaraLaserRight = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	FName socketParentName = "";

public:
	FORCEINLINE UP_PB_Laser* GetLaserBehavior() const { return laserBehavior; }
	FORCEINLINE const FName& GetSocketParentName() const { return socketParentName; }
	FORCEINLINE void SetSocketParentName(const FName& _name) { socketParentName = _name; }

public:
	AP_PatternLaser();
	
	virtual void InitializePattern(UP_DT_BaseDataTable* _dataTable) override;
	virtual void InitializePatternData(FPatternData& _patternData) override;
	
	void InitializeLaserActor(const float& _laserLength, const float& _laserRadius, const EPivot& _laserPivot) const;
private:
	void InitializeLeftLaser(const float& _laserLength, const float& _laserRadius) const;
	void InitializeRightLaser(const float& _laserLength, const float& _laserRadius) const;
	void InitializeCenterLaser(const float& _laserLength, const float& _laserRadius) const;


public:
	virtual void DeActivatePatternBehavior() override;
};