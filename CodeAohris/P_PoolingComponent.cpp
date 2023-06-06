#include "P_PoolingComponent.h"

#include "M_GameMode.h"

#include "M_BossManager.h"
#include "B_Boss.h"

#include "P_Projectile.h"
#include "P_ProjectileDirectional.h"
#include "P_ProjectileGuided.h"
#include "P_ProjectileSimulated.h"
#include "P_ProjectileBallistic.h"
#include "P_ProjectileSinusoidal.h"

#include "IU_Macros.h"

UP_PoolingComponent::UP_PoolingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UP_PoolingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePooling();
}
void UP_PoolingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

#if WITH_EDITOR
	if (!bTrackSpawn) return;
	
	LOG("**************************POOLING**************************");
	
	UE_LOG(LogTemp, Warning, TEXT("									TOTAL	INIT	GAME"));
	UE_LOG(LogTemp, Warning, TEXT("Projectile DIRECTIONAL spawned :	%d		%d		%d"), projectileTrack[EProjectileType::Directional].iTotal, projectileTrack[EProjectileType::Directional].iInit, projectileTrack[EProjectileType::Directional].iGame);
	UE_LOG(LogTemp, Warning, TEXT("Projectile GUIDED spawned :			%d		%d		%d"), projectileTrack[EProjectileType::Guided].iTotal, projectileTrack[EProjectileType::Guided].iInit, projectileTrack[EProjectileType::Guided].iGame);
	UE_LOG(LogTemp, Warning, TEXT("Projectile SIMULATED spawned :		%d		%d		%d"), projectileTrack[EProjectileType::Simulated].iTotal, projectileTrack[EProjectileType::Simulated].iInit, projectileTrack[EProjectileType::Simulated].iGame);
	UE_LOG(LogTemp, Warning, TEXT("Projectile BALLISTIC spawned :		%d		%d		%d"), projectileTrack[EProjectileType::Ballistic].iTotal, projectileTrack[EProjectileType::Ballistic].iInit, projectileTrack[EProjectileType::Ballistic].iGame);
	UE_LOG(LogTemp, Warning, TEXT("Projectile SINUSOIDAL spawned :		%d		%d		%d"), projectileTrack[EProjectileType::Sinusoidal].iTotal, projectileTrack[EProjectileType::Sinusoidal].iInit, projectileTrack[EProjectileType::Sinusoidal].iGame);
	UE_LOG(LogTemp, Warning, TEXT("Projectile FARGUIDED spawned :		%d		%d		%d"), projectileTrack[EProjectileType::FatGuided].iTotal, projectileTrack[EProjectileType::FatGuided].iInit, projectileTrack[EProjectileType::FatGuided].iGame);
	
	LOG("***********************************************************");
#endif
}

void UP_PoolingComponent::InitializePooling()
{
	world = GetWorld();
	timerManager = &world->GetTimerManager();

	if (AM_GameMode* _gm =GetWorld()->GetAuthGameMode<AM_GameMode>())
		if (AM_BossManager* _bossManager = _gm->GetBossManager())
			_bossManager->OnBossChanged().AddUniqueDynamic(this, &UP_PoolingComponent::UpdatePoolPhase);
}

