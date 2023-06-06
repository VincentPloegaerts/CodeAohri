#pragma once

#include "STRUCT_WaveSettings.generated.h"

/*Struct use to setup wave parameters.*/
USTRUCT()
struct FWaveSettings
{
	GENERATED_BODY()

	//Speed of the wave.
	UPROPERTY(EditAnywhere, Category = "Wave", meta = (ClampMin = "0"))
	float fWaveSpeed = 100;

	// Should use maxSize or not to stop the wave
	UPROPERTY(EditAnywhere,Category = "Wave")
		bool bUseMaxSize = false;
	
	//The time before the wave finish his behavior.
	UPROPERTY(EditAnywhere, Category = "Wave", meta = (ClampMin = "0",EditCondition = "!bUseMaxSize",EditConditionHides))
	float fWaveDuration = 5;
	
	//How wide in units can the wave go
	UPROPERTY(EditAnywhere,Category = "Wave",meta = (ClampMin = "0",EditCondition = "bUseMaxSize",EditConditionHides))
		float fMaxSize = 10000.0f;
	
	//Max size of the wave 
	UPROPERTY(EditAnywhere, Category = "Wave")
	float fWaveSize = 1.0f;
	
	//Damages of the Wave
	UPROPERTY(EditAnywhere, Category = "Wave", meta = (ClampMin = "0"))
	float fWaveDamage = 1;
	
	//Damages of the Wave
	UPROPERTY(EditAnywhere, Category = "Wave", meta = (ClampMin = "0"))
	float fWaveHeight = 1.0f;
	
	//Damages of the Wave
	UPROPERTY(EditAnywhere, Category = "Wave")
	float fWavethickness = 1.0f;

	
};