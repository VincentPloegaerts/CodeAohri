#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ENUM_BossPhase.h"
#include "ENUM_ProjectileType.h"

#include "P_Projectile.h"

#include "P_PoolingComponent.generated.h"

class AP_Projectile;

class AP_ProjectileDirectional;
class AP_ProjectileGuided;
class AP_ProjectileSimulated;
class AP_ProjectileBallistic;
class AP_ProjectileSinusoidal;

USTRUCT()
struct FProjectileContainer
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<AP_Projectile*> projectiles;
public:
	bool IsEmpty() const { return projectiles.Num() == 0; }
	bool IsNotEmpty() const { return projectiles.Num() > 0; }

	int Count() const { return projectiles.Num(); }
	AP_Projectile* GetProjectile()
	{
		AP_Projectile* _projectile = projectiles.Last();
		projectiles.Remove(_projectile);
		return _projectile;
	}
	void AddProjectile(AP_Projectile* _projectile)
	{
		projectiles.Add(_projectile);
	}
};

USTRUCT()
struct FSpawnTracking
{
	GENERATED_BODY()

	UPROPERTY()
	int iTotal = 0;
	UPROPERTY()
	int iInit = 0;
	UPROPERTY()
	int iGame = 0;
};

USTRUCT()
struct FPoolPhase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int iDirectionalQty = 0;
	UPROPERTY(EditAnywhere)
	int iGuidedQty = 0;
	UPROPERTY(EditAnywhere)
	int iSimulatedQty = 0;
	UPROPERTY(EditAnywhere)
	int iBallisticQty = 0;
	UPROPERTY(EditAnywhere)
	int iSinusoidalQty = 0;
	UPROPERTY(EditAnywhere)
	int iFatGuidedQty = 0;

	FPoolPhase() { }
	
	FPoolPhase(const int _directional, const int _guided, const int _simulated, const int _ballistic, const int _sinusoidal, const int _fatguided) :
	iDirectionalQty(_directional),
	iGuidedQty(_guided),
	iSimulatedQty(_simulated),
	iBallisticQty(_ballistic),
	iSinusoidalQty(_sinusoidal),
	iFatGuidedQty(_fatguided)
	{
		
	}
};

