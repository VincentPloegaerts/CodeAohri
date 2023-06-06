#include "B_BossCritPoint.h"

#include "IU_Macros.h"
#include "IU_PlayerArrow.h"
#include "Engine/SkeletalMeshSocket.h"

AB_BossCritPoint::AB_BossCritPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
}

void AB_BossCritPoint::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (debug)
		DrawCritPointDebug();
}

void AB_BossCritPoint::DrawCritPointDebug()
{
	//...
}

void AB_BossCritPoint::DamageCritPoint(const AIU_PlayerArrow* _arrow) const
{
	if (!_arrow || !boss) return;
	const float _damage = _arrow->GetDamage() * (1 + effect / 100);
	_arrow->DamageBoss(boss, _damage);
}

void AB_BossCritPoint::GetCritPointSettings(FCritPointSettings& _critPointSettings) const
{
	_critPointSettings.effect = effect;
	_critPointSettings.location = RootComponent->GetRelativeLocation();
	_critPointSettings.rotation = RootComponent->GetRelativeRotation();
	_critPointSettings.scale = GetActorRelativeScale3D();
	//...
}
void AB_BossCritPoint::SetCritPointSettings(const FCritPointSettings& _critPointSettings)
{
	effect = _critPointSettings.effect;
	SetActorRelativeLocation(_critPointSettings.location);
	SetActorRelativeRotation(_critPointSettings.rotation);
	SetActorRelativeScale3D(_critPointSettings.scale);
	//...
}
