#include "P_PoolingTriggerComponent.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

#include "P_Projectile.h"

#include "IU_Macros.h"

UP_PoolingTriggerComponent::UP_PoolingTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP_PoolingTriggerComponent::EnablePoolingTrigger()
{
	EnablePrimitiveComponent(bIsSphereTrigger ? ownerSphereComponent : ownerBoxComponent);
}
void UP_PoolingTriggerComponent::DisablePoolingTrigger()
{
	DisablePrimitiveComponent(ownerBoxComponent);
	DisablePrimitiveComponent(ownerSphereComponent);
}

void UP_PoolingTriggerComponent::SetPoolingTriggerSphereRadius(const float& _size) const
{
	if (!bIsSphereTrigger || !ownerSphereComponent) return;
	if (USphereComponent* _sphere = Cast<USphereComponent>(ownerSphereComponent))
		_sphere->SetSphereRadius(_size);
}

void UP_PoolingTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializePoolingTrigger();
}

void UP_PoolingTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if WITH_EDITOR
	PoolingTriggerDebug();
#endif
}

void UP_PoolingTriggerComponent::InitializePoolingTrigger()
{
	InitializeTriggerOwner();
	DisablePoolingTrigger();
}
void UP_PoolingTriggerComponent::InitializeTriggerOwner()
{
	owner = GetOwner();
	if (!owner) return;
	
	owner->OnActorEndOverlap.AddDynamic(this, &UP_PoolingTriggerComponent::EndOverlapTrigger);

	ownerSphereComponent = Cast<UPrimitiveComponent>(owner->GetComponentByClass(USphereComponent::StaticClass()));
	ownerBoxComponent = Cast<UPrimitiveComponent>(owner->GetComponentByClass(UBoxComponent::StaticClass()));

	if (!ownerBoxComponent || !ownerSphereComponent)
	{
		LOG("UP_PoolingTriggerComponent::InitializeTriggerOwner => Box or Sphere Component missing.");
		return;
	}
}

void UP_PoolingTriggerComponent::EnablePrimitiveComponent(UPrimitiveComponent* _component) const
{
	if (!IsValid(_component)) return;
	
	_component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void UP_PoolingTriggerComponent::DisablePrimitiveComponent(UPrimitiveComponent* _component) const
{
	if (!IsValid(_component)) return;
	
	_component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UP_PoolingTriggerComponent::EndOverlapTrigger(AActor* _self, AActor* _other)
{
	if (AP_Projectile* _projectile = Cast<AP_Projectile>(_other))
		_projectile->PoolProjectile();
}

#if WITH_EDITOR
void UP_PoolingTriggerComponent::PoolingTriggerDebug()
{
	if (!bShowDebug) return;
	USphereComponent* _sphere = Cast<USphereComponent>(GetOwner()->GetComponentByClass(USphereComponent::StaticClass()));
	UBoxComponent* _box = Cast<UBoxComponent>(GetOwner()->GetComponentByClass(UBoxComponent::StaticClass()));

	if (_sphere && bIsSphereTrigger)
		DrawDebugSphere(GetWorld(), _sphere->GetComponentLocation(), _sphere->GetScaledSphereRadius(), 10, _sphere->GetCollisionEnabled() == ECollisionEnabled::QueryOnly ? FColor::Blue : FColor::Black);
	if (_box && !bIsSphereTrigger)
		DrawDebugBox(GetWorld(), _box->GetComponentLocation(), _box->GetScaledBoxExtent(), _box->GetCollisionEnabled() == ECollisionEnabled::QueryOnly ? FColor::Blue : FColor::Black);
}
#endif