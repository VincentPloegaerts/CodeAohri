#include "P_PatternLaser.h"

#include "IU_Macros.h"
#include "P_PB_Laser.h"

#include "M_PatternsTickManager.h"

#include "P_DT_LaserDataTable.h"

AP_PatternLaser::AP_PatternLaser()
{
	PrimaryActorTick.bCanEverTick = false;
	laserCollider = CreateDefaultSubobject<UCapsuleComponent>("Laser Collider");
	laserCollider->SetupAttachment(RootComponent);
	laserCollider->SetRelativeRotation(FRotator(90.0f, 0, 0));

	niagaraLaserLeft = CreateDefaultSubobject<UNiagaraComponent>("Laser FX Left");
	niagaraLaserLeft->SetupAttachment(RootComponent);

	niagaraLaserRight = CreateDefaultSubobject<UNiagaraComponent>("Laser FX Right");
	niagaraLaserRight->SetupAttachment(RootComponent);
	niagaraLaserRight->SetRelativeRotation(FRotator(0, 180, 0));
	
	laserBehavior = CreateDefaultSubobject<UP_PB_Laser>("LaserBehavior");
	AddOwnedComponent(laserBehavior);
}

void AP_PatternLaser::InitializePattern(UP_DT_BaseDataTable* _dataTable)
{
	Super::InitializePattern(_dataTable);
	niagaraLaserLeft->Activate(true);
	niagaraLaserRight->Activate(true);
	UP_DT_LaserDataTable* _laserDataTable = Cast<UP_DT_LaserDataTable>(_dataTable);
	if (!_laserDataTable) return;

	InitializeBehavior(_laserDataTable->GetLaserState(), laserBehavior, _laserDataTable, patternsTickManager->GetOnTick());
}
void AP_PatternLaser::InitializePatternData(FPatternData& _patternData)
{
	Super::InitializePatternData(_patternData);
	
	InitializeBehavior(_patternData.GetLaserState(), laserBehavior, _patternData, patternsTickManager->GetOnTick());
}

void AP_PatternLaser::InitializeLaserActor(const float& _laserLength, const float& _laserRadius, const EPivot& _laserPivot) const
{
	if (!laserCollider) return;
	
	if (_laserPivot == EPivot::LEFT)
		InitializeLeftLaser(_laserLength, _laserRadius);
	else if (_laserPivot == EPivot::RIGHT)
		InitializeRightLaser(_laserLength, _laserRadius);
	else //(_laserPivot == EPivot::CENTER)
		InitializeCenterLaser(_laserLength, _laserRadius);
}

void AP_PatternLaser::InitializeLeftLaser(const float& _laserLength, const float& _laserRadius) const
{
	const float& _halfLength = _laserLength / 2.0f;

	niagaraLaserLeft->SetVectorParameter("Beam End", FVector(_laserLength, 0, 0));
	niagaraLaserRight->Deactivate();
	
	laserCollider->SetRelativeLocation(FVector(_halfLength, 0, 0));
	laserCollider->SetCapsuleHalfHeight(_halfLength);
	laserCollider->SetCapsuleRadius(_laserRadius);
}
void AP_PatternLaser::InitializeRightLaser(const float& _laserLength, const float& _laserRadius) const
{
	const float& _halfLength = _laserLength / 2.0f;

	niagaraLaserRight->SetVectorParameter("Beam End", FVector(_laserLength, 0, 0));
	niagaraLaserLeft->Deactivate();
	
	laserCollider->SetRelativeLocation(FVector(-_halfLength, 0, 0));
	laserCollider->SetCapsuleHalfHeight(_halfLength);
	laserCollider->SetCapsuleRadius(_laserRadius);
}
void AP_PatternLaser::InitializeCenterLaser(const float& _laserLength, const float& _laserRadius) const
{
	niagaraLaserRight->SetVectorParameter("Beam End", FVector(_laserLength, 0, 0));
	niagaraLaserLeft->SetVectorParameter("Beam End", FVector(_laserLength, 0, 0));
	
	laserCollider->SetCapsuleHalfHeight(_laserLength);
	laserCollider->SetCapsuleRadius(_laserRadius);
}

void AP_PatternLaser::DeActivatePatternBehavior()
{
	Super::DeActivatePatternBehavior();
	AM_PatternsTickManager::FOnTick& _onTick = patternsTickManager->GetOnTick();

	DeActivateBehavior(laserBehavior, _onTick);
}