#pragma once

#include "CoreMinimal.h"
#include "GPE_Platform.h"
#include "NiagaraComponent.h"
#include "GPE_Laser.generated.h"

UENUM(BlueprintType)
enum ELaserMode
{
	Standard,
	Infinity,
	Balayage,
	Rotate,
	SplineBackAndForth,
	SplineMovement
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaserActivate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaserDeactivate);

UCLASS()
class INSIDEUS_API AGPE_Laser : public AGPE_Platform
{
	GENERATED_BODY()
#pragma region Properties
#pragma region Base
protected:	
	//Current mode in use
	UPROPERTY(EditAnywhere,Category = "GPE|Laser",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		TEnumAsByte<ELaserMode> laserMode = Standard;
	//Duration in seconds for the patterns, the time it takes for it to do a cycle
	UPROPERTY(EditAnywhere,Category = "GPE|Laser",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float duration = 3.0f;
	//How far in seconds the actor is in for the patterns
	UPROPERTY(VisibleAnywhere,Category = "GPE|Laser",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float timePattern = 0.0f;
	//If the pattern is activated or not, used to determine which part of the pattern we're on,
	//NOT to deactivate the laser completely 
	UPROPERTY(EditAnywhere,Category = "GPE|Laser",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		bool isActive = true;
	//Damages done when entering collider
	UPROPERTY(EditAnywhere,Category = "GPE|Laser",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float damages = 100.0f;
	UPROPERTY(EditAnywhere,Category = "GPE|Laser",BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta = (ClampMin = "0.0",ClampMax = "5.0"))
		float durationNiagaraBeforeActivation = 0.5f;
	//Beam end property to set in Niagaras get from box size
	UPROPERTY(VisibleAnywhere,Category = "GPE|Laser",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FVector beamEnd = FVector(0);
	//Set this to the name of the vector property defining the size of the niagara
	UPROPERTY(EditAnywhere,Category = "GPE|Laser",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FName nameSizeBeamProperty = "Beam End";
#pragma endregion

#pragma region Rotate
private:
	//If true laser will rotate towards - Pi 
	UPROPERTY(EditAnywhere,Category = "GPE|Laser|Rotate",BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta=(EditCondition="laserMode == ELaserMode::Rotate",EditConditionHides))
		bool rotateCounterClockWise = false;
	//How wide from starting position should the rotation be
	UPROPERTY(EditAnywhere,Category = "GPE|Laser|Balayage",BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta = (ClampMin = "-360.0",ClampMax = "360.0"),meta=(EditCondition="laserMode == ELaserMode::Balayage"))
		float angleBalayage = 30.0f;
	//How wide should the slow portion of the laser be (counted by each edges)
	UPROPERTY(EditAnywhere,Category = "GPE|Laser|Balayage",BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta = (ClampMin = "0.0",ClampMax = "50.0"),meta=(EditCondition="laserMode == ELaserMode::Balayage"))
		float halfSlowPortionPercent = 10.0f;
	//How slow should the slow portion of the laser be (percent of the actual speed)
	UPROPERTY(EditAnywhere,Category = "GPE|Laser|Balayage",BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta = (ClampMin = "0.0",ClampMax = "100.0"),meta=(EditCondition="laserMode == ELaserMode::Balayage"))
		float speedPercentSlowPortion = 20.0f;
	//How fast the transition from fast to slow and vice versa should be
	UPROPERTY(EditAnywhere,Category = "GPE|Laser|Balayage",BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta = (ClampMin = "0.0"),meta=(EditCondition="laserMode == ELaserMode::Balayage"))
		float speedChangeLerpSpeed = 5.0f;
	//Current Speed percent being lerp from 100 to 'speedPercentSlowPortion'
	UPROPERTY(VisibleAnywhere,Category = "GPE|Laser|Balayage",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float actualSpeedPercent = 100.0f;
	//Get the laser rotation at BeginPlay so it's not calculated each tick
	UPROPERTY(VisibleAnywhere,Category = "GPE|Laser|Rotate",BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta=(EditCondition="laserMode == ELaserMode::Rotate"))
		FRotator baseRotation = FRotator::ZeroRotator;
	//Calculate the laser final rotation at BeginPlay so it's not calculated each tick
	UPROPERTY(VisibleAnywhere,Category = "GPE|Laser|Rotate",BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta=(EditCondition="laserMode == ELaserMode::Rotate"))
		FRotator finalRotation = FRotator::ZeroRotator;
#pragma endregion

#pragma region Component
protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	UBillboardComponent* laserBillboard = nullptr;	
#endif
	
	//Collider to hit the player
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UStaticMeshComponent* damageBox = nullptr;
	//Laser Niagara Component
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UNiagaraComponent* niagaraLaser = nullptr;
#pragma endregion

#pragma region Events
	//Broadcast when the laser activate ONLY IN STANDARD MODE
	UPROPERTY(EditAnywhere,BlueprintAssignable,Category = "GPE|Events",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FOnLaserActivate onLaserActivate;
	//Broadcast when the laser Deactivate ONLY IN STANDARD MODE
	UPROPERTY(EditAnywhere,BlueprintAssignable,Category = "GPE|Events",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FOnLaserDeactivate onLaserDeactivate;
#pragma endregion
#pragma endregion 
public:
	FORCEINLINE FOnLaserActivate& OnLaserActivate() {return onLaserActivate;}
	FORCEINLINE FOnLaserDeactivate& OnLaserDeactivate() {return onLaserDeactivate;}
	
public:
	AGPE_Laser();
protected:
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable) void UpdateStandard(float _deltaSeconds);
	UFUNCTION(BlueprintCallable) void UpdateBalayage(float _deltaSeconds);
	UFUNCTION(BlueprintCallable) void UpdateRotate(float _deltaSeconds);
	UFUNCTION(BlueprintCallable) void OnHit(AActor* _self,AActor* _other);
	UFUNCTION(BlueprintCallable) void Init();
};
