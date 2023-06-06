#include "B_BossCritPointCapsule.h"

AB_BossCritPointCapsule::AB_BossCritPointCapsule()
{
	capsuleCollider = CreateDefaultSubobject<UCapsuleComponent>("Capsule Collider");
	capsuleCollider->SetupAttachment(RootComponent);
}

void AB_BossCritPointCapsule::GetCritPointSettings(FCritPointSettings& _critPointSettings) const
{
	Super::GetCritPointSettings(_critPointSettings);
	_critPointSettings.radius = capsuleCollider->GetUnscaledCapsuleRadius();
	_critPointSettings.height = capsuleCollider->GetUnscaledCapsuleHalfHeight();
}

void AB_BossCritPointCapsule::SetCritPointSettings(const FCritPointSettings& _critPointSettings)
{
	Super::SetCritPointSettings(_critPointSettings);
	capsuleCollider->SetCapsuleRadius(_critPointSettings.radius);
	capsuleCollider->SetCapsuleHalfHeight(_critPointSettings.height);
}

void AB_BossCritPointCapsule::DrawCritPointDebug()
{
	DrawDebugCapsule(GetWorld(), GetActorLocation(), capsuleCollider->GetScaledCapsuleHalfHeight(), capsuleCollider->GetScaledCapsuleRadius(), GetActorQuat(), FColor::Green, false, -1, 0, 5);
}
