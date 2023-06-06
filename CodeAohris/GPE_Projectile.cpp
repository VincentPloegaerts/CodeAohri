#include "GPE_Projectile.h"

#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "IU_PlayerMovementComponent.h"
#include "P_DT_ZoneDataTable.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AGPE_Projectile::AGPE_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	
	sphereCollider->SetupAttachment(RootComponent);
	staticMesh->SetupAttachment(sphereCollider);

	sphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	staticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	staticMesh->SetGenerateOverlapEvents(false);
	staticMesh->SetVisibility(false);
}

void AGPE_Projectile::InitializeGPEBehavior(UP_DT_ZoneDataTable* _zoneDataTable)
{
	Super::InitializeGPEBehavior(_zoneDataTable);
	ROG(!_zoneDataTable, "AGPE_Projectile::InitializeGPEBehavior -> DataTable invalid");

	const FGPE_ProjectileSettings& _settings = _zoneDataTable->GetGPEProjectileSettings();

	fDamage = _settings.fDamage;
	fMoveSpeed = _settings.fMoveSpeed;
	fPreviewTime = _settings.fPreviewTime;
	
	if (_settings.bMoveUp)
	{
		vColliderGoal.Z = _settings.fZOffset;
	}
	else
	{
		vCurrentColliderLocation.Z = _settings.fZOffset;
		sphereCollider->SetRelativeLocation(vCurrentColliderLocation);
	}

	Event_AddUnique(onBehaviorDeltaTick, AGPE_Projectile::PreviewBehavior);
	sphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGPE_Projectile::InitializeGPEBehavior(FGPE_GlobalSettings& _patternData)
{
	Super::InitializeGPEBehavior(_patternData);
	const FGPE_ProjectileSettings& _settings = _patternData.projectileSettings;

	fDamage = _settings.fDamage;
	fMoveSpeed = _settings.fMoveSpeed;
	fPreviewTime = _settings.fPreviewTime;
	
	if (_settings.bMoveUp)
	{
		vColliderGoal.Z = _settings.fZOffset;
	}
	else
	{
		vCurrentColliderLocation.Z = _settings.fZOffset;
		sphereCollider->SetRelativeLocation(vCurrentColliderLocation);
	}

	Event_AddUnique(onBehaviorDeltaTick, AGPE_Projectile::PreviewBehavior);
}

void AGPE_Projectile::BehaviorTick(const float& _deltaTime)
{
	onBehaviorDeltaTick.Broadcast(_deltaTime);
}

void AGPE_Projectile::PreviewBehavior(const float& _deltaTime)
{
	fCurrentPreviewTime += _deltaTime;
	if (fCurrentPreviewTime > fPreviewTime)
	{
		Event_Remove(onBehaviorDeltaTick, AGPE_Projectile::PreviewBehavior);
		
		ActivateDamageBehavior();
		Event_AddUnique(OnActorBeginOverlap, AGPE_Projectile::DoDamageToPlayer);
		Event_AddUnique(onBehaviorDeltaTick, AGPE_Projectile::MoveCollider);
	}
}

void AGPE_Projectile::MoveCollider(const float& _deltaTime)
{
	vCurrentColliderLocation = Get_VLerpConst(vCurrentColliderLocation, vColliderGoal, _deltaTime, fMoveSpeed);
	sphereCollider->SetRelativeLocation(vCurrentColliderLocation);

	if (FVector::Dist(vCurrentColliderLocation, vColliderGoal) < 0.01f)
	{
		CompleteBehavior();
	}
}

void AGPE_Projectile::ActivateDamageBehavior() const
{
	staticMesh->SetVisibility(true);
	sphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGPE_Projectile::DoDamageToPlayer(AActor* _mine, AActor* _other)
{
	if (player == _other)
	{
		if (UIU_HealthSystemComponent* _healthComp = player->HealthComponent())
		{
			Event_Remove(OnActorBeginOverlap, AGPE_Projectile::DoDamageToPlayer);
			
			_healthComp->TakeDamage(fDamage);

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
			
			sphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			onCollideWithPlayer.Broadcast();
		}
	}
}