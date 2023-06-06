#include "P_PB_Laser.h"

#include "B_Boss.h"
#include "P_DT_LaserDataTable.h"
#include "P_PatternLaser.h"
#include "IU_Player.h"
#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"
#include "M_BlackBoardManager.h"
#include "M_GameMode.h"
#include "M_WorldTimeManager.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Kismet/KismetSystemLibrary.h"

void UP_PB_Laser::ActivateBehavior(UP_DT_BaseDataTable* _dataTable)
{
	Super::ActivateBehavior(_dataTable);
	world = GetWorld();
	
	const UP_DT_LaserDataTable* _laserDataTable = Cast<UP_DT_LaserDataTable>(_dataTable);
	if (!_laserDataTable) return;

	InitializeLaser(_laserDataTable);
}

void UP_PB_Laser::ActivateBehavior(FPatternData& _patternData)
{
	Super::ActivateBehavior(_patternData);
	world = GetWorld();
	
	if (_patternData.GetLaserState())
		laserSettings = _patternData.GetLaserSettings();

	fElapsedTime = 0;

	if (const AM_GameMode* _gamemode = world->GetAuthGameMode<AM_GameMode>())
	{
		player = _gamemode->GetPlayer();
		
		if (laserSettings.bLookPlayerOnEndFeedback)
		{
			const FRotator& _ownerRotation = owner->GetActorRotation();
			FRotator _rotation = Get_LookRotation(player->GetActorLocation() - owner->GetActorLocation());
			_rotation.Roll = _ownerRotation.Roll;
			if (!laserSettings.bLookPitch)
				_rotation.Pitch = _ownerRotation.Pitch;
			owner->SetActorRotation(_rotation);
		}
	}
	
	if (AP_PatternLaser* _pattern = Cast<AP_PatternLaser>(owner))
	{
		_pattern->InitializeLaserActor(laserSettings.fLaserLength, laserSettings.fLaserRadius, laserSettings.laserPivot);
		_pattern->OnActorBeginOverlap.AddDynamic(this, &UP_PB_Laser::LaserBeginOverlap);
		_pattern->OnActorEndOverlap.AddDynamic(this, &UP_PB_Laser::LaserEndOverlap);
		
		_pattern->OnComplete().AddDynamic(this, &UP_PB_Laser::DisableLaserBehavior);

		USceneComponent* _root = _pattern->GetRootComponent();
		if (fxLayer.Num() < 1)
			LOG_ERROR("UP_PB_Laser::InitializeLaser -> The FX Layer is empty");
		ROG_ERROR(!_root || !fxHitEnvironment || !fxHitPlayer, "UP_PB_Laser::InitializeLaser -> There is an invalid FX");
		
		niagaraEnvironment = SpawnNiagaraSystemOnSceneComponentNoDestroy(fxHitEnvironment, _root);
		niagaraPlayer = SpawnNiagaraSystemOnSceneComponentNoDestroy(fxHitPlayer, _root);
		niagaraPlayer->Deactivate();
	}
}

