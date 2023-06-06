#pragma once

#include "CoreMinimal.h"
#include "GPE_Zone.h"
#include "Components/SphereComponent.h"
#include "GPE_Projectile.generated.h"

UCLASS(Blueprintable)
class INSIDEUS_API AGPE_Projectile : public AGPE_Zone
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBehaviorDeltaTick, const float&, _deltaTime);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollideWithPlayer);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* staticMesh = nullptr;

	UPROPERTY(EditAnywhere)
	USphereComponent* sphereCollider = nullptr;

	//Amount of damage dealt to player.
	UPROPERTY()
	float fDamage = 1.0f;

	//Collider move speed.
	UPROPERTY()
	float fMoveSpeed = 100.0f;

	//Preview showing time.
	UPROPERTY()
	float fPreviewTime = 2.0f;

	//Current showing time.
	UPROPERTY()
	float fCurrentPreviewTime = 0.0f;

	//The sphereCollider goal location.
	UPROPERTY()
	FVector vColliderGoal = FVector(0, 0, 500);

	//The current location of the collider.
	UPROPERTY()
	FVector vCurrentColliderLocation = FVector::ZeroVector;
	
	UPROPERTY()
	FOnBehaviorDeltaTick onBehaviorDeltaTick;
	
protected:
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnCollideWithPlayer onCollideWithPlayer;
	
public:
	AGPE_Projectile();
	
	virtual void InitializeGPEBehavior(UP_DT_ZoneDataTable* _zoneDataTable) override;
	virtual void InitializeGPEBehavior(FGPE_GlobalSettings& _patternData) override;
	virtual void BehaviorTick(const float& _deltaTime) override;

private:
	//Show the preview.
	UFUNCTION() void PreviewBehavior(const float& _deltaTime);

	//Move the collider until reach goal.
	UFUNCTION() void MoveCollider(const float& _deltaTime);

	//When preview end, setup collider and visual.
	void ActivateDamageBehavior() const;

	//Do damage to the player.
	UFUNCTION() void DoDamageToPlayer(AActor* _mine, AActor* _other);
};