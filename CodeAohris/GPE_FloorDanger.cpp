#include "GPE_FloorDanger.h"

#include "P_PB_Scaling.h"
#include "IU_Macros.h"
#include "P_DT_ZoneDataTable.h"
#include "IU_Player.h"
#include "M_GameMode.h"
#include "IU_HealthSystemComponent.h"
#include "IU_PlayerFSMBrain.h"
#include "IU_PlayerMovementComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AGPE_FloorDanger::AGPE_FloorDanger()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	scalingBehavior = CreateDefaultSubobject<UP_PB_Scaling>(TEXT("ScalingComponent"));
	
	AddOwnedComponent(scalingBehavior);
}

void AGPE_FloorDanger::InitializeGPEBehavior(UP_DT_ZoneDataTable* _zoneDataTable)
{
	Super::InitializeGPEBehavior(_zoneDataTable);
	ROG(!_zoneDataTable, "AGPE_FloorDanger::InitializeGPEBehavior -> DataTable invalid");

	ROG(!player, "AGPE_FloorDanger::InitializeGPEBehavior -> player invalid");

	playerHealth = player->HealthComponent();
	ROG(!playerHealth, "AGPE_FloorDanger::InitializeGPEBehavior -> playerHealth invalid");
	
	collider = Cast<UShapeComponent>(GetComponentByClass(colliderToUse));
	ROG(!collider, "AGPE_FloorDanger::InitializeGPEBehavior -> Invalid collider, set correctly the colliderToUse value and add the corresponding collider");

	collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	const FGPE_FloorDangerSettings& _settings = _zoneDataTable->GetGPEFloorDangerSettings();
	if (_zoneDataTable->GetScalingState())
		scalingBehavior->ActivateBehavior(_zoneDataTable);
	
	fDamage = _settings.fDamage;
	fPreviewTime = _settings.fPreviewTime;
	fPersistantTime = _settings.fPersistantTime;

	Event_AddUnique(onBehaviorDeltaTick, AGPE_FloorDanger::PreviewBehavior);
}

void AGPE_FloorDanger::InitializeGPEBehavior(FGPE_GlobalSettings& _patternData)
{
	Super::InitializeGPEBehavior(_patternData);
	ROG(!player, "AGPE_FloorDanger::InitializeGPEBehavior -> player invalid");

	playerHealth = player->HealthComponent();
	ROG(!playerHealth, "AGPE_FloorDanger::InitializeGPEBehavior -> playerHealth invalid");
	
	collider = Cast<UShapeComponent>(GetComponentByClass(colliderToUse));
	ROG(!collider, "AGPE_FloorDanger::InitializeGPEBehavior -> Invalid collider, set correctly the colliderToUse value and add the corresponding collider");
	
	collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	const FGPE_FloorDangerSettings& _settings = _patternData.floorDangerSettings;
	scalingBehavior->ActivateBehaviorScaling(_patternData.scalingSettings);
	
	fDamage = _settings.fDamage;
	fPreviewTime = _settings.fPreviewTime;
	fPersistantTime = _settings.fPersistantTime;

	Event_AddUnique(onBehaviorDeltaTick, AGPE_FloorDanger::PreviewBehavior);
	collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGPE_FloorDanger::BehaviorTick(const float& _deltaTime)
{
	onBehaviorDeltaTick.Broadcast(_deltaTime);
	scalingBehavior->TickBehavior(_deltaTime);
}

void AGPE_FloorDanger::PreviewBehavior(const float& _deltaTime)
{
	fCurrentTime += _deltaTime;
	if (fCurrentTime > fPreviewTime)
	{
		ActivateDamageBehavior();
	}
}

void AGPE_FloorDanger::PersistantBehavior(const float& _deltaTime)
{
	fCurrentTime += _deltaTime;
	if (fCurrentTime > fPersistantTime)
	{
		CompleteBehavior();
	}
}

void AGPE_FloorDanger::ActivateDamageBehavior()
{
	Event_AddUnique(OnActorBeginOverlap, AGPE_FloorDanger::StartCollideWithPlayer);
	Event_AddUnique(OnActorEndOverlap, AGPE_FloorDanger::EndCollideWithPlayer);
	
	fCurrentTime = 0;
	Event_Remove(onBehaviorDeltaTick, AGPE_FloorDanger::PreviewBehavior);
	collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Event_AddUnique(onBehaviorDeltaTick, AGPE_FloorDanger::PersistantBehavior);
}

void AGPE_FloorDanger::StartCollideWithPlayer(AActor* _mine, AActor* _other)
{
	if (_other == player)
	{
		Event_AddUnique(onBehaviorDeltaTick, AGPE_FloorDanger::DoDamageToPlayer);
		onStartCollideWithPlayer.Broadcast();
	}
}

void AGPE_FloorDanger::DoDamageToPlayer(const float& _deltaTime)
{
	playerHealth->TakeDamageFromZone(fDamage);
	const EStateType& _state = player->MovementFSM()->CurrentState()->StateType();
	if (_state == EStateType::Jumping || _state == EStateType::Falling || _state == EStateType::Dashing)return;
	
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

void AGPE_FloorDanger::EndCollideWithPlayer(AActor* _mine, AActor* _other)
{
	if (_other == player)
	{
		Event_Remove(onBehaviorDeltaTick, AGPE_FloorDanger::DoDamageToPlayer);
		onEndCollideWithPlayer.Broadcast();
	}
}