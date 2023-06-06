#pragma once

#include "CoreMinimal.h"
#include "IU_Camera.h"
#include "Cam_CameraPlatforming.generated.h"



UCLASS()
class INSIDEUS_API ACam_CameraPlatforming : public AIU_Camera
{
	GENERATED_BODY()
#pragma region Properties

	UPROPERTY(EditAnywhere, Category = "Camera values")
		float rampUpFactor = 1.5f;
	UPROPERTY(VisibleAnywhere, Category = "Camera values")
		float rampHeight = 0.5f;

	// Additional distance on ground
	UPROPERTY(VisibleAnywhere, Category = "Camera values",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float additionalPitch = 0.0f;
#pragma region Walls
	// Collision layers for the passive detections of walls
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	TArray<TEnumAsByte<EObjectTypeQuery>> cameraWallCollisionLayer;
	// Speed at which the camera will move if it detects a wall
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float repositionSpeed = 5.0f;	
	// Speed at which the camera will move back to its position after it detects a wall
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float wallLerpBackSpeed =2.0f;
	// Distance to be at when detecting a wall 
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float distanceMinFromWalls = 100.0f;
	
	//Factor the difference between 'distanceFromTarget' and the current position if there's a wall to move the camera up accordingly
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float wallLerpFactor = 1.0f;
	//The current result of 'wallLerpFactor' relative to the current position
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float additionalHeight = 0.0f;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FHitResult result;
	//How much input is accepted to not cancel the passive behaviour (to avoid cancelling it while moving on the other axis and not being 100% accurate)
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float anglePassiveAcceptance = 5.0f;
	//How often is the angle checked to see if the passive behaviour starts
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float angleCheckRate = 0.2f;
	//Float ticking up to angleCheckRate to pass 'movingPitch' to false
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float pitchCheckTime = 0.0f;
	//Float ticking up to angleCheckRate to pass 'movingYaw' to false
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float yawCheckTime = 0.0f;
	//Additional range on the raycast detecting walls on the sides
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float raycastAdditionalRange = 50.0f;
	//float added to the look at when against a wall
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float additionalPitchLookAt =0.0f;
	//float added to the look at when against a wall
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float maxAdditionalPitchLookAt = 300.0f;
	//multiply the 'additionalPitchLookAt' to match the correct angle speed
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float speedAdditionalPitchLookAt =5.0f;
	//is the camera against a wall
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Collisions|Wall Evasion",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool isWalled = false;
	
	// Radius for the sphere cast detecting walls to avoid NOT USED FOR NOW
	UPROPERTY(/*EditAnywhere, Category = "Camera values|Collisions|Wall Evasion|SphereTraceTest",*/BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float sphereRadius = 20.0f;
#pragma endregion

#pragma region FOV
	//How much FOV to add to the base one when moving
	UPROPERTY(EditAnywhere, Category = "Camera values|FOV",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float maxAdditionalFOV = 20.0f;
	//Speed at which the FOV zooms out
	UPROPERTY(EditAnywhere, Category = "Camera values|FOV",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float FOVLerpSpeed = 5.0f;
	//Fov to add on a Dash
	UPROPERTY(EditAnywhere, Category = "Camera values|FOV",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float additionalFOVOnDash = 5.0f;
	//Speed at which the FOV zooms back in
	UPROPERTY(EditAnywhere, Category = "Camera values|FOV",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float FOVLerpBackSpeed = 5.0f;
	UPROPERTY(VisibleAnywhere, Category = "Camera values|FOV",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float currentAdditionalFOV = 0.0f;
	//frames count that will not be used for 
	UPROPERTY(EditAnywhere, Category = "Camera values|FOV",BlueprintReadWrite, meta = (AllowPrivateAccess = True),meta = (ClampMin = "0",ClampMax = "10"))
		float skipTime = 0.3f;
	UPROPERTY(VisibleAnywhere, Category = "Camera values|FOV",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float skipProgress = 0;
	UPROPERTY(VisibleAnywhere, Category = "Camera values|FOV",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float baseFOV = 0.0f;
	UPROPERTY(VisibleAnywhere, Category = "Camera values|FOV",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool keepFOV = false;
#pragma endregion 

#pragma region Debug
	//Show some trace to debug collisions
	UPROPERTY(EditAnywhere, Category = "Camera values|Debug",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool showLineTrace = false;
	//Is the player moving the camera on the yaw Axis
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Debug",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool movingYaw = false;
	//Is the player moving the camera on the pitch Axis
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Debug",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool movingPitch = false;
	//Is the camera blocked against a wall with not enough input against wall
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Debug",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool isBlocked = false;
#pragma endregion
#pragma endregion

#pragma region Methods
#pragma region Engine
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
#pragma endregion

UFUNCTION(BlueprintCallable) void Init();

#pragma region Movement
	virtual void UpdateLocation(const float& _deltaTime) override;

	virtual void MovePitchSuper(const float& _pitchValue, const float& _sens,const bool& _ignoreAcceptance = false,const bool& _ignoreInvert = false) override;
	virtual void MoveYawSuper(const float& _yawValue, const float& _sens,const bool& _ignoreAcceptance = false,const bool& _ignoreInvert = false) override;
	//virtual FVector ComputeNewPosition(const float& _distance, const float& _pitch, const float& _yaw, const FVector& _targetLocation, const float& _minHeight) override;
	UFUNCTION(BlueprintCallable) void CheckValue(float& _currentValue,const float& _max,bool& _toChange, const float& _toAdd);
#pragma endregion

#pragma region Collisions
	//UFUNCTION(BlueprintCallable) void CheckSurroundings(const float& _yawValue);
	//void RayCastToWall(const FVector& _start,const FVector& _end,FHitResult& _result,bool& sideBlocked,float& _sideInputQuantity) const;
	//static bool CanPassWall(float& _value,const float& _max,const float& _toAdd);
	//UFUNCTION(BlueprintCallable) void CheckSurroundingsSphere();
#pragma endregion

#pragma region FOV
	UFUNCTION(BlueprintCallable) void ModifyFOV(float _input);
	UFUNCTION(BlueprintCallable) void UpdateFOV(float _additionalFOV);
	UFUNCTION(BlueprintCallable) void KeepingFOV(float _input);
	UFUNCTION(BlueprintCallable) void DashModifyFOV();
	UFUNCTION(BlueprintCallable) void UpdateTimerFOV(const float& _deltaTime);
#pragma endregion
#pragma endregion
};
