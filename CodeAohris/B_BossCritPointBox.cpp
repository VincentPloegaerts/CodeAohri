#include "B_BossCritPointBox.h"

AB_BossCritPointBox::AB_BossCritPointBox()
{
	boxCollider = CreateDefaultSubobject<UBoxComponent>("Box Collider");
	boxCollider->SetupAttachment(RootComponent);
}

void AB_BossCritPointBox::GetCritPointSettings(FCritPointSettings& _critPointSettings) const
{
	Super::GetCritPointSettings(_critPointSettings);
	_critPointSettings.extent = boxCollider->GetUnscaledBoxExtent();
}

void AB_BossCritPointBox::SetCritPointSettings(const FCritPointSettings& _critPointSettings)
{
	Super::SetCritPointSettings(_critPointSettings);
	boxCollider->SetBoxExtent(_critPointSettings.extent);
}

void AB_BossCritPointBox::DrawCritPointDebug()
{
	DrawDebugBox(GetWorld(), GetActorLocation(), boxCollider->GetScaledBoxExtent(), GetActorQuat(), FColor::Green, false, -1, 0, 2);
}
