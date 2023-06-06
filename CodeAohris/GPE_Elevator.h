#pragma once
#include "CoreMinimal.h"
#include "GPE_Platform.h"

#include "Components/SplineComponent.h"
#include "GPE_BalanceMiniGame.h"
#include "C_InteractionWidgetComponent.h"
#include "GPE_Elevator.generated.h"

USTRUCT()
struct FGPE_ElevatorJson : public  FGPE_BaseJson
{
	GENERATED_BODY()
	
	UPROPERTY()
	float fSavedProgress = 0.0f;
	
};

class UGPE_ElevatorAnimInstance;

UCLASS()
class INSIDEUS_API AGPE_Elevator : public AGPE_Platform
{
	GENERATED_BODY()
#pragma region Properties
#pragma region GPE Values
	//How fast should the mesh Move along the splines (units per seconds)
	UPROPERTY(EditAnywhere, Category = "GPE|Values", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fMoveSpeed = 200.0f;
	//How fast should the mesh Move along the splines
	UPROPERTY(EditAnywhere, Category = "GPE|Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fProgress = 0.0f;
	//Is the Elevator moving
	UPROPERTY(EditAnywhere, Category = "GPE|Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bIsMoving = false;
	//Is the Elevator moving backward 
	UPROPERTY(EditAnywhere, Category = "GPE|Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bReverse = false;
	//stocked the spline lenght for better performance
	UPROPERTY(VisibleAnywhere, Category = "GPE|Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float stockedSplineLenght = 0.0f;
	//How fast should the mesh Move along the splines (units per seconds)
	UPROPERTY(VisibleAnywhere, Category = "GPE|Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float fActualMoveSpeed = 200.0f;
	UPROPERTY(VisibleAnywhere, Category = "GPE|Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool canBeUsed = true;
	UPROPERTY(EditAnywhere,Category="GPE|Camera")
		float cameraPitch = 20.0f;
	UPROPERTY(EditAnywhere,Category="GPE|Camera")
		float cameraYaw = 0.0f;
	UPROPERTY()
		float fProgressAtSave = 0.0f;

	UPROPERTY(EditAnywhere, Category = "GPE|Animation", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bUseCustomPlayRate = false;
	UPROPERTY(EditAnywhere, Category = "GPE|Animation", BlueprintReadWrite, meta = (EditCondition = bUseCustomPlayRate, EditConditionHides, AllowPrivateAccess = True))
		float fCustomPlayRate = 1.0f;

	UPROPERTY()
	bool isWidgetHidden = true;
	UPROPERTY()
	class UUI_PlayerWidget* playerWidget = nullptr;
	UPROPERTY()
	FTimerHandle timerDistance;
#pragma endregion 
	
#pragma region Events
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArrived);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartMoving);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteracted);
	//Broadcasted when the mesh attains the end of the spline
	UPROPERTY(VisibleAnywhere, Category = "GPE|Events", BlueprintReadWrite,BlueprintAssignable, meta = (AllowPrivateAccess = True))
		FOnArrived onArrived;	
	//Broadcasted when the mesh starts moving
	UPROPERTY(VisibleAnywhere, Category = "GPE|Events", BlueprintReadWrite,BlueprintAssignable, meta = (AllowPrivateAccess = True))
		FOnStartMoving onStartMoving;
	//Broadcasted the player interacts with the elevator
	UPROPERTY(VisibleAnywhere, Category = "GPE|Events", BlueprintReadWrite,BlueprintAssignable, meta = (AllowPrivateAccess = True))
		FOnInteracted onInteracted;
#pragma endregion 
	
#pragma region Components
	//Spline that the mesh follows
	UPROPERTY(EditAnywhere, Category = "GPE|Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		USplineComponent* splinePath = nullptr;
	UPROPERTY(EditAnywhere, Category = "GPE|Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		USkeletalMeshComponent* skeletalMesh = nullptr;
	//to know where the player need to look at during the elevator animation
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True),Category = "Anim TP location")
		USceneComponent* animLocation = nullptr;
	

#pragma endregion
	
	UPROPERTY()
		UGPE_ElevatorAnimInstance* animInstance;


#pragma endregion

	UPROPERTY() bool bCanBeActivated = true;
	
public:
	FORCEINLINE FOnArrived& OnArrived() {return onArrived;}
	FORCEINLINE FOnStartMoving& OnStartMoving() {return onStartMoving;}
	FORCEINLINE FOnInteracted& OnInteracted() {return onInteracted;}
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const {return skeletalMesh;}
	UFUNCTION(BlueprintCallable) FORCEINLINE void InverseMovement() {bReverse = !bReverse;}
	FORCEINLINE USceneComponent* GetAnimLocation() const {return animLocation;}
	FORCEINLINE bool GetUseCustomPlayRate() const {return bUseCustomPlayRate;}
	FORCEINLINE bool GetCanBeUsed() const {return canBeUsed;}
	FORCEINLINE float GetCustomPlayRate() const {return fCustomPlayRate;}
	
public:
	AGPE_Elevator();
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void SetAnimationState(bool _status) const;
	
	UFUNCTION() void CheckPlayerInteractionRange();
	
public:
	UFUNCTION(BlueprintCallable) void InstantActivateOnLoad();
	UFUNCTION(BlueprintCallable) void ActivateElevator(const bool& _reset = false);
	UFUNCTION(BlueprintCallable) void DeactivateElevator();
	UFUNCTION(BlueprintCallable) void MoveMeshAlongSpline(const float& _progressToAdd);
	UFUNCTION(BlueprintCallable) void FailedMiniGame(const float& _speed,const float& _duration);
	UFUNCTION(BlueprintCallable) void StopFail();

	virtual void InitializeGPE(const FString& _jsonStr) override;
	virtual FString GetGPEJson() const override;
	
	
};