class AB_Boss;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UP_PoolingComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPoolAllProjectile);
	
	UPROPERTY(EditAnywhere, Category = "Pooling | Settings | Projectile")
	TSubclassOf<AP_ProjectileDirectional> projectileDirectionalType;
	UPROPERTY(EditAnywhere, Category = "Pooling | Settings | Projectile")
	TSubclassOf<AP_ProjectileGuided> projectileGuidedType;
	UPROPERTY(EditAnywhere, Category = "Pooling | Settings | Projectile")
	TSubclassOf<AP_ProjectileSimulated> projectileSimulatedType;
	UPROPERTY(EditAnywhere, Category = "Pooling | Settings | Projectile")
	TSubclassOf<AP_ProjectileBallistic> projectileBallisticType;
	UPROPERTY(EditAnywhere, Category = "Pooling | Settings | Projectile")
	TSubclassOf<AP_ProjectileSinusoidal> projectileSinusoidalType;
	UPROPERTY(EditAnywhere, Category = "Pooling | Settings | Projectile")
	TSubclassOf<AP_ProjectileGuided> projectileFatGuidedType;

	UPROPERTY(EditAnywhere, Category = "Pooling | Settings | Pool")
	TMap<TEnumAsByte<EBossPhase>, FPoolPhase> poolPhase =
	{
		{ EBossPhase::UN, FPoolPhase(500, 100, 100, 50, 0, 0) },
		{ EBossPhase::DEUX, FPoolPhase(1000, 100, 250, 50, 0, 0) },
		{ EBossPhase::TROIS, FPoolPhase(1000, 100, 250, 50, 0, 0) },
	};
	
	UPROPERTY(EditAnywhere, Category = "Pooling | Settings | Pool", meta = (UIMin = "1", UIMax = "50", ClampMin = "1", ClampMax = "50"))
	int iProjectileSpawnParse = 25;
	UPROPERTY(EditAnywhere, Category = "Pooling | Settings | Pool", meta = (UIMin = "0.3", UIMax = "1", ClampMin = "0.3", ClampMax = "1"))
	float fPoolGenerationRate = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Pooling | Settings | Pool")
	FVector vPoolLocation = FVector(0, 0, 10000);

	UPROPERTY(VisibleAnywhere, Category = "Pooling | Values")
	TEnumAsByte<EBossPhase> currentPhase = EBossPhase::UN;
	UPROPERTY(VisibleAnywhere, Category = "Pooling | Values")
	FPoolPhase currentPoolPhase = FPoolPhase();
	
	UPROPERTY()
	FTimerHandle poolGenerationTimer;
	UPROPERTY()
	int iCurrentSpawn = 0;
	UPROPERTY()
	TEnumAsByte<EProjectileType> currentProjectileType = EProjectileType::Directional;
	UPROPERTY()
	bool generationCompleted = false;
	
	UPROPERTY(EditAnywhere, Category = "Pooling | Values")
	unsigned uPlaySongOnProjectileEvery = 4;
	UPROPERTY(VisibleAnywhere, Category = "Pooling | Values")
	unsigned uCurrentPlaySongOnProjectileEvery = 1;
	
	UPROPERTY()
	TMap<TEnumAsByte<EProjectileType>, FProjectileContainer> pooledProjectiles =
	{
		{ EProjectileType::Directional, FProjectileContainer() },
		{ EProjectileType::Guided, FProjectileContainer() },
		{ EProjectileType::Simulated, FProjectileContainer() },
		{ EProjectileType::Ballistic, FProjectileContainer() },
		{ EProjectileType::Sinusoidal, FProjectileContainer() },		
		{ EProjectileType::FatGuided, FProjectileContainer() },		
	};
	
	UPROPERTY()
	UWorld* world = nullptr;
	
	FTimerManager* timerManager = nullptr;
	
	UPROPERTY()
	FOnPoolAllProjectile onPoolAllProjectile;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Pooling | Tracking")
	bool bTrackSpawn = true;
	
	UPROPERTY()
	TMap<TEnumAsByte<EProjectileType>, FSpawnTracking> projectileTrack =
	{
		{ EProjectileType::Directional, FSpawnTracking() },
		{ EProjectileType::Guided, FSpawnTracking() },
		{ EProjectileType::Simulated, FSpawnTracking() },
		{ EProjectileType::Ballistic, FSpawnTracking() },
		{ EProjectileType::Sinusoidal, FSpawnTracking() },
		{ EProjectileType::FatGuided, FSpawnTracking() },
	};
#endif
	
public:	
	UP_PoolingComponent();

	//Get a Projectile from Pool (or create a new one)
	AP_Projectile* GetProjectile(const TEnumAsByte<EProjectileType>& _projectileType);
	//Retrun a Projectile to the Pool
	void ReturnProjectile(AP_Projectile* _projectile);
	//Broadcast event to all projectile (Pool every projectile)
	void PoolAllProjectile();
	
private:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void InitializePooling();

	UFUNCTION() void UpdatePoolPhase(AB_Boss* _boss);
	void InitializePoolPhase();
	
	void InitializePool();
	UFUNCTION() void InitProjectilePool();
	//Spawn Projectile in pool and return if generation for Current Projectile Type is done
	bool InitializeProjectilePool(const TSubclassOf<AP_Projectile> _projectileType, const EProjectileType& _currentType, const EProjectileType& _nextType, const int& _qty);
	
	//Return if the Pool contains at least one projectile of the given type 
	bool HadProjectileAvailable(const EProjectileType& _type);

	//Pool Management : Add the given projectile to the Pool
	void AddProjectileToPool(AP_Projectile* _toPool);
	//Pool Management : Get a Projectile from the Pool 
	AP_Projectile* GetProjectileFromPool(const EProjectileType& _type);

	//Spawn a new Projectile
	AP_Projectile* SpawnProjectile(const TEnumAsByte<EProjectileType> _projectileType);

	TSubclassOf<AP_Projectile> GetProjectileFromType(const TEnumAsByte<EProjectileType> _projectileType) const;	
};
