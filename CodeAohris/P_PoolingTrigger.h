#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P_PoolingTrigger.generated.h"

UCLASS()
class INSIDEUS_API AP_PoolingTrigger : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Pooling Trigger | Settings")
	float fDisableTimer = 5.0f;
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Pooling Trigger | Debug")
	bool bDebug = false;
#endif
	
	UPROPERTY()
	float fTime = 0;
	UPROPERTY()
	bool bDisableProcess = false;
	UPROPERTY()
	bool bDisable = false;
	
public:	
	AP_PoolingTrigger();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	//Initialize Pooling Trigger
	virtual void InitializePoolingTrigger();

public:
	//Call by Owner (can be an event) to enable Trigger only when needed
	UFUNCTION(BlueprintCallable) virtual void EnablePoolingTrigger();
	////Call by Owner (can be an event) to Disable the Trigger after fDisableTimer
	UFUNCTION(BlueprintCallable) void BeginDisablePoolingTrigger();
	//Call by Owner (can be an event) to disable Trigger
	UFUNCTION(BlueprintCallable) virtual void DisablePoolingTrigger();

private:
	//Check if EndOverlap is by a Projectile to pool it
	UFUNCTION() void OnEndOverlapPoolingTrigger(AActor* _self, AActor* _other);

#if WITH_EDITOR
protected:
	virtual void DrawPoolingTriggerDebug();
#endif
};
