#pragma once

#include "STRUCT_PlayerConfig.generated.h"

USTRUCT(BlueprintType)
struct FPlayerConfig
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Config")
	bool bVibration = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Config", meta = (UIMin = "0.5", UIMax = "2", ClampMin = "0.5", ClampMax = "2"))
	float fSizeHUD = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Config", meta = (UIMin = "0.5", UIMax = "2", ClampMin = "0.5", ClampMax = "2"))
	float fSizeCrosshair = 0.5f;
	
public:
	FORCEINLINE const bool& GetVibrationEnable() const { return bVibration; }
	FORCEINLINE const float& GetSizeHUD() const { return fSizeHUD; }
	FORCEINLINE const float& GetSizeCrosshair() const { return fSizeCrosshair; }
	
	FORCEINLINE void SetVibrationEnable(const bool& _enable) { bVibration = _enable; }
	FORCEINLINE void SetSizeHUD(const float& _sizeHUD) { fSizeHUD = _sizeHUD; }
	FORCEINLINE void SetSizeCrosshair(const float& _sizeCrosshair) { fSizeCrosshair = _sizeCrosshair; }
};
