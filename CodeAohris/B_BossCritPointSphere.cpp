#include "B_BossCritPointSphere.h"

AB_BossCritPointSphere::AB_BossCritPointSphere()
{
	sphereCollider = CreateDefaultSubobject<USphereComponent>("Sphere Collider");
	sphereCollider->SetupAttachment(RootComponent);
}

void AB_BossCritPointSphere::GetCritPointSettings(FCritPointSettings& _critPointSettings) const
{
	Super::GetCritPointSettings(_critPointSettings);
	_critPointSettings.radius = sphereCollider->GetUnscaledSphereRadius();
}

void AB_BossCritPointSphere::SetCritPointSettings(const FCritPointSettings& _critPointSettings)
{
	Super::SetCritPointSettings(_critPointSettings);
	sphereCollider->SetSphereRadius(_critPointSettings.radius);
}

void AB_BossCritPointSphere::DrawCritPointDebug()
{
	DrawDebugSphere(GetWorld(), GetActorLocation(), sphereCollider->GetScaledSphereRadius(), 15, FColor::Green, false, -1, 0, 2);
}
