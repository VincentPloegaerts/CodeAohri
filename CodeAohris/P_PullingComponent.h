#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P_PullingComponent.generated.h"

class AM_ProjectileManager;
class AP_Projectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UP_PullingComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
		FTimerHandle checkTimer = FTimerHandle();

	UPROPERTY(EditAnywhere, Category = "Pulling values")
		bool bUseBoxToCheck = false;

	UPROPERTY(EditAnywhere, Category = "Pulling values | Gizmos")
		bool bDrawGizmos = false;
	
	UPROPERTY(EditAnywhere, Category = "Pulling values | Gizmos", meta = (UIMin = "3", UIMax = "100", ClampMin = "3", ClampMax = "100"))
		int iArenaSegments = 8;

	UPROPERTY(VisibleAnywhere, Category = "Pulling values")
		int iCurrentCheckIndex = 0;
	
	UPROPERTY(EditAnywhere, Category = "Pulling values", meta = (UIMin = "1", UIMax = "1000", ClampMin = "1", ClampMax = "1000"))
		int iCheckCount = 20;
	
	UPROPERTY(EditAnywhere, Category = "Pulling values", meta = (UIMin = "0.01", UIMax = "60.0", ClampMin = "0.01", ClampMax = "60.0"))
		float fCheckRate = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Pulling values", meta = (UIMin = "0.01", UIMax = "5.0", ClampMin = "0.01", ClampMax = "5.0"))
		float fRandFirstDelay = 0.25f;
	
	UPROPERTY(EditAnywhere, Category = "Pulling values", meta = (UIMin = "0.0", UIMax = "10000.0", ClampMin = "0.0", ClampMax = "10000.0"), meta = (EditCondition = "!bUseBoxToCheck"))
		float fArenaRadius = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Pulling values")
		FVector offset = FVector(0.0f);
		
	UPROPERTY(EditAnywhere, Category = "Pulling values", meta = (EditCondition = "bUseBoxToCheck"))
    	FVector arenaExtent = FVector(100.0f);

	UPROPERTY(EditAnywhere, Category = "Pulling values | Gizmos")
		FColor arenaColor = FColor::Blue;

	UPROPERTY(VisibleAnywhere, Category = "Pulling values")
		TArray<AP_Projectile*> projectiles = TArray<AP_Projectile*>();
	
	UPROPERTY()
		UWorld* world = nullptr;
	
	UPROPERTY()
		AM_ProjectileManager* projectileManager = nullptr;

private:
	FORCEINLINE FVector GetOwnerLocation() const { return GetOwner()->GetActorLocation() + offset; }
public:
	FORCEINLINE TArray<AP_Projectile*>& GetProjectiles() { return projectiles; }
	FORCEINLINE void SetSize(const float& _radius)
	{
		fArenaRadius = _radius;
		arenaExtent.X = _radius;
		arenaExtent.Y = _radius;
		arenaExtent.Z = _radius;
	}
	
public:	
	UP_PullingComponent();

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitManagers();

	void DrawArena() const;
	UFUNCTION(BlueprintCallable) void CheckProjectiles();
	bool IsIntoArena(const AP_Projectile* _projectile) const;

public:
	void GarbageAllProjectiles();
};