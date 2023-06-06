#pragma once

#include "CoreMinimal.h"
#include "GPE_Platform.h"
#include "IU_Player.h"
#include "IU_PlayerMovementComponent.h"
#include "GPE_Module.generated.h"

class AGPE_ZoneModuActivation;
class UGPE_SweepVerticalAnimInstance;

UENUM(BlueprintType)
enum EGPEMode
{
	Base,
	Sweep,
	Rotation,
	Movement
};

UCLASS()
class INSIDEUS_API AGPE_Module : public AGPE_Platform
{
	GENERATED_BODY()
protected:
#pragma region Properties

#pragma region GPE Values	
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values")
		TEnumAsByte<EGPEMode> gpeMode = Base;
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values")
		float damages = 100.0f;
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values")
		float knockBackStrength = 100.0f;
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values")
		float knockBackDuration = 3.0f;
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values")
		 TSoftObjectPtr<AGPE_ZoneModuActivation> activationBox = nullptr;
	UPROPERTY()
		bool bHasCollideThisFrame = false;
	UPROPERTY()
		bool boxActivated = false;

#pragma region Sweep
private:
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values|Balayage")
		bool bUseAnimation = false;
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values|Balayage", meta = (EditCondition = "bUseAnimation", EditConditionHides))
		float fPlayRateAnim = 1.0f;
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values|Balayage", meta = (EditCondition = "bUseAnimation", EditConditionHides))
		float fPlayRateWarning = 1.0f;
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values|Balayage", meta = (EditCondition = "!bUseAnimation", EditConditionHides))
		float fSweepAngle = 90.0f;
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values|Balayage", meta = (EditCondition = "!bUseAnimation", EditConditionHides))
		float fSpeedForward = 60.0f;
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values|Balayage", meta = (EditCondition = "!bUseAnimation", EditConditionHides))
		float fSpeedBackward = 60.0f;
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values|Balayage|Pause")
		float fPauseTime = 2.0f;
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values|Balayage|Pause")
		float fPauseProgress = 0.0f;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "GPE|GPE Values|Balayage|Pause",meta = (AllowPrivateAccess = "True"))
		bool bIsPaused = false;
#pragma endregion 
	
#pragma region Rotation
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values|Rotation")
		bool bCounterClockwise = false;
	UPROPERTY(EditAnywhere,Category = "GPE|GPE Values|Rotation")
		float fRotateSpeed = 90.0f;
#pragma endregion

#pragma region Debug
protected:
	UPROPERTY(VisibleAnywhere,Category = "GPE|Debug")
		float fProgress = 0.0f;
	UPROPERTY(VisibleAnywhere,Category = "GPE|Debug")
		bool bGoingForward = true;
	UPROPERTY(VisibleAnywhere,Category = "GPE|Debug")
		FRotator baseRotation = FRotator::ZeroRotator;
	UPROPERTY(VisibleAnywhere,Category = "GPE|Debug")
		float fSlowSpeed = 1.0f;
	UPROPERTY(VisibleAnywhere,Category = "GPE|Debug")
		FRotator finalRotation;
	UPROPERTY(VisibleAnywhere,Category = "GPE|Debug")
		FVector knockDirection = FVector(0.0f);
	UPROPERTY(VisibleAnywhere,Category = "GPE|Debug")
		AIU_Player* playerRef = nullptr;
	UPROPERTY(VisibleAnywhere,Category = "GPE|Debug")
		UGPE_SweepVerticalAnimInstance* animInstanceRef = nullptr;
	UPROPERTY(VisibleAnywhere,Category = "GPE|Debug")
		FTimerHandle timerHandle;
#pragma endregion

#pragma endregion

#pragma region Components
	UPROPERTY(EditAnywhere,Category = "GPE|Components")
		USkeletalMeshComponent* skeletalMesh = nullptr;
	UPROPERTY(EditAnywhere,Category = "GPE|Components")
		UStaticMeshComponent* damageMesh = nullptr;
	UPROPERTY(EditAnywhere,Category = "GPE|Components")
		USceneComponent* rotationRoot = nullptr;
#pragma endregion

#pragma region Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPauseUpdate);
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
		FOnPauseUpdate onPauseStart;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
		FOnPauseUpdate onPauseEnd;
#pragma endregion

#pragma endregion 
public:
	AGPE_Module();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable) void UpdateBase(const float& _fDeltaSeconds);
	UFUNCTION(BlueprintCallable) void UpdateSweep(const float& _fDeltaSeconds);
	UFUNCTION(BlueprintCallable) void UpdateRotation(const float& _fDeltaSeconds);
	UFUNCTION(BlueprintCallable) void UpdatePause(const float& _fDeltaSeconds);
	UFUNCTION(BlueprintCallable) void SetSlowSpeed(const float& _speed);
	UFUNCTION(BlueprintCallable) void Init();
	UFUNCTION(BlueprintCallable) void OnMeshOverlap(UPrimitiveComponent* _overlappedComponent,
													AActor* _otherActor,
													UPrimitiveComponent* _otherComp,
													int32 _otherBodyIndex,
													bool _bFromSweep,
													const FHitResult& _SweepResult);
	UFUNCTION(BlueprintCallable) void KnockPlayer(AIU_Player* _player);
	//UFUNCTION(BlueprintCallable) FORCEINLINE void StopKnock();
	virtual void CheckPlayerDistance() override;
	UFUNCTION(BlueprintCallable) void BoxCollided(AActor* _box, AActor* _other);
};
