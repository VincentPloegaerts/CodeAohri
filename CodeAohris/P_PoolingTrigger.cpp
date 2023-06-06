#include "P_PoolingTrigger.h"

#include "P_Projectile.h"
#include "IU_Macros.h"

AP_PoolingTrigger::AP_PoolingTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");
}

void AP_PoolingTrigger::BeginPlay()
{
	Super::BeginPlay();
	InitializePoolingTrigger();	
}
void AP_PoolingTrigger::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bDisableProcess)
	{
		fTime += DeltaSeconds;
		if (fTime > fDisableTimer)
			DisablePoolingTrigger();
	}

#if WITH_EDITOR
	DrawPoolingTriggerDebug();
#endif
}

void AP_PoolingTrigger::InitializePoolingTrigger()
{
	OnActorEndOverlap.AddUniqueDynamic(this, &AP_PoolingTrigger::OnEndOverlapPoolingTrigger);
	DisablePoolingTrigger();
}

void AP_PoolingTrigger::EnablePoolingTrigger()
{
	bDisable = false;
	bDisableProcess = false;
	
	//Declaration in Child class...
}
void AP_PoolingTrigger::BeginDisablePoolingTrigger()
{
	if (bDisable || bDisableProcess) return;
	
	fTime = 0;
	bDisableProcess = true;
}
void AP_PoolingTrigger::DisablePoolingTrigger()
{
	bDisableProcess = false;
	bDisable = true;
	fTime = 0;
	
	//Declaration in Child class...
}

void AP_PoolingTrigger::OnEndOverlapPoolingTrigger(AActor* _self, AActor* _other)
{
	if (AP_Projectile* _projectile = Cast<AP_Projectile>(_other))
		_projectile->PoolProjectile();
}

#if WITH_EDITOR
void AP_PoolingTrigger::DrawPoolingTriggerDebug()
{
	//	
}
#endif