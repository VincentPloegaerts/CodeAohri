#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Camera/CameraComponent.h"
#include "C_FrustumChecker.h"

#include "STRUCT_CameraConfig.h"
#include "STRUCT_GraphicsConfig.h"

#include "IU_Player.h"

#include "IU_Camera.generated.h"

class UForceFeedbackComponent;
class ACameraShakeSourceActor;

UCLASS(Abstract)
class INSIDEUS_API AIU_Camera : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACameraShakeSourceActor> cameraShakeToSpawn;
	UPROPERTY(VisibleAnywhere)
	ACameraShakeSourceActor* cameraShaker = nullptr;

	UPROPERTY(EditAnywhere)
	UForceFeedbackEffect* vibroManetteToSpawn = nullptr;
	UPROPERTY(EditAnywhere)
	bool bCanSpawnVibroManette = true;
	UPROPERTY(VisibleAnywhere)
	UForceFeedbackComponent* vibroManetteComponent = nullptr;

	#pragma region Properties

	#pragma region Default
	
protected:
	// The status of camera movement
	UPROPERTY(VisibleAnywhere, Category = "Camera values")
		bool isActive = true;
	
	// The status of camera movement
	UPROPERTY(VisibleAnywhere, Category = "Camera values")
		bool bCanMove = true;

	//Is player moving the camera 
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Debug", meta = (ClampMin = "0.0", ClampMax = "1.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool movingCamera = false;
	
	// Axis start being registered after that value 
	// To Avoid drift on controller and such
	UPROPERTY(EditAnywhere, Category = "Camera values", meta = (ClampMin = "0.0", ClampMax = "1.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float axisAcceptance = 0.05f;
	
	// The movement speed around the target
	UPROPERTY(EditAnywhere, Category = "Camera values", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float moveSpeed = 50.0f;
	
	// The rotation speed for the look at on the target
	UPROPERTY(EditAnywhere, Category = "Camera values", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float rotateSpeed = 50.0f;
	
	// The rotation speed for the look at on the target
	UPROPERTY(EditAnywhere, Category = "Camera values", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float aimRotateFactor = 0.25f;
	UPROPERTY(EditAnywhere, Category = "Camera values",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float lerpToCinematicSpeed = 1.0f;

	UPROPERTY()
		bool useCinematicAngles = false;
	
	UPROPERTY()
		bool rotatePositiveSide = false;
	UPROPERTY()
		bool loopAngle = false;

	#pragma endregion

	#pragma region Target
	
	// The targeted character
	UPROPERTY(EditAnywhere, Category = "Camera values|Target",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSoftObjectPtr<AIU_Player> target = nullptr;
	
	// The distance between the camera and the target
	UPROPERTY(EditAnywhere, Category = "Camera values|Target", meta = (ClampMin = "0.0", ClampMax = "5000.0", UIMin = "0.0", UIMax = "5000.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float distanceFromTarget = 100.0f;
	
	// The distance between the camera and the target
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Target", meta = (ClampMin = "0.0", ClampMax = "5000.0", UIMin = "0.0", UIMax = "5000.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float currentDistanceFromTarget = 100.0f;
	
	// The position offset of the camera to the target
	UPROPERTY(EditAnywhere, Category = "Camera values|Target",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FVector positionOffset = FVector(0.0f);
	
	// The look at offset of the camera to the target
	UPROPERTY(EditAnywhere, Category = "Camera values|Target",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FVector lookAtOffset = FVector(0.0f);
	
	#pragma endregion

	#pragma region Yaw
	
	// Status of the X axis
	UPROPERTY(EditAnywhere, Category = "Camera values|Yaw",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool invertXAxis = false;
	
	// The current yaw angle
	UPROPERTY(EditAnywhere, Category = "Camera values|Yaw",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float yaw = 180.0f;
	
	// The velocity of the yaw angle
	UPROPERTY(EditAnywhere, Category = "Camera values|Yaw", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float yawSpeed = 50.0f;
	
	// The velocity of the yaw angle when there's no input on the right stick and the camera's just following
	UPROPERTY(EditAnywhere, Category = "Camera values|Yaw", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float passiveYawSpeed = 25.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Yaw",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float yawCinematic = 0.0f;

	#pragma endregion

	#pragma region Pitch
	
	// Status of the Y axis
	UPROPERTY(EditAnywhere, Category = "Camera values|Pitch",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool invertYAxis = false;
	
	// The current pitch angle
	UPROPERTY(EditAnywhere, Category = "Camera values|Pitch",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float pitch = 0.0f;
	
	// The minimal pitch angle
	UPROPERTY(EditAnywhere, Category = "Camera values|Pitch", meta = (ClampMin = "-90.0", ClampMax = "0.0", UIMin = "-90.0", UIMax = "0.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float minPitch = -80.0f;
	
	// The maximal pitch angle
	UPROPERTY(EditAnywhere, Category = "Camera values|Pitch", meta = (ClampMin = "0.0", ClampMax = "90.0", UIMin = "0.0", UIMax = "90.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float maxPitch = 80.0f;

	// The velocity of the pitch angle
	UPROPERTY(EditAnywhere, Category = "Camera values|Pitch", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float pitchSpeed = 50.0f;

	UPROPERTY(VisibleAnywhere, Category = "Camera values|Pitch",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float pitchCinematic = 0.0f;

	#pragma endregion

	#pragma region Collisions
	
	// Camera height when it hits the ground
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Collisions|Ground",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float cameraMinHeight = 0.0f;

	// Camera height when it hits the ground
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Collisions|Ceiling",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float cameraMaxHeight = 0.0f;
	// Layer of the ground
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Ground",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<TEnumAsByte<EObjectTypeQuery>> groundLayer = TArray<TEnumAsByte<EObjectTypeQuery>>();

	// Layer of the ceiling
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Ceiling",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<TEnumAsByte<EObjectTypeQuery>> ceilingLayer = TArray<TEnumAsByte<EObjectTypeQuery>>();
	
	// The range detection with the ground 
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float raycastRange = 1000.0f;

	// The height the camera will hover over ground
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Ground", meta = (ClampMin = "0.0", ClampMax = "10000.0", UIMin = "0.0", UIMax = "10000.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float heightFromGround = 40.0f;
	// The height the camera will hover over ground
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Ceiling", meta = (ClampMin = "0.0", ClampMax = "10000.0", UIMin = "0.0", UIMax = "10000.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float distanceFromCeiling = 10.0f;

	#pragma endregion

	#pragma region Reset Setting

	//Is the delay up and passive behaviour started
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Reset",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool passiveBehaviour = true;
	
	// Time before passive reaction reactive after an input
	UPROPERTY(EditAnywhere, Category = "Camera values|Reset",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float passiveReactivationDelay = 5.0f;
	
	// Time ticking before passive reaction reactive after an input
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Reset",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float passiveReactivationTime =0.0f;

	#pragma endregion

	#pragma region Options
	
	// Speed of Controller sensitivity that modify rotation speed
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Mouse Option",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float sensitivityXSpeed = 1.0f;

	// Speed of controller sensitivity that modify rotation speed
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Mouse Option",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float sensitivityYSpeed = 1.0f;

	// Speed of mouse sensitivity that modify rotation speed
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Mouse Option",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float mouseSensitivitySpeed = 1.0f;

	#pragma endregion

	#pragma region Components
	
	// The rendering component
	UPROPERTY(EditAnywhere, Category = "Camera values|Components",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UCameraComponent* render = nullptr;

	// The aim assist component
	UPROPERTY(EditAnywhere, Category = "Camera values|Components",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		class UC_AimAssistComponent* aimAssist = nullptr;

	// The rendering component
	UPROPERTY(EditAnywhere, Category = "Camera values|Components",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UC_FrustumChecker* frustumChecker = nullptr;

	#pragma endregion

	#pragma region AimAssist
	
	// Speed of aim assist that modify rotation speed
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Aim Assist",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float aimAssistSpeed = 1.0f;

	#pragma endregion

	#pragma endregion
	
public:
	#pragma region Getters / Setters

	#pragma region Getters
	
	FORCEINLINE bool CanMove() const { return bCanMove; }
	FORCEINLINE float GetPitch() const { return pitch; }
	FORCEINLINE float GetYaw() const { return yaw; }
	FORCEINLINE bool GetInvertXAxis() const { return invertXAxis; }
	FORCEINLINE bool GetInvertYAxis() const { return invertYAxis; }
	FORCEINLINE float GetSensitivityXSpeed() const { return sensitivityXSpeed; }
	FORCEINLINE float GetSensitivityYSpeed() const { return sensitivityYSpeed; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return render; }
	FORCEINLINE TSoftObjectPtr<AIU_Player> GetTarget() const { return target; }

	UFUNCTION(BlueprintCallable) FORCEINLINE FVector TargetLocation() const
	{
		return target ? target->GetActorLocation() : FVector(0.0f);
	}
	UFUNCTION(BlueprintCallable) FORCEINLINE FVector TargetLocationWithOffset(const FVector& _offset) const
	{
		if (!target) return TargetLocation();
		
		const FVector& _xVector = target->GetActorForwardVector() * _offset.X;
		const FVector& _yVector = target->GetActorRightVector() * _offset.Y;
		const FVector& _zVector = target->GetActorUpVector() * _offset.Z;
		return TargetLocation() + _xVector + _yVector + _zVector;
	}
	UFUNCTION(BlueprintCallable) FORCEINLINE FVector TargetLocationWithoutOffset(const FVector& _offset) const
	{
		if (!target) return TargetLocation();
		
		const FVector& _xVector = target->GetActorForwardVector() * _offset.X;
		const FVector& _yVector = target->GetActorRightVector() * _offset.Y;
		const FVector& _zVector = target->GetActorUpVector() * _offset.Z;
		return TargetLocation() - _xVector - _yVector - _zVector;
	}
	UFUNCTION(BlueprintCallable) FORCEINLINE FVector TargetLocationWithCameraOffset(const FVector& _offset) const
	{
		if (!target) return TargetLocation();
		
		const FVector& _xVector = GetActorForwardVector() * _offset.X;
		const FVector& _yVector = GetActorRightVector() * _offset.Y;
		const FVector& _zVector = GetActorUpVector() * _offset.Z;
		return TargetLocation() + _xVector + _yVector + _zVector;
	}

	#pragma endregion

	#pragma region Setters
	
	FORCEINLINE bool SetCanMove(bool _status) { return bCanMove = _status; }
	FORCEINLINE void SetUseCinematicAngles(bool _status)
	{
		yaw = yaw < 0.0f ? yaw + 360.0f : yaw;
		yawCinematic = yawCinematic < 0.0f ? yawCinematic + 360.0f : yawCinematic;
		useCinematicAngles = _status;
	}
	FORCEINLINE void SetRotatePositiveSide(bool _status)
	{
		loopAngle = (_status && yaw > yawCinematic) || (!_status && yaw < yawCinematic);
	}
	FORCEINLINE void SetCinematicAngles(const float& _pitch,const float& _yaw)
	{
		pitchCinematic = _pitch;
		yawCinematic = _yaw;
	}
	FORCEINLINE void SetTarget(const AIU_Player* _player)
	{
		if (!_player)return;
		target = _player;
	}
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetAimAssistSpeedFactor(const float _factor)
	{
		aimAssistSpeed = _factor;
	}
	FORCEINLINE void SetInvertX(bool _status)
	{
		invertXAxis = _status;
	}
	FORCEINLINE void SetInvertY(bool _status)
	{
		invertYAxis = _status;
	}
	FORCEINLINE void SetSensitivityXSpeed(const float _value)
	{
		sensitivityXSpeed = _value;
	}
	FORCEINLINE void SetSensitivityYSpeed(const float _value)
	{
		sensitivityYSpeed = _value;
	}
	FORCEINLINE void SetYaw(float _yawValue)
	{
		yaw = _yawValue;
	}
	FORCEINLINE void SetPitch(float _pitchValue)
	{
		pitch = _pitchValue;
	}
	#pragma endregion

	#pragma endregion 

public:
	AIU_Camera();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void MoveYawSuper(const float& _yawValue,const float& _sens,const bool& _ignoreAcceptance = false,const bool& _ignoreInvert = false);
	virtual void MovePitchSuper(const float& _pitchValue,const float& _sens,const bool& _ignoreAcceptance = false,const bool& _ignoreInvert = false);
	UFUNCTION(BlueprintCallable) virtual FVector ComputeNewPosition(const float& _distance,const float& _pitch,const float& _yaw,const FVector& _targetLocation,const float& _minHeight);
	bool CheckGroundCollisions();
	bool CheckCeilingCollisions();

private:
	void UpdateYawPitchViaAimAssist();
	UFUNCTION(BlueprintCallable) void MoveYawWithMovement(float _yawValue);

	UFUNCTION(BlueprintCallable) void InitializeCameraConfig(const FCameraConfig& _config);
	UFUNCTION(BlueprintCallable) void InitializeGraphicConfig(const FGraphicsConfig& _config);

	UFUNCTION() void InitShake();
	UFUNCTION() void StartShake(bool _hasTakeHit);
	UFUNCTION() void DestroyVibro();
	
public:
	UFUNCTION(BlueprintCallable) void UpdateCameraFromSave(const class US_PlayerSave* _playerSave);
	
	void ResetCamera(const FVector& _location, const float& _rotationZ);

	// Yaw
	UFUNCTION(BlueprintCallable) virtual void MoveYaw(float _yawValue);
	UFUNCTION(BlueprintCallable) virtual void MoveYawController(float _yawValue);

	// Pitch
	UFUNCTION(BlueprintCallable) virtual void MovePitch(float _pitchValue);
	UFUNCTION(BlueprintCallable) virtual void MovePitchController(float _pitchValue);

	UFUNCTION(BlueprintCallable) virtual void UpdateLocation(const float& _deltaTime);
};