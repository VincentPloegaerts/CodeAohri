#pragma once

#include "CoreMinimal.h"
#include "IU_Camera.h"
#include "Cam_CameraArena.generated.h"



UCLASS()
class INSIDEUS_API ACam_CameraArena : public AIU_Camera
{
	GENERATED_BODY()
#pragma region Ground Arena	
	// Additional distance on ground
	UPROPERTY(VisibleAnywhere, Category = "Camera values|Collisions|Ground",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float additionalPitch = 0.0f;

	// Maximal Additional distance on ground
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Ground", meta = (ClampMin = "0.0", ClampMax = "10000.0", UIMin = "0.0", UIMax = "10000.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float maxAdditionalPitch = 250.0f;
	
	// Factor for speed when camera is hitting ground
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Ground", meta = (ClampMin = "0.0", ClampMax = "100.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float groundSpeedFactor  = 1;

	// Minimal pitch angle before lerping to the target 
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Ground", meta = (ClampMin = "-80.0", ClampMax = "80.0", UIMin = "-80.0", UIMax = "80.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float resetPitchAngle = -5.0f;

	// Camera velocity after leaving ground
	UPROPERTY(EditAnywhere, Category = "Camera values|Collisions|Ground", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"),BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float groundLerpSpeed = 2.0f;

#pragma endregion
	UPROPERTY(EditAnywhere, Category = "Camera values")
		float rampUpFactor = 1.5f;
	UPROPERTY(VisibleAnywhere, Category = "Camera values")
		float rampHeight = 0.5f;
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void MovePitchSuper(const float& _pitchValue,const float& _sens, const bool& _ignoreAcceptance, const bool& _ignoreInvert) override;
	virtual void UpdateLocation(const float& _deltaTime) override;
};
