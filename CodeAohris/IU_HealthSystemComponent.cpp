#include "IU_HealthSystemComponent.h"

#include "M_GameMode.h"
#include "M_UIManager.h"

#include "IU_Player.h"

UIU_HealthSystemComponent::UIU_HealthSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UIU_HealthSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	timerManager = &GetWorld()->GetTimerManager();
	player = Cast<AIU_Player>(GetOwner());
	
	fActualMaxHealth = fMaxHealth;
	if (!player)
		onDeath.AddDynamic(this, &UIU_HealthSystemComponent::ResetHealth);
}
void UIU_HealthSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCanRegen && bRegenState)
	{
		UpdateActualHealth(fRegenValue * DeltaTime);
	}
}

void UIU_HealthSystemComponent::TakeDamage(const float _damage)
{
	if (!bCanTakeDamages || !bCanDecreaseHealth || IsDead()) return;

	ResetRegenTimer();
	
	UpdateActualHealth(-_damage);
	onTakeDamage.Broadcast(_damage);

	FTimerHandle _takingHitAnimationTimer;
	onTakingHit.Broadcast(true);
	GetWorld()->GetTimerManager().SetTimer(_takingHitAnimationTimer,this,&UIU_HealthSystemComponent::UpdateTakingHitAnimation, 1,false);

	if (bCanRegen)
	{
		UpdateActualMaxHealth(-_damage * (fMaxLifeToRemove / 100.0f));
	}

	if (fInvicibilityDuration > 0.0f && fActualHealth > 0)
	{
		bCanTakeDamages = false;
		FTimerHandle _invincibleTimer;
		GetWorld()->GetTimerManager().SetTimer(_invincibleTimer, this, &UIU_HealthSystemComponent::ResetCanTakeDamages, fInvicibilityDuration, false);
	}
	
	onLifeUpdatedWithMaxLife.Broadcast(fActualHealth, fActualMaxHealth);
}

void UIU_HealthSystemComponent::TakeDamageFromProjectile(const float _damage)
{
	TakeDamage(_damage + fProjectileExtraDamage);
}

void UIU_HealthSystemComponent::TakeDamageFromZone(const float _damage)
{
	TakeDamage(_damage + fZoneExtraDamage);
}

void UIU_HealthSystemComponent::TakeFallDamage()
{
	TakeDamage(fFallDamage);
}

void UIU_HealthSystemComponent::TakeDamageWithImpactLocation(const float _damage, const bool _useCustomLocation, const FVector& _impactLocation)
{
	TakeDamage(_damage);

	if (!bCanTakeDamages)return;

	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if (AM_UIManager* _uiManager = _gameMode->GetUIManager())
		{
			_uiManager->ShowDamage(_damage, damageFloatingWidgetEnum, _useCustomLocation ? _impactLocation : GetOwner()->GetActorLocation());
		}
	}
}

void UIU_HealthSystemComponent::UpdateActualHealth(float _value)
{
	fActualHealth += _value;
	fActualHealth = fActualHealth > fActualMaxHealth ? fActualMaxHealth
				  : fActualHealth < 0.0f ? 0.0f
				  : fActualHealth;
	
	onLifeUpdated.Broadcast(fActualHealth, fMaxHealth);

	if (fActualHealth <= 0.0f)
	{
		FTimerHandle _deathTimer;
		GetWorld()->GetTimerManager().SetTimer(_deathTimer, this, &UIU_HealthSystemComponent::Death, 0.1f);
	}
}

void UIU_HealthSystemComponent::UpdateActualMaxHealth(float _value)
{
	fActualMaxHealth += _value;
	fActualMaxHealth = fActualMaxHealth > fMaxHealth ? fMaxHealth
					 : fActualMaxHealth < 0.0f ? 0.0f
					 : fActualMaxHealth;

	onActualMaxLifeUpdated.Broadcast(fActualMaxHealth, fMaxHealth);
	
	if (fActualMaxHealth < fActualHealth)
	{
		fActualHealth = fActualMaxHealth;
		onLifeUpdated.Broadcast(fActualHealth, fMaxHealth);
	}
}

void UIU_HealthSystemComponent::Death()
{
	onDeath.Broadcast();
}

void UIU_HealthSystemComponent::UpdateTakingHitAnimation()
{
	onTakingHit.Broadcast(false);
}

void UIU_HealthSystemComponent::ResetRegenTimer()
{
	bRegenState = false;
	if (fRegenCooldown > 0.0f)
	{
		FTimerDelegate _regenDelegate;
		_regenDelegate.BindUFunction(this, "SetRegenState", true);
		GetWorld()->GetTimerManager().ClearTimer(regenTimer);
		GetWorld()->GetTimerManager().SetTimer(regenTimer, _regenDelegate, fRegenCooldown, false);
	}
	else
		SetRegenState(true);
}

void UIU_HealthSystemComponent::InitializeHealth(float _extraLife)
{
	UpdateMaxHealth(_extraLife, true);
	ResetHealthForced();
}

void UIU_HealthSystemComponent::ResetHealth()
{
	if (bCanRegen)
	{
		UpdateActualMaxHealth(fMaxHealth);
	}
	
	UpdateActualHealth(fMaxHealth);
	onLifeUpdatedWithMaxLife.Broadcast(fActualHealth, fMaxHealth);
}

void UIU_HealthSystemComponent::ResetHealthForced()
{
	UpdateActualMaxHealth(fMaxHealth);
	UpdateActualHealth(fMaxHealth);
	onLifeUpdatedWithMaxLife.Broadcast(fActualHealth, fMaxHealth);
}

void UIU_HealthSystemComponent::UpdateMaxHealth(float _addValue, bool _regen)
{
	fMaxHealth += _addValue;
	if (_regen)
		ResetHealth();

	onMaxLifeUpdated.Broadcast(fActualMaxHealth, fMaxHealth);
}
