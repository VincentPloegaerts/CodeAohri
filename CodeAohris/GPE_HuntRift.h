#pragma once

#include "CoreMinimal.h"
#include "GPE_Zone.h"
#include "GPE_HuntRift.generated.h"

class AIU_Player;
class UIU_HealthSystemComponent;
class USphereComponent;

UCLASS(Blueprintable)
class INSIDEUS_API AGPE_HuntRift : public AGPE_Zone
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBehaviorDeltaTick, const float&, _deltaTime);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartCollideWithPlayer);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndCollideWithPlayer);

	//The collider
	UPROPERTY(EditAnywhere)
	USphereComponent* sphereCollider = nullptr;

	//Health component of the player.
	UPROPERTY()
	UIU_HealthSystemComponent* playerHealth = nullptr;

	//The Z location on start.
	UPROPERTY()
	float fZSpawnHeight = 0.0f;
	
	//The damage dealt by the collider.
	UPROPERTY()
	float fDamage = 0.0f;

	//The move speed.
	UPROPERTY()
	float fMoveSpeed = 0.0f;

	//The follow player duration. When reach, call OnCompleteBehavior().
	UPROPERTY()
	float fFollowDuration = 0.0f;

	//Current time.
	UPROPERTY()
	float fCurrentFollowDuration = 0.0f;
	
	UPROPERTY()
	FOnBehaviorDeltaTick onBehaviorDeltaTick;

protected:
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnStartCollideWithPlayer onStartCollideWithPlayer;
	
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnEndCollideWithPlayer onEndCollideWithPlayer;
	
public:
	AGPE_HuntRift();
	
	virtual void InitializeGPEBehavior(UP_DT_ZoneDataTable* _zoneDataTable) override;
	virtual void InitializeGPEBehavior(FGPE_GlobalSettings& _patternData) override;
	virtual void BehaviorTick(const float& _deltaTime) override;

private:
	//Move constantly to the player direction.
	UFUNCTION() void MoveToPlayer(const float& _deltaTime);
	
#pragma region PlayerDamage
	UFUNCTION() void StartCollideWithPlayer(AActor* _mine, AActor* _other);
	UFUNCTION() void DoDamageToPlayer(const float& _deltaTime);
	UFUNCTION() void EndCollideWithPlayer(AActor* _mine, AActor* _other);
#pragma endregion PlayerDamage
};