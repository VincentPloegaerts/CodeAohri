#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "P_PoolingTriggerComponent.generated.h"

class UP_PoolingComponent; 
class AP_Projectile;

//Pooling Trigger, used in the Poolig system. His Owner required Box & Sphere Component to work (And need to be Enable/Disable by owner)
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UP_PoolingTriggerComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileToPool, AP_Projectile*, _projectile);

	UPROPERTY(EditAnywhere, Category = "Pooling Trigger")
	bool bIsSphereTrigger = true;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "DEBUG")
	bool bShowDebug = false;
#endif
	
	UPROPERTY()
	AActor* owner = nullptr;
	UPROPERTY()
	UPrimitiveComponent* ownerBoxComponent = nullptr;
	UPROPERTY()
	UPrimitiveComponent* ownerSphereComponent = nullptr;
	
	UPROPERTY()
	FOnProjectileToPool onProjectileToPool;

public:
	FORCEINLINE const bool& IsSphereTrigger() const { return bIsSphereTrigger; }
	
public:	
	UP_PoolingTriggerComponent();

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Editor Trigger") FORCEINLINE void EnableTrigger() { EnablePoolingTrigger(); }
#endif
	
	//Call by Owner (can be an event) to enable Trigger only when needed 
	UFUNCTION() void EnablePoolingTrigger();
	//Call by Owner (can be an event) to disable Trigger 
	UFUNCTION() void DisablePoolingTrigger();
	
	//Mainly called by the GPE Shot to set Sphere Radius
	void SetPoolingTriggerSphereRadius(const float& _size) const;
	
private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Initialize Pooling Trigger
	void InitializePoolingTrigger();
	//Bind to owner EndOverlap and get owner Primitive Component (Box & Sphere) 
	void InitializeTriggerOwner();

	//Register the Primitive Component and enable his collision
	void EnablePrimitiveComponent(UPrimitiveComponent* _component) const;
	//Unregister the Primitive Component and disable his collision
	void DisablePrimitiveComponent(UPrimitiveComponent* _component) const;

	//Check if EndOverlap is by a Projectile to pool it
	UFUNCTION() void EndOverlapTrigger(AActor* _self, AActor* _other);

#if WITH_EDITOR
	void PoolingTriggerDebug();
#endif
};