void UP_PoolingComponent::UpdatePoolPhase(AB_Boss* _boss)
{
	currentPhase = _boss->GetBossPhase();
	if (poolPhase.Contains(currentPhase))
	{
		currentPoolPhase = poolPhase[currentPhase];
		InitializePoolPhase();
	}
	else
		currentPoolPhase = FPoolPhase();

	LOG("Begin Pool Generation for Boss Phase : %s", *UEnum::GetValueAsString(currentPhase));
	currentProjectileType = EProjectileType::Directional;
	iCurrentSpawn = 0;
	generationCompleted = false;
	if (timerManager)
	{
		timerManager->ClearTimer(poolGenerationTimer);
		timerManager->SetTimer(poolGenerationTimer, this, &UP_PoolingComponent::InitializePool, fPoolGenerationRate, true, -1);
	}
}
void UP_PoolingComponent::InitializePoolPhase()
{
	if (pooledProjectiles.Contains(EProjectileType::Directional))
	{
		const int _requiredDirectional = currentPoolPhase.iDirectionalQty - pooledProjectiles[EProjectileType::Directional].Count();
		currentPoolPhase.iDirectionalQty = _requiredDirectional < 0 ? 0 : _requiredDirectional;
	}

	if (pooledProjectiles.Contains(EProjectileType::Guided))
	{
		const int _requiredGuided = currentPoolPhase.iGuidedQty - pooledProjectiles[EProjectileType::Guided].Count();
		currentPoolPhase.iGuidedQty = _requiredGuided < 0 ? 0 : _requiredGuided;
	}
	
	if (pooledProjectiles.Contains(EProjectileType::Simulated))
	{
		const int _requiredSimulated = currentPoolPhase.iSimulatedQty - pooledProjectiles[EProjectileType::Simulated].Count();
		currentPoolPhase.iSimulatedQty = _requiredSimulated < 0 ? 0 : _requiredSimulated;
	}

	if (pooledProjectiles.Contains(EProjectileType::Ballistic))
	{
		const int _requiredBallistic = currentPoolPhase.iBallisticQty - pooledProjectiles[EProjectileType::Ballistic].Count();
		currentPoolPhase.iBallisticQty = _requiredBallistic < 0 ? 0 : _requiredBallistic;
	}

	if (pooledProjectiles.Contains(EProjectileType::Sinusoidal))
	{
		const int _requiredSinusoidal = currentPoolPhase.iSinusoidalQty - pooledProjectiles[EProjectileType::Sinusoidal].Count();
		currentPoolPhase.iSinusoidalQty = _requiredSinusoidal < 0 ? 0 : _requiredSinusoidal;
	}

	if (pooledProjectiles.Contains(EProjectileType::FatGuided))
	{
		const int _requiredFatGuided = currentPoolPhase.iFatGuidedQty - pooledProjectiles[EProjectileType::FatGuided].Count();
		currentPoolPhase.iFatGuidedQty = _requiredFatGuided < 0 ? 0 : _requiredFatGuided;
	}
}

void UP_PoolingComponent::InitializePool()
{
	if (timerManager)
	{
		for (int i = 0; i < iProjectileSpawnParse; i++)
		{
			FTimerHandle _timer;
			timerManager->SetTimer(_timer, this, &UP_PoolingComponent::InitProjectilePool, FMath::RandRange(0.1f, 0.03f), false);
		}
	}
}

void UP_PoolingComponent::InitProjectilePool()
{
	if (generationCompleted) return;
	
	iCurrentSpawn++;

	if (InitializeProjectilePool(projectileDirectionalType, EProjectileType::Directional, EProjectileType::Guided, currentPoolPhase.iDirectionalQty))
		return;
	if (InitializeProjectilePool(projectileGuidedType, EProjectileType::Guided, EProjectileType::Simulated, currentPoolPhase.iGuidedQty))
		return;
	if (InitializeProjectilePool(projectileSimulatedType, EProjectileType::Simulated, EProjectileType::Ballistic, currentPoolPhase.iSimulatedQty))
		return;
	if (InitializeProjectilePool(projectileBallisticType, EProjectileType::Ballistic, EProjectileType::Sinusoidal, currentPoolPhase.iBallisticQty))
		return;
	if (InitializeProjectilePool(projectileSinusoidalType, EProjectileType::Sinusoidal, EProjectileType::FatGuided, currentPoolPhase.iSinusoidalQty))
		return;
	if (InitializeProjectilePool(projectileFatGuidedType, EProjectileType::FatGuided, EProjectileType::FatGuided, currentPoolPhase.iFatGuidedQty))
	{
		generationCompleted = true;
		LOG("Pool Generation for Boss Phase %s finished.", *UEnum::GetValueAsString(currentPhase))
		if (timerManager) timerManager->ClearTimer(poolGenerationTimer);
	}
}

bool UP_PoolingComponent::InitializeProjectilePool(const TSubclassOf<AP_Projectile> _projectileType, const EProjectileType& _currentType, const EProjectileType& _nextType, const int& _qty)
{
	if (currentProjectileType == _currentType)
	{
		if (iCurrentSpawn > _qty)
		{
			iCurrentSpawn = 0;
			currentProjectileType = _nextType;
			return true;
		}
		
		if (_projectileType.Get())
		{
			ReturnProjectile(SpawnProjectile(_currentType));

			#if WITH_EDITOR
			projectileTrack[_currentType].iInit++;
			#endif

			return false;
		}
		//else
		iCurrentSpawn = 0;	
		currentProjectileType = _nextType;
		return true;
	}

	return false;
}

