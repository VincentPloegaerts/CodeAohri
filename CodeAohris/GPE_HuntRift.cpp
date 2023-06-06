#include "GPE_HuntRift.h"

#include "IU_Player.h"
#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"
#include "IU_PlayerMovementComponent.h"
#include "M_GameMode.h"
#include "P_DT_ZoneDataTable.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AGPE_HuntRift::AGPE_HuntRift()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	sphereCollider->SetupAttachment(RootComponent);
}

void AGPE_HuntRift::InitializeGPEBehavior(UP_DT_ZoneDataTable* _zoneDataTable)
{
	Super::InitializeGPEBehavior(_zoneDataTable);
	ROG(!_zoneDataTable, "AGPE_HuntRift::InitializeGPEBehavior -> DataTable invalid");

	ROG(!player, "AGPE_HuntRift::InitializeGPEBehavior -> player invalid");

	playerHealth = player->HealthComponent();
	ROG(!playerHealth, "AGPE_HuntRift::InitializeGPEBehavior -> playerHealth invalid");
	
	const FGPE_HuntRiftSettings& _settings = _zoneDataTable->GetGPEHuntRiftSettings();
	
	fDamage = _settings.fDamage;
	fMoveSpeed = _settings.fMoveSpeed;
	fFollowDuration = _settings.fFollowDuration;

	fZSpawnHeight = GetActorLocation().Z;

	Event_AddUnique(OnActorBeginOverlap, AGPE_HuntRift::StartCollideWithPlayer);
	Event_AddUnique(OnActorEndOverlap, AGPE_HuntRift::EndCollideWithPlayer);
	Event_AddUnique(onBehaviorDeltaTick, AGPE_HuntRift::MoveToPlayer);
}

void AGPE_HuntRift::InitializeGPEBehavior(FGPE_GlobalSettings& _patternData)
{
	Super::InitializeGPEBehavior(_patternData);
	ROG(!player, "AGPE_HuntRift::InitializeGPEBehavior -> player invalid");

	playerHealth = player->HealthComponent();
	ROG(!playerHealth, "AGPE_HuntRift::InitializeGPEBehavior -> playerHealth invalid");

	const FGPE_HuntRiftSettings& _settings = _patternData.huntRiftSettings;

	fDamage = _settings.fDamage;
	fMoveSpeed = _settings.fMoveSpeed;
	fFollowDuration = _settings.fFollowDuration;

	fZSpawnHeight = GetActorLocation().Z;

	Event_AddUnique(OnActorBeginOverlap, AGPE_HuntRift::StartCollideWithPlayer);
	Event_AddUnique(OnActorEndOverlap, AGPE_HuntRift::EndCollideWithPlayer);
	Event_AddUnique(onBehaviorDeltaTick, AGPE_HuntRift::MoveToPlayer);
}

void AGPE_HuntRift::BehaviorTick(const float& _deltaTime)
{
	onBehaviorDeltaTick.Broadcast(_deltaTime);
}

void AGPE_HuntRift::MoveToPlayer(const float& _deltaTime)
{
	fCurrentFollowDuration += _deltaTime;

	FVector _playerLocation(player->GetActorLocation());
	FVector _huntRiftLocation(GetActorLocation());
	_playerLocation.Z = 0;
	_huntRiftLocation.Z = 0;

	SetActorRotation(Get_LookRotation(_playerLocation - _huntRiftLocation));
	
	_playerLocation.Z = fZSpawnHeight;

	SetActorLocation(Get_VLerpConst(GetActorLocation(), _playerLocation, _deltaTime, fMoveSpeed));
	
	if (fCurrentFollowDuration > fFollowDuration)
	{
		CompleteBehavior();
	}
}

void AGPE_HuntRift::StartCollideWithPlayer(AActor* _mine, AActor* _other)
{
	if (_other == player)
	{
		Event_AddUnique(onBehaviorDeltaTick, AGPE_HuntRift::DoDamageToPlayer);
		onStartCollideWithPlayer.Broadcast();
	}
}

void AGPE_HuntRift::DoDamageToPlayer(const float& _deltaTime)
{
	playerHealth->TakeDamageFromZone(fDamage);
	
	if (knockbackSettings.bActivateKnockBack)
	{
		FVector _origin, _boxExtent;
		player->GetActorBounds(true, _origin, _boxExtent);

		TArray<AActor*> _actorToIgnore, _outActor;
		_actorToIgnore.Add(this);
		
		if (UIU_PlayerMovementComponent* _playerMovement = player->MovementComponent())
		{
			FVector _direction(player->GetActorLocation() - GetActorLocation());
			_direction.Z = 0.f;
			_direction = _direction.GetSafeNormal(UE_SMALL_NUMBER, FVector::ForwardVector);
			_direction *= knockbackSettings.fKnockBackStrenght;

			const FVector& _baseGoalLocation = _direction * knockbackSettings.fKnockBackDuration * 100.0f;
			
			if (UKismetSystemLibrary::BoxOverlapActors(world, _origin + _baseGoalLocation, _boxExtent, otherFloorLayer, AGPE_Zone::StaticClass(), _actorToIgnore, _outActor))
			{
				bool _isValid = false;
				for (size_t i = 0; i < PASTIS; ++i)
				{
					const float& _rad = FMath::DegreesToRadians(FMath::RandRange(0.f, 360.f));
					_direction.X = cos(_rad) * knockbackSettings.fKnockBackStrenght * knockbackSettings.fKnockBackDuration;
					_direction.Y = sin(_rad) * knockbackSettings.fKnockBackStrenght * knockbackSettings.fKnockBackDuration;
					
					if (!UKismetSystemLibrary::BoxOverlapActors(world, _origin + _direction * 100.f, _boxExtent, otherFloorLayer, AGPE_Zone::StaticClass(), _actorToIgnore, _outActor))
					{
						_isValid = true;
						break;
					}
				}

				if (!_isValid)
				{
					_direction = player->GetActorLocation() - GetActorLocation();
					_direction.Z = 0.f;
					_direction = _direction.GetSafeNormal(UE_SMALL_NUMBER, FVector::ForwardVector);
					_direction *= knockbackSettings.fKnockBackStrenght;
				}
			}

			
			_playerMovement->StartKnockBack(_direction, knockbackSettings.fKnockBackDuration);
		}
	}
}

void AGPE_HuntRift::EndCollideWithPlayer(AActor* _mine, AActor* _other)
{
	if (_other == player)
	{
		Event_Remove(onBehaviorDeltaTick, AGPE_HuntRift::DoDamageToPlayer);
		onEndCollideWithPlayer.Broadcast();
	}
}