#pragma once
#include "CoreMinimal.h"

#include "ENUM_ProjectileType.h"
#include "NiagaraSystem.h"
#include "STRUCT_ProjectileSettings.h"

#include "GameFramework/Actor.h"
#include "P_Projectile.generated.h"

class USphereComponent;
class AM_ProjectileManager;

USTRUCT()
struct FProjectileGlobalJSON
{
	GENERATED_BODY()
	
	UPROPERTY()
	FProjectileGlobalSettings globalSettings;
	
	UPROPERTY()
	TEnumAsByte<ECollisionEnabled::Type> projectileColliderEnable;
	UPROPERTY()
	TEnumAsByte<ECollisionChannel> projectileColliderType;
	UPROPERTY()
	FCollisionResponseContainer projectileColliderCollision;

	UPROPERTY()
	TEnumAsByte<ECollisionEnabled::Type> envColliderEnable;
	UPROPERTY()
	TEnumAsByte<ECollisionChannel> envColliderType;
	UPROPERTY()
	FCollisionResponseContainer envColliderCollision;
	
	UPROPERTY()
	UNiagaraSystem* initializeFX = nullptr;
	UPROPERTY()
	UNiagaraSystem* collideEnvironmentFX = nullptr;
	UPROPERTY()
	UNiagaraSystem* collidePlayerFX = nullptr;
	UPROPERTY()
	UNiagaraSystem* niagaraComponentFX = nullptr;
	
	void InitProjectileGlobalJSON(FProjectileGlobalJSON _json)
	{
		projectileColliderEnable = _json.projectileColliderEnable;
		projectileColliderType = _json.projectileColliderType;
		projectileColliderCollision = _json.projectileColliderCollision;
		envColliderEnable = _json.envColliderEnable;
		envColliderType = _json.envColliderType;
		envColliderCollision = _json.envColliderCollision;
		initializeFX = _json.initializeFX;
		collideEnvironmentFX = _json.collideEnvironmentFX;
		collidePlayerFX = _json.collidePlayerFX;
		niagaraComponentFX = _json.niagaraComponentFX;
		globalSettings = _json.globalSettings;
	}
};

UCLASS(Abstract, NotBlueprintable)
class INSIDEUS_API AP_Projectile : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileEnabled);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileEnabledWithSong);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileDisabled);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		UStaticMeshComponent* staticMesh = nullptr;

	//FX that is Activate/Deactivate when the Projectile is Initialized/Pooled
	UPROPERTY(VisibleAnywhere, Category = "Projectile | FX")
	UNiagaraComponent* niagaraComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		USphereComponent* projectileCollider = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		USphereComponent* envCollider = nullptr;

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
		FProjectileGlobalSettings globalSettings = FProjectileGlobalSettings();

	UPROPERTY(EditAnywhere, Category = "Projectile | Global")
		float lifeTime = 25.0f;
	
	//FX created when the Projectile is Initialized
	UPROPERTY(EditAnywhere, Category = "Projectile | FX")
		UNiagaraSystem* initializeFX = nullptr;
	//FX created when the Projectile collide with the Environment
	UPROPERTY(EditAnywhere, Category = "Projectile | FX")
		UNiagaraSystem* collideEnvironmentFX = nullptr;
	//FX created when the Projectile collide with the Player
	UPROPERTY(EditAnywhere, Category = "Projectile | FX")
		UNiagaraSystem* collidePlayerFX = nullptr;

	UPROPERTY(VisibleAnywhere)
	FKnockbackSettings knockbackSettings = FKnockbackSettings();
	
	UPROPERTY()
	float elapsedTime = 0.f;
	UPROPERTY()
	TEnumAsByte<EProjectileType> projectileType;
	UPROPERTY()
	FTimerHandle poolingTimer;
	
	UPROPERTY()
	AM_ProjectileManager* projectileManager = nullptr;

	//When the projectile is used in game
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Projectile Enabled"))
	FOnProjectileEnabled onProjectileEnabled;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Projectile Enabled With Song"))
	FOnProjectileEnabledWithSong onProjectileEnabledWithSong;
	
	//When the projectile is not used in game
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Projectile Disabled"))
	FOnProjectileDisabled onProjectileDisabled;
	
public:
	FORCEINLINE const FProjectileGlobalSettings& GetProjectileGlobalSettings() const { return globalSettings; }
	FORCEINLINE const TEnumAsByte<EProjectileType>& GetProjectileType() const { return projectileType; }
	
	FORCEINLINE void SetProjectileManager(AM_ProjectileManager* _manager) { projectileManager = _manager; };
	
public:	
	AP_Projectile();

protected:
	virtual void BeginPlay() override;
	
public:
#pragma region Pooling
	//Call by Pooling Component to Enable the projectile (Show Projectile & Enable Collision)
	virtual void EnableProjectile() const;
	//Call by Pooling Component to Enable the projectile (Show Projectile & Enable Collision)
	virtual void EnableProjectileWithSong() const;
	//Call by Pooling Component to Disable the projectile (Hide Projectile & Disable Collision)
	virtual void DisableProjectile() const;
	//Call by Projectile Manager to initialize projectile settings 
	virtual void InitializeProjectile(const FProjectileSettings& _settings);
	//Call by Pooling Component to reset Projectile variables
	virtual void ResetProjectile();
	//Call by Pooling Trigger when the projectile exit Trigger Collider (Projectile will be pooled back) or by Event (PoolingComponent)
	UFUNCTION() void PoolProjectile();
#pragma endregion
	
	//Movement Behavior of the Projectile
	UFUNCTION() void TickProjectile(const float& _deltaTime);

	void CatchUpDelayedTime(const float _delayedTime);
	
protected:
	virtual void MovementBehavior(const float& _deltaTime);
	
	UFUNCTION() void OnProjectileColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		                                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnEnvColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		                                  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

#pragma region FX
	void CreateFX(UNiagaraSystem* _system) const;
	void ActivateFX();
	void DeactivateFX();
#pragma endregion

#if WITH_EDITOR
public:
	virtual void SaveProjectile();
	virtual void LoadProjectile();

	FProjectileGlobalJSON GetProjectileGlobalJSON() const;
	void LoadProjectileGlobalJSON(const FProjectileGlobalJSON& _json);
#endif
};