bool UP_PoolingComponent::HadProjectileAvailable(const EProjectileType& _type)
{
	return pooledProjectiles[_type].IsNotEmpty();
}

AP_Projectile* UP_PoolingComponent::GetProjectile(const TEnumAsByte<EProjectileType>& _projectileType)
{
	AP_Projectile* _projectile = nullptr;
	
	if (HadProjectileAvailable(_projectileType))
		_projectile = GetProjectileFromPool(_projectileType);
	else
	{
		_projectile = SpawnProjectile(_projectileType);
		#if WITH_EDITOR
		projectileTrack[_projectileType].iGame++;
		#endif		
	}

	if (!_projectile)
	{
		LOG("UP_PoolingComponent::GetProjectile => Projectile (Get or Spawn) is nullptr");	
		return nullptr;
	}
	
	onPoolAllProjectile.AddUniqueDynamic(_projectile, &AP_Projectile::PoolProjectile);	
	_projectile->ResetProjectile();

	++uCurrentPlaySongOnProjectileEvery;
	if (uPlaySongOnProjectileEvery <= uCurrentPlaySongOnProjectileEvery)
	{
		_projectile->EnableProjectileWithSong();
		uCurrentPlaySongOnProjectileEvery = 0;
	}
	else
		_projectile->EnableProjectile();
	
	return _projectile;
}
void UP_PoolingComponent::ReturnProjectile(AP_Projectile* _projectile)
{
	if (!_projectile) return;
	onPoolAllProjectile.RemoveDynamic(_projectile, &AP_Projectile::PoolProjectile);
	AddProjectileToPool(_projectile);
	_projectile->DisableProjectile();
	_projectile->SetActorLocationAndRotation(vPoolLocation, FRotator(0));
}
void UP_PoolingComponent::PoolAllProjectile()
{
	onPoolAllProjectile.Broadcast();
}

void UP_PoolingComponent::AddProjectileToPool(AP_Projectile* _toPool)
{
	if (!_toPool) return;
	const EProjectileType& _type = _toPool->GetProjectileType();
	pooledProjectiles[_type].AddProjectile(_toPool);
}
AP_Projectile* UP_PoolingComponent::GetProjectileFromPool(const EProjectileType& _type)
{
	if (pooledProjectiles[_type].IsEmpty())
		return nullptr;

	AP_Projectile* _projectile = pooledProjectiles[_type].GetProjectile();
	return _projectile;	
}

AP_Projectile* UP_PoolingComponent::SpawnProjectile(TEnumAsByte<EProjectileType> __projectileType)
{
	const TSubclassOf<AP_Projectile>& _type = GetProjectileFromType(__projectileType);
	if (!_type.Get())
	{
		LOG("UP_PoolingComponent::SpawnProjectile => projectile BP type is not valid.");
		return nullptr;
	}
	
	AP_Projectile* _projectile = world->SpawnActor<AP_Projectile>(_type, vPoolLocation, FRotator(0));

#if WITH_EDITOR
	if (_projectile)
		projectileTrack[__projectileType].iTotal++;
#endif
	
	return _projectile;
}

TSubclassOf<AP_Projectile> UP_PoolingComponent::GetProjectileFromType(const TEnumAsByte<EProjectileType> _projectileType) const
{
	if (_projectileType == EProjectileType::Directional)
		return projectileDirectionalType;
	if (_projectileType == EProjectileType::Guided)
		return projectileGuidedType;
	if (_projectileType == EProjectileType::Simulated)
		return projectileSimulatedType;
	if (_projectileType == EProjectileType::Ballistic)
		return projectileBallisticType;
	if (_projectileType == EProjectileType::Sinusoidal)
		return projectileSinusoidalType;
	if (_projectileType == EProjectileType::FatGuided)
		return projectileFatGuidedType;

	LOG("Pooling Component GetProjectileFromType => Projectile Type unknown")
	return projectileDirectionalType;
}
