#pragma once
#include "CoreMinimal.h"

#include "P_Projectile.h"
#include "STRUCT_ProjectileSettings.h"

#include "GameFramework/Actor.h"
#include "M_ProjectileManager.generated.h"

class UP_PoolingComponent;

UCLASS()
class INSIDEUS_API AM_ProjectileManager : public AActor
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickProjectile, const float&, _deltaTime);
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UP_PoolingComponent* poolingComponent = nullptr;

	//Used for projectile tick speed
	UPROPERTY()
		float fWorldSpeed = 1.0f;
	
	//Used to tick projectiles
	UPROPERTY(EditAnywhere)
		FOnTickProjectile onTickProjectile = FOnTickProjectile();
	
	UPROPERTY()
		UWorld* world = nullptr;

public:
	FORCEINLINE void SetWorldSpeed(const float& _worldSpeed) { fWorldSpeed = _worldSpeed; }
	FORCEINLINE UP_PoolingComponent* GetPoolingComponent() const { return poolingComponent; }
	
public:
	AM_ProjectileManager();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	//Call to use a projectile from PoolingComponent (Spawn or Get an unused one)
	UFUNCTION() void SpawnProjectile(const TEnumAsByte<EProjectileType>& _projectileType, const FProjectileSettings& _projectileSettings, const FVector& _location, const FRotator& _rotation);
	//
	UFUNCTION() void SpawnProjectileDelayed(const float _delay, const TEnumAsByte<EProjectileType>& _projectileType, const FProjectileSettings& _projectileSettings, const FVector& _location, const FRotator& _rotation);
	//Return the given projectile to PoolingComponent
	void ReturnProjectile(AP_Projectile* _projectile);

	//Call to return all projectile to PoolingComponent (can be an Event)
	UFUNCTION() void ReturnAllProjectile();
};

/*	Old PoolingSystem (PullingComponent)

	// Size max of the projectiles storage
	UPROPERTY(EditAnywhere, Category = "Projectile Manager values", meta = (UIMin = "0", UIMax = "10000", ClampMin = "0", ClampMax = "10000"))
		int iStorageSize = 250;

	// All projectiles garbaged 
	UPROPERTY(EditAnywhere, Category = "Projectile Manager values")
		TMap<TEnumAsByte<EProjectileType>, FProjectileData> garbagedProjectiles = TMap<TEnumAsByte<EProjectileType>, FProjectileData>();

	UFUNCTION() void GarbageProjectile(AP_Projectile* _projectile, TArray<AP_Projectile*>& _projectiles);
	UFUNCTION() void GarbageDestroyedProjectile(AP_Projectile* _projectile, UP_PullingComponent* _pulling);

	AP_Projectile* GetProjectileByType(const EProjectileType& _projectileType);
*/