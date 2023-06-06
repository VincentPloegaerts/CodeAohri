#pragma once

#include "STRUCT_SoundConfig.generated.h"

USTRUCT(BlueprintType)
struct FSoundConfig
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics Config")
	bool mute = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics Config", meta = (ClampMin = 0, ClampMax = 100))
	float masterValue = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics Config", meta = (ClampMin = 0, ClampMax = 100))
	float musicValue = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics Config", meta = (ClampMin = 0, ClampMax = 100))
	float ambientValue = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics Config", meta = (ClampMin = 0, ClampMax = 100))
	float sfxValue= 50.0f;
	
public:
	FORCEINLINE const bool& GetMute() const { return mute; }
	
	FORCEINLINE const float& GetMasterValue() const { return masterValue; }
	FORCEINLINE const float& GetMusicValue() const { return musicValue; }
	FORCEINLINE const float& GetAmbientValue() const { return ambientValue; }
	FORCEINLINE const float& GetSFXValue() const { return sfxValue; }
	
	FORCEINLINE void SetMute(const bool& _mute) { mute = _mute; }
	
	FORCEINLINE void SetMasterValue(const float& _value) { masterValue = _value; }
	FORCEINLINE void SetMusicValue(const float& _value) { musicValue = _value; }
	FORCEINLINE void SetAmbientValue(const float& _value) { ambientValue = _value; }
	FORCEINLINE void SetSFXValue(const float& _value) { sfxValue = _value; }
};
