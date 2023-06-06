#include "P_PoolingTriggerBox.h"

AP_PoolingTriggerBox::AP_PoolingTriggerBox()
{
	boxComponent = CreateDefaultSubobject<UBoxComponent>("Box Component");
	boxComponent->SetupAttachment(RootComponent);
}

void AP_PoolingTriggerBox::EnablePoolingTrigger()
{
	Super::EnablePoolingTrigger();
	
	if (!boxComponent) return;
	boxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	
}
void AP_PoolingTriggerBox::DisablePoolingTrigger()
{
	Super::DisablePoolingTrigger();
	
	if (!boxComponent) return;
	boxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
}

#if WITH_EDITOR
void AP_PoolingTriggerBox::DrawPoolingTriggerDebug()
{
	if (!bDebug || !boxComponent) return;
	const FColor& _color = bDisable ? FColor::Red : bDisableProcess ? FColor::Orange : FColor::Green;
	DrawDebugBox(GetWorld(), GetActorLocation(), boxComponent->GetScaledBoxExtent(), GetActorQuat(), _color, false, -1, 0, 10);
}
#endif