#pragma once
#include "CoreMinimal.h"
#include "IU_Player.h"

#include "M_WorldTimeManager.h"

#include "Components/ActorComponent.h"
#include "C_SlowMotionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UC_SlowMotionComponent : public UActorComponent
{
	GENERATED_BODY()

	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlowMoBegin);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnSlowMoBegin"))
	FOnSlowMoBegin onSlowMoBegin = FOnSlowMoBegin();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlowMoEnd);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnSlowMoEnd"))
	FOnSlowMoEnd onSlowMoEnd = FOnSlowMoEnd();

	
	
	UPROPERTY(VisibleAnywhere,Category = "Settings")
		bool bCanSlow = true;

	UPROPERTY(VisibleAnywhere,Category = "Settings")
		bool bIsInSlowMotion = false;
	
	UPROPERTY()
		float fPrivateSlowMotionTimer = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fOrbConsumptionPerSeconds = 2.0f; 
	UPROPERTY(VisibleAnywhere, Category = "Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fBaseOrbConsumptionPerSeconds = 2.0f; 

	UPROPERTY()
		UC_PlayerAttackComponent* playerAttack = nullptr;
	
	UPROPERTY()
		AM_WorldTimeManager* worldTimeManager = nullptr;

	UPROPERTY()
	bool bHasUsedSlowMo = false;			//useful for tuto
public:	
	UC_SlowMotionComponent();

	
	FORCEINLINE void SetCanApplySlowMotion(bool _status) { bCanSlow = _status; }
	FORCEINLINE void SetOrbConsumptionPerSecond(float _newConso) { fOrbConsumptionPerSeconds = _newConso; }
	FORCEINLINE void ResetOrbConsumptionPerSecond() { fOrbConsumptionPerSeconds = fBaseOrbConsumptionPerSeconds; }
	
	FORCEINLINE void SetHasUsedSlowMo(bool _status) { bHasUsedSlowMo = _status; }
	FORCEINLINE bool GetHasUsedSlowMo() { return bHasUsedSlowMo; }
	FORCEINLINE void ResetHasUsedSlowMo() { bHasUsedSlowMo = false; }

	FORCEINLINE FOnSlowMoBegin& OnSlowMoBegin() {return onSlowMoBegin;}
	FORCEINLINE FOnSlowMoEnd& OnSlowMoEnd() {return onSlowMoEnd;}
	
		


	
	
private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable) void ApplySlowMotion();
	UFUNCTION(BlueprintCallable) void StopSlowMotion();
	UFUNCTION(BlueprintCallable) void SlowMotionTimer();
};