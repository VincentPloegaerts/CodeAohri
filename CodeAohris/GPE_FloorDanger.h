#pragma once

#include "CoreMinimal.h"
#include "GPE_Zone.h"
#include "GPE_FloorDanger.generated.h"

class AIU_Player;
class UP_PB_Scaling;
class UIU_HealthSystemComponent;

/**
 * !Beware!
 * You have to add manually the collider cause Unreal is a limited engine.
 */
UCLASS(Blueprintable)
class INSIDEUS_API AGPE_FloorDanger : public AGPE_Zone
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBehaviorDeltaTick, const float&, _deltaTime);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartCollideWithPlayer);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndCollideWithPlayer);

	//The type of collider used.
	UPROPERTY(EditAnywhere)
	TSubclassOf<UShapeComponent> colliderToUse;

	/**
	 * The collider used to damage the player.
	 * Value NOT INITIALIZED !
	 */
	UPROPERTY(EditAnywhere)
	UPrimitiveComponent* collider = nullptr;

	//The scaling behavior.
	UPROPERTY(EditAnywhere)
	UP_PB_Scaling* scalingBehavior = nullptr;

	//Health component of the player.
	UPROPERTY()
	UIU_HealthSystemComponent* playerHealth = nullptr;

	//The damage dealt by the collider.
	UPROPERTY()
	float fDamage = 1.0f;

	//Time before activating behavior.
	UPROPERTY()
	float fPreviewTime = 1.0f;

	//Time before calling the end of the GPE.
	UPROPERTY()
	float fPersistantTime = 5.0f;

	//Current used time.
	UPROPERTY()
	float fCurrentTime = 0.0f;
	
	UPROPERTY()
	FOnBehaviorDeltaTick onBehaviorDeltaTick;

protected:
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnStartCollideWithPlayer onStartCollideWithPlayer;
	
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnEndCollideWithPlayer onEndCollideWithPlayer;

public:
	AGPE_FloorDanger();

	virtual void InitializeGPEBehavior(UP_DT_ZoneDataTable* _zoneDataTable) override;
	virtual void InitializeGPEBehavior(FGPE_GlobalSettings& _patternData) override;
	virtual void BehaviorTick(const float& _deltaTime) override;

private:
	//Tick the time before activating behavior.
	UFUNCTION() void PreviewBehavior(const float& _deltaTime);

	//Tick the time before calling OnCompleteBehavior().
	UFUNCTION() void PersistantBehavior(const float& _deltaTime);

	//Activate the collider to damage the player.
	void ActivateDamageBehavior();

#pragma region PlayerDamage
	UFUNCTION() void StartCollideWithPlayer(AActor* _mine, AActor* _other);
	UFUNCTION() void DoDamageToPlayer(const float& _deltaTime);
	UFUNCTION() void EndCollideWithPlayer(AActor* _mine, AActor* _other);
#pragma endregion PlayerDamage
};