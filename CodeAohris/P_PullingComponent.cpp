#include "P_PullingComponent.h"
#include "DrawDebugHelpers.h"
#include "IU_Macros.h"
#include "M_GameMode.h"
#include "TimerManager.h"
#include "M_ProjectileManager.h"
#include "P_Projectile.h"

UP_PullingComponent::UP_PullingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UP_PullingComponent::BeginPlay()
{
	Super::BeginPlay();
	InitManagers();
	world->GetTimerManager().SetTimer(checkTimer, this, &UP_PullingComponent::CheckProjectiles, fCheckRate, true, FMath::RandRange(0.0f, fRandFirstDelay));
}
void UP_PullingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DrawArena();
}

void UP_PullingComponent::InitManagers()
{
	world = GetWorld();
	if (!world) return;
	
	if (AM_GameMode* _gamemode = world->GetAuthGameMode<AM_GameMode>())
	{
		projectileManager = _gamemode->GetProjectileManager();
	}
}
void UP_PullingComponent::DrawArena() const
{
	if (!bDrawGizmos) return;

	const FVector& _ownerLocation = GetOwnerLocation();
	
	if (bUseBoxToCheck)
	{
		DrawDebugBox(GetWorld(), _ownerLocation, arenaExtent, arenaColor);
	}

	else
	{
		DrawDebugSphere(GetWorld(), _ownerLocation, fArenaRadius, iArenaSegments, arenaColor);
	}
}
void UP_PullingComponent::CheckProjectiles()
{
	if (projectiles.Num() < 1 || !projectileManager)return;

	bool _hasResetIndex(false);
	
	for (int _count(0); _count < iCheckCount; ++_count)
	{
		const int& _checkIndex = iCurrentCheckIndex - _count;
		
		if (_checkIndex < 0 || _checkIndex > projectiles.Num() - 1)
		{
			iCurrentCheckIndex = projectiles.Num() - 1;

			if (_hasResetIndex)return;

			_hasResetIndex = true;
			--_count;
			continue;
		}
		
		AP_Projectile* _projectile = projectiles[_checkIndex];
		if (!_projectile || IsIntoArena(_projectile)) continue;
		//projectileManager->GarbageProjectile(_projectile, projectiles);
	}

	iCurrentCheckIndex -= iCheckCount;
}
bool UP_PullingComponent::IsIntoArena(const AP_Projectile* _projectile) const
{
	bool _status;
	const FVector& _center = GetOwnerLocation();
	const FVector& _projectileLocation = _projectile->GetActorLocation();

	if (!bUseBoxToCheck)
	{
		_status = FVector::Distance(_projectileLocation, _center) < fArenaRadius;
	}

	else
	{
		_status = _projectileLocation.X - _center.X < arenaExtent.X
			   && _projectileLocation.Y - _center.Y < arenaExtent.Y
			   && _projectileLocation.Z - _center.Z < arenaExtent.Z;
	}
	
	return _status;
}

void UP_PullingComponent::GarbageAllProjectiles()
{
	if (!projectileManager)return;

	uint projectileNum(projectiles.Num());
	while (projectileNum)
	{
		if (AP_Projectile* _projectile = projectiles[0])
		{
			//projectileManager->GarbageProjectile(_projectile, projectiles);
		}
		else
		{
			LOG("UP_PullingComponent::GarbageAllProjectiles -> A projectile was nullptr");
		}
		--projectileNum;
	}
}