void UP_PB_Laser::InitializeLaser(const UP_DT_LaserDataTable* _dataTable)
{	
	if (_dataTable->GetLaserState())
		laserSettings = _dataTable->GetLaserSettings();

	fElapsedTime = 0;

	if (const AM_GameMode* _gamemode = world->GetAuthGameMode<AM_GameMode>())
	{
		player = _gamemode->GetPlayer();
		blackBoardManager = _gamemode->GetBlackBoardManager();
		worldTimeManager = _gamemode ->GetWorldTimeManager();
		
		if (laserSettings.bDetachLaserFromSocket)
		{
			socketParentName = owner->GetAttachParentSocketName();
			AActor* _bossOwner = owner->GetAttachParentActor();
		
			if (const AB_Boss* _boss = Cast<AB_Boss>(_bossOwner))
			{
				bossSkeletal = _boss->GetSkeletalMeshComponent();
				if (bossSkeletal)
				{
					if (bossSkeletal->DoesSocketExist(socketParentName))
						owner->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					else
						LOG_ERROR("UP_PB_Laser::InitializeLaser -> Laser is not attached to a socket");
				}
			}
		}
		
		if (laserSettings.bLookPlayerOnEndFeedback)
		{
			const FRotator& _ownerRotation = owner->GetActorRotation();
			FRotator _rotation = Get_LookRotation(player->GetActorLocation() - owner->GetActorLocation());
			_rotation.Roll = _ownerRotation.Roll;
			if (!laserSettings.bLookPitch)
				_rotation.Pitch = _ownerRotation.Pitch;
			owner->SetActorRotation(_rotation + laserSettings.rLookOffset);
		}
	}
	
	if (AP_PatternLaser* _pattern = Cast<AP_PatternLaser>(owner))
	{
		_pattern->SetSocketParentName(socketParentName);
		_pattern->InitializeLaserActor(laserSettings.fLaserLength, laserSettings.fLaserRadius, laserSettings.laserPivot);
		Event_Add(_pattern->OnActorBeginOverlap, UP_PB_Laser::LaserBeginOverlap);
		Event_Add(_pattern->OnActorEndOverlap, UP_PB_Laser::LaserEndOverlap);
		Event_Add(_pattern->OnComplete(), UP_PB_Laser::DisableLaserBehavior);

		USceneComponent* _root = _pattern->GetRootComponent();
		if (fxLayer.Num() < 1)
			LOG_ERROR("UP_PB_Laser::InitializeLaser -> The FX Layer is empty");
		ROG_ERROR(!_root || !fxHitEnvironment || !fxHitPlayer, "UP_PB_Laser::InitializeLaser -> There is an invalid FX");
		
		niagaraEnvironment = SpawnNiagaraSystemOnSceneComponentNoDestroy(fxHitEnvironment, _root);
		niagaraPlayer = SpawnNiagaraSystemOnSceneComponentNoDestroy(fxHitPlayer, _root);
		niagaraPlayer->Deactivate();
	}
}

void UP_PB_Laser::TickBehavior(const float& _deltaTime)
{
	const float& _speed = worldTimeManager->GetCurrentWorldSpeed() * _deltaTime;
	Super::TickBehavior(_speed);
	if (!bIsEnable) return;

	blackBoardManager->UPDATE_DEBUG();

	if (laserSettings.bKeepFollowSocketLocation && laserSettings.bDetachLaserFromSocket)
	{
		if (owner && bossSkeletal && bossSkeletal->DoesSocketExist(socketParentName))
			owner->SetActorLocation(bossSkeletal->GetSocketLocation(socketParentName));
	}
	
	fElapsedTime += _speed;
	if (fElapsedTime > laserSettings.fLaserDuration)
	{
		bIsEnable = false;
		onBehaviorComplete.Broadcast();
	}

	if (bIsEnable && bPlayerPresence && healthComponent)
		healthComponent->TakeDamageFromZone(laserSettings.fLaserDamage);

	RaycastLaser();
}

void UP_PB_Laser::LaserBeginOverlap(AActor* _owner, AActor* _other)
{
	if (player == _other)
	{
		healthComponent = player->HealthComponent();
		bPlayerPresence = true;
	}
}
void UP_PB_Laser::LaserEndOverlap(AActor* _owner, AActor* _other)
{
	if (player == _other)
		bPlayerPresence = false;
}

void UP_PB_Laser::DisableLaserBehavior(AP_PatternBase* _pattern)
{
	bIsEnable = false;
}

void UP_PB_Laser::RaycastLaser()
{
	if (!niagaraEnvironment || !niagaraPlayer || fxLayer.Num() < 1)return;
	
	const FVector& _location = owner->GetActorLocation();
	
	FHitResult _hitResult;
	if (Get_LineTrace(world, _location, _location + owner->GetActorForwardVector() * fFXLaserDistance, fxLayer, _hitResult))
	{
		const FVector& _hitLocation = _hitResult.Location;
		if (player == _hitResult.GetActor())
		{
			if (bRaycastPlayer)
			{
				niagaraPlayer->SetWorldLocation(_hitLocation);
			}
			else
			{
				bRaycastPlayer = true;				
				niagaraEnvironment->Deactivate();
				niagaraPlayer->Activate();
			}
		}
		else
		{
			if (bRaycastPlayer)
			{
				bRaycastPlayer = false;
				niagaraEnvironment->Activate();
				niagaraPlayer->Deactivate();
			}
			else
			{
				niagaraEnvironment->SetWorldLocation(_hitLocation);
			}
		}
	}
	else
	{
		niagaraEnvironment->Deactivate();
		niagaraPlayer->Deactivate();
	}
}