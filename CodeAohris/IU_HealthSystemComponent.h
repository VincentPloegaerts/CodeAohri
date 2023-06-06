#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ENUM_HitFeedbackEnum.h"
#include "ExternalLifeBar.h"
#include "IU_Player.h"
#include "STRUCT_WidgetComponentSettings.h"

#include "IU_Macros.h"
#include "IU_HealthSystemComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UIU_HealthSystemComponent : public UActorComponent
{
	GENERATED_BODY()

	#pragma region Events
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakingHit, bool, _isTakingHit);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDamage, float, _damages);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLifeUpdated, float, _life, float, _maxLife);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxLifeUpdated, float, _actualMaxLife, float, _maxLife);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriesUpdated, int, _tries);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLifeUpdatedWithMaxLife, float, _life, float, _maxLife);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRegenEnabled);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

	#pragma endregion

	#pragma region Damages
	
	// Can the actor take damage
	UPROPERTY(EditAnywhere, Category = "Health values | Damages", BlueprintReadWrite, meta = (AllowPrivateAccess=True))
		bool bCanTakeDamages = true;

	// Invincibility time after taking a hit
	UPROPERTY(EditAnywhere, Category = "Health values | Damages", BlueprintReadWrite, meta = (AllowPrivateAccess=True))
		float fInvicibilityDuration = 10.0f;
	
	// Type of owner for the floatingwidget color
	UPROPERTY(EditAnywhere, Category = "Health values | Damages", BlueprintReadWrite, meta = (AllowPrivateAccess=True))
		TEnumAsByte<EHitFeedbackEnum> damageFloatingWidgetEnum = HIT_BOSS;

	UPROPERTY(EditAnywhere, Category = "Health values | Damages", BlueprintReadWrite, meta = (AllowPrivateAccess=True))
		float fProjectileExtraDamage = 0;
	
	UPROPERTY(EditAnywhere, Category = "Health values | Damages", BlueprintReadWrite, meta = (AllowPrivateAccess=True))
		float fZoneExtraDamage = 0;

	UPROPERTY(EditAnywhere, Category = "Health values | Damages", BlueprintReadWrite, meta = (AllowPrivateAccess=True))
		float fFallDamage = 0;

	#pragma endregion

	#pragma region Health
	
	// God mode
	UPROPERTY(VisibleAnywhere, Category = "Health values | Health", meta = (ClampMin = "0.0", ClampMax = "100000.0", BlueprintReadWrite, AllowPrivateAccess = True))
		bool bCanDecreaseHealth = true;

	// Actual Health calls onDeath when dropping to 0.0f
	UPROPERTY(EditAnywhere, Category = "Health values | Health", BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100000.0", AllowPrivateAccess = True))
		float fActualHealth = 100.0f;


	// Current max health, actualHealth can't go past this
	UPROPERTY(VisibleAnywhere, Category = "Health values | Health", meta = (BlueprintReadWrite, AllowPrivateAccess = True))
		float fActualMaxHealth = 0.0f;
	
	// Max of max health, baseMaxHealth can't go past this
	UPROPERTY(EditAnywhere, Category = "Health values | Health", meta = (ClampMin = "0.0", ClampMax = "100000.0", BlueprintReadWrite, AllowPrivateAccess = True))
		float fMaxHealth = 100.0f;

	#pragma endregion

	#pragma region Regen

	// Status of regen of max life
	UPROPERTY(EditAnywhere, Category = "Health values | Regen")
		bool bCanRegen = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Health values | Regen")
		bool bRegenState = false;
	
	// Percent of Maxlife to remove per damage 
	UPROPERTY(EditAnywhere, Category = "Health values | Regen", meta = (UIMin = "0.0", UIMax = "100.0", ClampMin = "0.0", ClampMax = "100.0"))
		float fMaxLifeToRemove = 50.0f;

	// Cooldown before regen max life again
	UPROPERTY(EditAnywhere, Category = "Health values | Regen", meta = (UIMin = "0.0", UIMax = "60.0", ClampMin = "0.0", ClampMax = "60.0"))
		float fRegenCooldown = 5.0f;

	// Max life to regen for each regen
	UPROPERTY(EditAnywhere, Category = "Health values | Regen", meta = (UIMin = "0.01", UIMax = "100.0", ClampMin = "0.01", ClampMax = "100.0"))
		float fRegenValue = 0.5f;

	UPROPERTY()
		FTimerHandle regenTimer = FTimerHandle();

	#pragma endregion

	UPROPERTY()
		AIU_Player* player = nullptr;

	FTimerManager* timerManager = nullptr;
		
protected:
	#pragma region Events

	// Called when the player take damage
	// ONLY FOR ANIMATION
	// Params = Animation status
	UPROPERTY()
		FOnTakingHit onTakingHit = FOnTakingHit();
	
	// Called when player loose health
	// Params = Damage value
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
		FOnTakeDamage onTakeDamage = FOnTakeDamage();

	// Called when player life changed
	// Params = Actual health
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
		FOnLifeUpdated onLifeUpdated = FOnLifeUpdated();

	// Called when player increase his max health
	// Params = Max health
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
		FOnMaxLifeUpdated onActualMaxLifeUpdated = FOnMaxLifeUpdated();
	// Called when player increase his max health
	// Params = Max health Added
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
		FOnMaxLifeUpdated onMaxLifeUpdated = FOnMaxLifeUpdated();

	// Called when player life changed
	// Params = Actual health and max health
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
		FOnLifeUpdatedWithMaxLife onLifeUpdatedWithMaxLife = FOnLifeUpdatedWithMaxLife();
	
	// Called when the player can regen
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
		FOnRegenEnabled onRegenEnabled = FOnRegenEnabled();

	// Called when actualHealth drops to 0
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
		FOnDeath onDeath = FOnDeath();

	// Called when currentTries change
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
		FOnTriesUpdated onTriesUpdated = FOnTriesUpdated();

	#pragma endregion
	
public:	
	UIU_HealthSystemComponent();

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Methods
	
public:
	#pragma region Get
	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsDead() const { return fActualHealth <= 0.0f; }
	FORCEINLINE float GetHealth() const { return fActualHealth; }
	FORCEINLINE float GetActualMaxHealth() const { return fActualMaxHealth; }
	FORCEINLINE float GetMaxHealth() const { return fMaxHealth; }
	FORCEINLINE float GetHealthPercent() const { return fActualHealth / fActualMaxHealth; }

	FORCEINLINE FOnTakingHit& OnTakingHit() { return onTakingHit; }
	FORCEINLINE FOnTakeDamage* OnTakeDamage() { return &onTakeDamage; }
	FORCEINLINE FOnLifeUpdated* OnLifeUpdated() { return &onLifeUpdated; }
	FORCEINLINE FOnMaxLifeUpdated* OnActualMaxLifeUpdated() { return &onActualMaxLifeUpdated; }
	FORCEINLINE FOnMaxLifeUpdated* OnMaxLifeUpdated() { return &onMaxLifeUpdated; }
	FORCEINLINE FOnLifeUpdatedWithMaxLife* OnLifeUpdatedWithMaxLife() { return &onLifeUpdatedWithMaxLife; }
	FORCEINLINE FOnRegenEnabled* OnRegenEnabled() { return &onRegenEnabled; }
	FORCEINLINE FOnDeath* OnDeath() { return &onDeath; }
	FORCEINLINE FOnTriesUpdated* OnTriesUpdated() { return &onTriesUpdated; }
	#pragma endregion

	#pragma region Set
	UFUNCTION(BlueprintCallable, Exec) FORCEINLINE void ToggleGodMode() { bCanDecreaseHealth = !bCanDecreaseHealth; }

	FORCEINLINE void SetMaxHealth(const float _maxHealth)
	{
		fMaxHealth = _maxHealth;
		fActualMaxHealth = _maxHealth;
	}
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetRegenState(bool _state) { bRegenState = _state; }
	
	FORCEINLINE void SetCanRegen(bool _status) { bCanRegen = _status; }
	FORCEINLINE void SetRegenDatas(float _regenCooldown, float _regenValue, float _maxLifeToRemove)
	{
		fRegenCooldown = _regenCooldown;
		fRegenValue = _regenValue;
		fMaxLifeToRemove = _maxLifeToRemove;
	}
	
	FORCEINLINE void SetDamageFromProjectile(float _projectileExtraDamage) { fProjectileExtraDamage = _projectileExtraDamage; }
	FORCEINLINE void SetDamageFromZone(float _zoneExtraDamage) { fZoneExtraDamage = _zoneExtraDamage; }
	FORCEINLINE void SetFallDamage(float _fallDamage) { fFallDamage = _fallDamage; }

	UFUNCTION(BlueprintCallable) FORCEINLINE void SetCanTakeDamages(bool _status) { bCanTakeDamages = _status; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void ResetCanTakeDamages() { bCanTakeDamages = true; }
	
	FORCEINLINE void SetInvincibleDuration(const float _invincibilityDuration) { fInvicibilityDuration = _invincibilityDuration; }
	FORCEINLINE void SetDamageFloatingEnum(const EHitFeedbackEnum& _enum) { damageFloatingWidgetEnum = _enum; }
	#pragma endregion

	#pragma region Methods

private:	
	/* Call this only to heal this component, damage are managed by "TakeDamage" */
	UFUNCTION(BlueprintCallable) void UpdateActualHealth(float _value);
	
	/* Modify maxHealth value, but doesn't regen Health. Call RegainHealth if needed*/
	UFUNCTION(BlueprintCallable) void UpdateActualMaxHealth(float _value);

	/* Start to death after a timer */
	UFUNCTION(BlueprintCallable) void Death();

	/* To pass the taking hit bool of animation to false */
	UFUNCTION(BlueprintCallable) void UpdateTakingHitAnimation();

	// reset regen timer (on taking damage)
	UFUNCTION(BlueprintCallable) void ResetRegenTimer();
	
public:
	UFUNCTION(BlueprintCallable) void InitializeHealth(float _extraLife);
	
	UFUNCTION(BlueprintCallable) void ResetHealth();
	//Reset Health careless of Statement (can regen, etc)
	UFUNCTION(BlueprintCallable) void ResetHealthForced();
	
	UFUNCTION(BlueprintCallable) void UpdateMaxHealth(float _addValue,bool _regen = true);
	
	/* Call this only to deal damage to this component, heals are managed by "RegainHealth" */
	UFUNCTION(BlueprintCallable) void TakeDamage(const float _damage);

	// Take damage scaled with difficulty for Projectile
	UFUNCTION(BlueprintCallable) void TakeDamageFromProjectile(const float _damage);
	
	// Take damage scaled with difficulty for Zone (Laser, GPE Zone etc...)
	UFUNCTION(BlueprintCallable) void TakeDamageFromZone(const float _damage);

	// Take Fall Damage scaled with difficulty (Kill Zone)
	UFUNCTION(BlueprintCallable) void TakeFallDamage();
	
	/* Call this only to deal damage to this component, heals are managed by "RegainHealth" with impact location */
	UFUNCTION(BlueprintCallable) void TakeDamageWithImpactLocation(const float _damage, const bool _useCustomLocation, const FVector& _impactLocation = FVector::ZeroVector);

	#pragma endregion 
};
