
#include "T_ProjectileDetectionSphere.h"
#include "M_GameMode.h"
#include "T_ProjectileGuidedTuto.h"
#include "T_TutoLogic.h"

void UT_ProjectileDetectionSphere::BeginPlay()
{
	Super::BeginPlay();
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FTimerHandle _handle;
	GetWorld()->GetTimerManager().SetTimer(_handle,this,&UT_ProjectileDetectionSphere::Init,2.0f,false);
}

void UT_ProjectileDetectionSphere::Init()
{
	if (AT_TutoLogic* _logic =GetWorld()->GetAuthGameMode<AM_GameMode>()->GetTutoLogic())
		_logic->RegisterSphere(this);
}

void UT_ProjectileDetectionSphere::ModifyRadius(float _newRadius)
{
	this->SphereRadius = _newRadius;
}

