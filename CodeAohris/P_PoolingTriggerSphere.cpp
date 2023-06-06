#include "P_PoolingTriggerSphere.h"

AP_PoolingTriggerSphere::AP_PoolingTriggerSphere()
{
	sphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	sphereComponent->SetupAttachment(RootComponent);
}

void AP_PoolingTriggerSphere::EnablePoolingTrigger()
{
	Super::EnablePoolingTrigger();
	
	if (!sphereComponent) return;
	sphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AP_PoolingTriggerSphere::DisablePoolingTrigger()
{
	Super::DisablePoolingTrigger();
	
	if (!sphereComponent) return;
	sphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
}

void AP_PoolingTriggerSphere::SetTriggerSphereRadius(const float& _radius) const
{
	if (!sphereComponent) return;
	sphereComponent->SetSphereRadius(_radius);
}

#if WITH_EDITOR
void AP_PoolingTriggerSphere::DrawPoolingTriggerDebug()
{
	if (!bDebug || !sphereComponent) return;
	const FColor& _color = bDisable ? FColor::Red : bDisableProcess ? FColor::Orange : FColor::Green;
	DrawDebugSphere(GetWorld(), GetActorLocation(), sphereComponent->GetScaledSphereRadius(), 15, _color, false, -1, 0, 10);
}
#endif