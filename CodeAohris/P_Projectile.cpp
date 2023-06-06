#include "P_Projectile.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"
#include "M_ProjectileManager.h"
#include "P_PoolingTrigger.h"
#include "IU_Player.h"
#include "IU_PlayerMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AP_Projectile::AP_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	staticMesh->SetupAttachment(RootComponent);
	
	niagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Niagara");
	niagaraComponent->SetupAttachment(RootComponent);
	
	projectileCollider = CreateDefaultSubobject<USphereComponent>("Projectile Collider");
	projectileCollider->SetupAttachment(RootComponent);

	envCollider = CreateDefaultSubobject<USphereComponent>("Env Collider");
	envCollider->SetupAttachment(RootComponent);
	
	if (staticMesh)
	{
		const ConstructorHelpers::FObjectFinder<UStaticMesh> _staticMesh(TEXT("/Game/Graph/03_FX/04_Meshes/SM_Low_Sphere.SM_Low_Sphere"));
		if (_staticMesh.Succeeded()) staticMesh->SetStaticMesh(_staticMesh.Object);
	}
}
void AP_Projectile::BeginPlay()
{
	Super::BeginPlay();
	projectileCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AP_Projectile::OnProjectileColliderOverlap);
	envCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AP_Projectile::OnEnvColliderOverlap);
	//OnActorBeginOverlap.AddDynamic(this,&AP_Projectile::OnProjectileOverlap);
}

void AP_Projectile::EnableProjectile() const
{
	RootComponent->SetVisibility(true, true);
	niagaraComponent->Activate();
	projectileCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	envCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	onProjectileEnabled.Broadcast();
}

void AP_Projectile::EnableProjectileWithSong() const
{
	RootComponent->SetVisibility(true, true);
	niagaraComponent->Activate();
	projectileCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	envCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	onProjectileEnabledWithSong.Broadcast();
}

void AP_Projectile::DisableProjectile() const
{
	RootComponent->SetVisibility(false, true);
	niagaraComponent->Deactivate();
	projectileCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	envCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	onProjectileDisabled.Broadcast();
}
void AP_Projectile::InitializeProjectile(const FProjectileSettings& _settings)
{
	globalSettings = _settings.globalSettings;
	if (_settings.bEditProjectileKnockbackSettings)
	{
		knockbackSettings = _settings.projectileKnockbackSettings;
	}
	elapsedTime = 0;

	CreateFX(initializeFX);
	ActivateFX();
}
void AP_Projectile::ResetProjectile()
{
	//Reset Projectile variables... see declaration in Child Class
}

void AP_Projectile::PoolProjectile()
{
	DeactivateFX();
	
	if (!projectileManager)	return;
	
	projectileManager->ReturnProjectile(this);	
}

void AP_Projectile::TickProjectile(const float& _deltaTime)
{
	MovementBehavior(_deltaTime);
	
	elapsedTime += _deltaTime;
	if (elapsedTime > lifeTime)
		PoolProjectile();
}

void AP_Projectile::CatchUpDelayedTime(const float _delayedTime)
{
	TickProjectile(_delayedTime);
}

void AP_Projectile::MovementBehavior(const float& _deltaTime)
{
	//Declaration in Child Class
}

void AP_Projectile::OnProjectileColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Player
	if (const AIU_Player* _player = Cast<AIU_Player>(OtherActor))
	{
		if (UIU_HealthSystemComponent* _health = _player->HealthComponent())
		{
			_health->TakeDamageFromProjectile(globalSettings.fDamage);
			CreateFX(collidePlayerFX);
		}
		
		if (knockbackSettings.bActivateKnockBack)
		{
			if (UIU_PlayerMovementComponent* _playerMovement = _player->MovementComponent())
			{
				//Try to calculate where the projectile come from
				const FVector& _projectileLastLocation = GetActorLocation() - GetActorForwardVector() * 100.0f;
				FVector _direction(_player->GetActorLocation() - _projectileLastLocation);
				_direction.Z = 0.f;
				_direction = _direction.GetSafeNormal(UE_SMALL_NUMBER, FVector::ForwardVector);
				_direction *= knockbackSettings.fKnockBackStrenght;
				_playerMovement->StartKnockBack(_direction, knockbackSettings.fKnockBackDuration);
			}
		}

		PoolProjectile();
	}
}
void AP_Projectile::OnEnvColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Wall/Ground
	if (Cast<AP_PoolingTrigger>(OtherActor))return;
	
	CreateFX(collideEnvironmentFX);
	PoolProjectile();
}

void AP_Projectile::CreateFX(UNiagaraSystem* _system) const
{
	if (!_system) return;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), _system, GetActorLocation(), GetActorRotation(), staticMesh->GetComponentScale());
}
void AP_Projectile::ActivateFX()
{
	if (!niagaraComponent) return;
	niagaraComponent->Activate(true);
}
void AP_Projectile::DeactivateFX()
{
	if (!niagaraComponent) return;
	niagaraComponent->Deactivate();	
}

#if WITH_EDITOR
void AP_Projectile::SaveProjectile()
{
	//Child Class
}
void AP_Projectile::LoadProjectile()
{
	//Child Class
}

FProjectileGlobalJSON AP_Projectile::GetProjectileGlobalJSON() const
{
	FProjectileGlobalJSON _json;
	_json.globalSettings = globalSettings;
	_json.projectileColliderEnable = projectileCollider->GetCollisionEnabled();
	_json.projectileColliderType = projectileCollider->GetCollisionObjectType();
	_json.projectileColliderCollision = projectileCollider->GetCollisionResponseToChannels();
	_json.envColliderEnable = envCollider->GetCollisionEnabled();
	_json.envColliderType = envCollider->GetCollisionObjectType();
	_json.envColliderCollision = envCollider->GetCollisionResponseToChannels();
	_json.initializeFX = initializeFX;
	_json.collideEnvironmentFX = collideEnvironmentFX;
	_json.collidePlayerFX = collidePlayerFX;
	_json.niagaraComponentFX = niagaraComponent->GetAsset();
	return _json;
}

void AP_Projectile::LoadProjectileGlobalJSON(const FProjectileGlobalJSON& _json)
{
	globalSettings = _json.globalSettings;
	projectileCollider->SetCollisionEnabled(_json.projectileColliderEnable);
	projectileCollider->SetCollisionObjectType(_json.projectileColliderType);
	projectileCollider->SetCollisionResponseToChannels(_json.projectileColliderCollision);
	envCollider->SetCollisionEnabled(_json.envColliderEnable);
	envCollider->SetCollisionObjectType(_json.envColliderType);
	envCollider->SetCollisionResponseToChannels(_json.envColliderCollision);
	initializeFX = _json.initializeFX;
	collideEnvironmentFX = _json.collideEnvironmentFX;
	collidePlayerFX = _json.collidePlayerFX;
	niagaraComponent->SetAsset(_json.niagaraComponentFX);
}
#endif