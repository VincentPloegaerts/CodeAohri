#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "CineCameraComponent.h"
#include "Components/SplineComponent.h"

#include "Cam_CameraMenu.generated.h"

class AS_SaveActor;

UENUM()
enum ECameraMenuPoint
{
	POINT_NONE UMETA(DisplayName = "None"),
	POINT_TITLE UMETA(DisplayName = "Title"),
	POINT_SAVE UMETA(DisplayName = "Save")
};

USTRUCT()
struct FCameraMenuPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 pointIndex = -1;
	UPROPERTY(EditAnywhere)
	AActor* pointView = nullptr;
};

UCLASS()
class INSIDEUS_API ACam_CameraMenu : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArrivedAtPoint);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCineCameraComponent* camera = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* spline = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = "Camera Menu | Settings Points")
	TMap<TEnumAsByte<ECameraMenuPoint>, FCameraMenuPoint> points = { };
	
	UPROPERTY(EditAnywhere, Category = "Camera Menu | Settings Camera")
	float accelerationTime = 2;
	UPROPERTY(EditAnywhere, Category = "Camera Menu | Settings Camera")
	float maxAcceleration = 5;
	UPROPERTY(EditAnywhere, Category = "Camera Menu | Settings Camera")
	float decelerationTime = 2;
	UPROPERTY(EditAnywhere, Category = "Camera Menu | Settings Camera")
	float maxDeceleration = 5;
	UPROPERTY(EditAnywhere, Category = "Camera Menu | Settings Camera")
	float globalMoveSpeed = 1;
	UPROPERTY(EditAnywhere, Category = "Camera Menu | Settings Camera")
	float rotateSpeed = 150;
	UPROPERTY(EditAnywhere, Category = "Camera Menu | Settings Camera")
	FVector offset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "Camera Menu | Settings Camera")
	float range = 1;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera Menu | Values")
	TEnumAsByte<ECameraMenuPoint> currentPoint = ECameraMenuPoint::POINT_NONE;
	UPROPERTY(VisibleAnywhere, Category = "Camera Menu | Values")
	float time = 0;
	UPROPERTY(VisibleAnywhere, Category = "Camera Menu | Values")
	TEnumAsByte<ECameraMenuPoint> previousPoint = ECameraMenuPoint::POINT_NONE;
	UPROPERTY(VisibleAnywhere, Category = "Camera Menu | Values")
	float currentMoveSpeed = 0;
	UPROPERTY(VisibleAnywhere, Category = "Camera Menu | Values")
	bool isAtPoint = false;

	UPROPERTY()
	FOnArrivedAtPoint onArrivedAtPoint;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Camera Menu | Debug")
	bool debug = false;
	UPROPERTY(EditAnywhere, Category = "Camera Menu Debug")
	TEnumAsByte<ECameraMenuPoint> debugPoint = ECameraMenuPoint::POINT_NONE;
#endif
	
public:
	FORCEINLINE FOnArrivedAtPoint& OnArrivedAtPoint() { return onArrivedAtPoint; }
	FORCEINLINE const UCineCameraComponent* GetCamera() const { return camera; } 
	
	FORCEINLINE bool IsValidPoint(const ECameraMenuPoint& _point) const { return points.Contains(_point); }
	FORCEINLINE bool IsValidIndexPoint(const int32& _index) const { return spline->GetNumberOfSplinePoints() > _index; }
	
public:
	ACam_CameraMenu();
	
	UFUNCTION(CallInEditor, Category = "Camera Menu Debug") void SetPoint(const ECameraMenuPoint& _point);
	void InitCameraMenu(const ECameraMenuPoint& _point);

	bool IsAtPoint();
	
private:	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	FORCEINLINE virtual bool ShouldTickIfViewportsOnly() const override { return debug; }
#endif
	
	void InitializeCameraMenu();
	UFUNCTION() void UpdateCamera(const class US_PlayerSave* _playerSave);

#pragma region Behavior
	void CameraBehavior(const float _deltaTime);
	void MoveToSplinePoint(const int32& _index, const float _deltaTime);
	void LookAt(const AActor* _view, const float _deltaTime) const;
	void UpdateSpeed(const int32& _index, const float _deltaTime);
#pragma endregion

#pragma region Navigation
	UFUNCTION() void MoveToTitle();
	UFUNCTION() void MoveToSave(); 
#pragma endregion

	float NormalizeValue(const float _current, const float _min, const float _max) const;
	float ClampTime(const float _min, const float _max) const;

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Camera Menu Debug") void SetDebugPoint();
	
	void DrawDebugCameraMenu();
#endif
};
