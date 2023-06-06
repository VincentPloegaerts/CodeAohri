#pragma once

#include "STRUCT_CameraConfig.generated.h"

USTRUCT(BlueprintType)
struct FCameraConfig
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Config")
	bool bInvertAxisX = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Config")
	bool bInvertAxisY = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Config", meta = (UIMin = "0.5", UIMax = "2", ClampMin = "0.5", ClampMax = "2"))
	float fMouseSensitivity = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Config", meta = (UIMin = "0.5", UIMax = "2", ClampMin = "0.5", ClampMax = "2"))
	float fGamepadSensitivityX = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Config", meta = (UIMin = "0.5", UIMax = "2", ClampMin = "0.5", ClampMax = "2"))
	float fGamepadSensitivityY = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Config", meta = (UIMin = "0.01", UIMax = "1", ClampMin = "0.01", ClampMax = "1"))
	float fAimSensitivity = 0.5;

public:
	FORCEINLINE const bool& GetInvertAxisX() const { return bInvertAxisX; }
	FORCEINLINE const bool& GetInvertAxisY() const { return bInvertAxisY; }
	FORCEINLINE const float& GetMouseSensitivity() const { return fMouseSensitivity; }
	FORCEINLINE const float& GetGamepadSensitivityX() const { return fGamepadSensitivityX; }
	FORCEINLINE const float& GetGamepadSensitivityY() const { return fGamepadSensitivityY; }
	FORCEINLINE const float& GetAimSensitivity() const { return fAimSensitivity; }

	FORCEINLINE void SetInvertAxisX(const bool& _invertX) { bInvertAxisX = _invertX; }
	FORCEINLINE void SetInvertAxisY(const bool& _invertY) { bInvertAxisY = _invertY; }
	FORCEINLINE void SetMouseSensitivity(const float& _sensitivity) { fMouseSensitivity = _sensitivity; }
	FORCEINLINE void SetGamepadSensitivityX(const float& _sensitivityX) { fGamepadSensitivityX = _sensitivityX; }
	FORCEINLINE void SetGamepadSensitivityY(const float& _sensitivityY) { fGamepadSensitivityY = _sensitivityY; }
	FORCEINLINE void SetAimSensitivity(const float& _sensitivity) { fAimSensitivity = _sensitivity; }
};
