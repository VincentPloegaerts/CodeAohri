#pragma once

#include "CoreMinimal.h"
#include "P_DT_BaseDataTable.h"

#include "STRUCT_WaveSettings.h"

#include "P_DT_WaveDataTable.generated.h"

USTRUCT()
struct FWaveDataTableJSON : public FBaseDataTableJSON
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool bOverrideWave = false;
	UPROPERTY()
	FWaveSettings waveSettings;
};

UCLASS(Blueprintable)
class INSIDEUS_API UP_DT_WaveDataTable : public UP_DT_BaseDataTable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Wave")
	bool bOverrideWave = false;
	UPROPERTY(EditAnywhere, Category = "Wave")
	FWaveSettings waveSettings;

public:
	FORCEINLINE const bool& GetOverrideWave() const { return bOverrideWave; }
	
	FORCEINLINE const FWaveSettings& GetWaveSettings() const { return waveSettings; }

#if WITH_EDITOR
public:
	virtual void SavePattern() override;
	virtual void LoadPattern() override;

	FWaveDataTableJSON GetWaveDataTableJSON() const;
	void LoadWaveDataTableJSON(const FWaveDataTableJSON& _json);
	
	virtual void InitializePatternData(FPatternData& _data) const override;
#endif
};