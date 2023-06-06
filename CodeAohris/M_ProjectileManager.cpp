#include "M_ProjectileManager.h"

#include "IU_Macros.h"
#include "P_Projectile.h"
#include "M_GameMode.h"
#include "P_PoolingComponent.h"

AM_ProjectileManager::AM_ProjectileManager()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	poolingComponent = CreateDefaultSubobject<UP_PoolingComponent>("Pooling Component");
	AddOwnedComponent(poolingComponent);
}

void AM_ProjectileManager::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
}
void AM_ProjectileManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const float& _worldSpeed = fWorldSpeed * DeltaSeconds;
	onTickProjectile.Broadcast(_worldSpeed);
}

void AM_ProjectileManager::SpawnProjectile(const TEnumAsByte<EProjectileType>& _projectileType, const FProjectileSettings& _projectileSettings, const FVector& _location, const FRotator& _rotation)
{
	AP_Projectile* _projectile = poolingComponent->GetProjectile(_projectileType);
	if (!_projectile)
	{
		LOG("AM_ProjectileManager::SpawnProjectile => Projectile get from Pool is nullptr")
		return;
	}
	
	_projectile->SetProjectileManager(this);
	_projectile->SetActorLocationAndRotation(_location, _rotation);
	_projectile->InitializeProjectile(_projectileSettings);
	
	onTickProjectile.AddUniqueDynamic(_projectile, &AP_Projectile::TickProjectile);
}

void AM_ProjectileManager::SpawnProjectileDelayed(const float _delay, const TEnumAsByte<EProjectileType>& _projectileType, const FProjectileSettings& _projectileSettings, const FVector& _location, const FRotator& _rotation)
{
	AP_Projectile* _projectile = poolingComponent->GetProjectile(_projectileType);
	if (!_projectile)
	{
		LOG("AM_ProjectileManager::SpawnProjectile => Projectile get from Pool is nullptr")
		return;
	}
	
	_projectile->SetProjectileManager(this);
	_projectile->SetActorLocationAndRotation(_location, _rotation);
	_projectile->InitializeProjectile(_projectileSettings);
	_projectile->CatchUpDelayedTime(_delay);
	onTickProjectile.AddUniqueDynamic(_projectile, &AP_Projectile::TickProjectile);
}

void AM_ProjectileManager::ReturnProjectile(AP_Projectile* _projectile)
{
	if (!_projectile) return;
	
	onTickProjectile.RemoveDynamic(_projectile, &AP_Projectile::TickProjectile);
	poolingComponent->ReturnProjectile(_projectile);
}
void AM_ProjectileManager::ReturnAllProjectile()
{
	poolingComponent->PoolAllProjectile();
}

/* Old Pooling System (PullingComponent)

void AM_ProjectileManager::SpawnProjectile(const TSubclassOf<AP_Projectile>& type, const FVector& _location, const FRotator& _rotation, const FProjectileSettings& _settings, const FProjectileData& _projectileData)
{
	const TEnumAsByte<EProjectileType>& _projectileType = type.GetDefaultObject()->GetProjectileType();
	AP_Projectile* _projectile = GetProjectileByType(_projectileType);
	if (!_projectile)
	{
		_projectile = world->SpawnActor<AP_Projectile>(type, _location, _rotation);
	}

	if (!garbagedProjectiles.Contains(_projectileType))
	{
		garbagedProjectiles.Add(_projectileType, _projectileData);
	}
	
	UP_PullingComponent* _pulling = _projectileData.pulling;
	if (_projectile && _pulling)
	{
		_pulling->GetProjectiles().Add(_projectile);
		_projectile->SetActorLocationAndRotation(_location, _rotation);
		_projectile->InitializeProjectile(_settings);
		_projectile->SetPullingComponent(_pulling);
		_projectile->OnCollideNotWithProjectile().AddDynamic(this, &AM_ProjectileManager::GarbageDestroyedProjectile);
		onTickProjectile.AddDynamic(_projectile, &AP_Projectile::MovementBehavior);
	}
}



void AM_ProjectileManager::GarbageProjectile(AP_Projectile* _projectile, TArray<AP_Projectile*>& _projectiles)
{
	if (!_projectile) return;

	_projectiles.Remove(_projectile);
	onTickProjectile.RemoveDynamic(_projectile, &AP_Projectile::MovementBehavior);
	_projectile->OnCollideNotWithProjectile().RemoveDynamic(this, &AM_ProjectileManager::GarbageDestroyedProjectile);
	
	TArray<AP_Projectile*>& _garbagedProjectiles = garbagedProjectiles[_projectile->GetProjectileType()].projectiles;
	
	if (_garbagedProjectiles.Num() >= iStorageSize)
	{
		_projectile->Destroy();
		return;
	}

	_garbagedProjectiles.Add(_projectile);
	_projectile->DisableProjectile();
}


void AM_ProjectileManager::GarbageDestroyedProjectile(AP_Projectile* _projectile, UP_PullingComponent* _pulling)
{
	if (!_projectile || !_pulling) return;
	GarbageProjectile(_projectile, _pulling->GetProjectiles());
}

AP_Projectile* AM_ProjectileManager::GetProjectileByType(const EProjectileType& _projectileType)
{
	if (!garbagedProjectiles.Contains(_projectileType)) return nullptr;
	TArray<AP_Projectile*>& _garbagedProjectiles = garbagedProjectiles[_projectileType].projectiles;
	
	if (_garbagedProjectiles.Num() > 0)
	{
		AP_Projectile* _last = _garbagedProjectiles.Last();
		if (!_last) return nullptr;
		
		_garbagedProjectiles.Remove(_last);
		return _last;
	}

	return nullptr;
}
*/