#include "P_ProjectileGuided.h"

#include "M_GameMode.h"
#include "IU_Player.h"
#include "M_SaveManager.h"
#include "NiagaraComponent.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

AP_ProjectileGuided::AP_ProjectileGuided()
{
	if (staticMesh) staticMesh->SetStaticMesh(nullptr);
	projectileCollider->SetSphereRadius(0);
	projectileCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	projectileGuidedCollider = CreateDefaultSubobject<UCapsuleComponent>("Collider Guided");
	projectileGuidedCollider->SetupAttachment(RootComponent);
	projectileGuidedCollider->SetRelativeRotation(FRotator(90.0f, 0, 0));
	
	projectileType = Guided;
}

void AP_ProjectileGuided::EnableProjectile() const
{
	Super::EnableProjectile();
	projectileCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	projectileGuidedCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AP_ProjectileGuided::DisableProjectile() const
{
	Super::DisableProjectile();
	projectileGuidedCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AP_ProjectileGuided::BeginPlay()
{
	Super::BeginPlay();
	projectileGuidedCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AP_ProjectileGuided::OnProjectileColliderOverlap);
}

void AP_ProjectileGuided::InitializeProjectile(const FProjectileSettings& _settings)
{
	Super::InitializeProjectile(_settings);
	
	guidedSettings = _settings.guidedSettings;

	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		target = _gameMode->GetPlayer();
	}
	
	fSpeed = globalSettings.fProjectileSpeed;
}

void AP_ProjectileGuided::ResetProjectile()
{
	Super::ResetProjectile();
	
	fSpeed = 0.0f;

	target = nullptr;
	bAtTarget = false;
	bRotate = false;
	fELapsedTime = 0.0f;
}

void AP_ProjectileGuided::MovementBehavior(const float& _deltaTime)
{
	CheckWaitTime(_deltaTime);
	
	AddActorWorldOffset(GetActorForwardVector() * fSpeed * _deltaTime);
	RotateToTarget(_deltaTime);
}

void AP_ProjectileGuided::RotateToTarget(const float& _deltaTime)
{
	if (!target || bAtTarget || !bRotate) return;
	
	const FRotator& _rotationTarget = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation());
	const FRotator& _rotation = UKismetMathLibrary::RInterpTo_Constant(GetActorRotation(), _rotationTarget, _deltaTime, guidedSettings.fRotationSpeed);
	//const FRotator& _rotationTarget = Get_LookRotation(target->GetActorLocation());
	//const FRotator& _rotation = Get_RLerpConst(GetActorRotation(), _rotationTarget, _deltaTime, fRotationSpeed);

	SetActorRotation(_rotation);

	if (IsAtTarget())
	{
		bAtTarget = true;
	}
}

bool AP_ProjectileGuided::IsAtTarget() const
{
	if (!target) return false;
	return FVector::Dist(GetActorLocation(), target->GetActorLocation()) < guidedSettings.fRangeGuided;
}

void AP_ProjectileGuided::CheckWaitTime(const float& _deltaTime)
{
	if (bRotate) return;
	
	fELapsedTime+= _deltaTime;
	if (fELapsedTime > guidedSettings.fWaitTime)
	{
		fELapsedTime = 0.0f;
		bRotate = true;
		fSpeed = guidedSettings.fProjectileGuidedSpeed;
	}
}

#if WITH_EDITOR
void AP_ProjectileGuided::SaveProjectile()
{
	FProjectileGuidedJSON _json = GetProjectileGuidedJSON();
	
	UM_SaveManager::Save(_json, Save_Projectile, GetName());
}

void AP_ProjectileGuided::LoadProjectile()
{
	FProjectileGuidedJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Projectile, GetName());
	if (!_loadSuccess) return;
	
	LoadProjectileGuidedJSON(_json);
}

FProjectileGuidedJSON AP_ProjectileGuided::GetProjectileGuidedJSON() const
{
	FProjectileGuidedJSON _json;
	FProjectileGlobalJSON _jsonGlobal = GetProjectileGlobalJSON();
	_jsonGlobal.projectileColliderEnable = projectileGuidedCollider->GetCollisionEnabled();
	_jsonGlobal.projectileColliderType = projectileGuidedCollider->GetCollisionObjectType();
	_jsonGlobal.projectileColliderCollision = projectileGuidedCollider->GetCollisionResponseToChannels();
	_json.InitProjectileGlobalJSON(_jsonGlobal);
	
	_json.guidedSettings = guidedSettings;
	return _json;
}

void AP_ProjectileGuided::LoadProjectileGuidedJSON(const FProjectileGuidedJSON& _json)
{
	globalSettings = _json.globalSettings;
	projectileGuidedCollider->SetCollisionEnabled(_json.projectileColliderEnable);
	projectileGuidedCollider->SetCollisionObjectType(_json.projectileColliderType);
	projectileGuidedCollider->SetCollisionResponseToChannels(_json.projectileColliderCollision);
	envCollider->SetCollisionEnabled(_json.envColliderEnable);
	envCollider->SetCollisionObjectType(_json.envColliderType);
	envCollider->SetCollisionResponseToChannels(_json.envColliderCollision);
	initializeFX = _json.initializeFX;
	collideEnvironmentFX = _json.collideEnvironmentFX;
	collidePlayerFX = _json.collidePlayerFX;
	niagaraComponent->SetAsset(_json.niagaraComponentFX);
	
	guidedSettings = _json.guidedSettings;
}
#endif