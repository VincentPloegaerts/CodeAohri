
#include "GPE_WallShotBox.h"

AGPE_WallShotBox::AGPE_WallShotBox()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool AGPE_WallShotBox::ShouldTickIfViewportsOnly() const
{
	return showDebug;
}
#if WITH_EDITOR
void AGPE_WallShotBox::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DrawDebugBox(GetWorld(),GetActorLocation(),boxRange,GetActorRotation().Quaternion(),FColor::Yellow,false,-1,0,5.0f);		
}
#endif
bool AGPE_WallShotBox::IsInsideBox(const FVector& _position)
{
	const FVector& _turretToPlayer = _position  - GetActorLocation();
	return (_turretToPlayer.ProjectOnTo(GetActorForwardVector()).Size() < (GetActorForwardVector() * boxRange.X).Size() &&
			_turretToPlayer.ProjectOnTo(GetActorRightVector()).Size() < (GetActorRightVector() * boxRange.Y).Size() &&
			_turretToPlayer.ProjectOnTo(GetActorUpVector()).Size() < (GetActorUpVector() * boxRange.Z).Size());
}
