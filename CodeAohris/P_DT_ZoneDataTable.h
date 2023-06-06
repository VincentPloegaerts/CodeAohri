#pragma once

#include "CoreMinimal.h"
#include "P_DT_PatternDataTable.h"

#include "STRUCT_ScalingSettings.h"
#include "STRUCT_SpawnerConcentricSettings.h"
#include "STRUCT_SpawnerDefaultSettings.h"
#include "STRUCT_SpawnerMultiSettings.h"
#include "STRUCT_SpawnerSettings.h"

#include "STRUCT_GPE_FloorDangerSettings.h"
#include "STRUCT_GPE_HuntRiftSettings.h"
#include "STRUCT_GPE_ProjectileSettings.h"

#include "P_DT_ZoneDataTable.generated.h"

USTRUCT()
struct FZoneDataTableJSON : public FPatternDataTableJSON
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool bScaling = false;
	UPROPERTY()
	FScalingSettings scalingSettings;
	
	UPROPERTY()
	FSpawnerSettings spawnerSettings;
	
	UPROPERTY()
	bool bSpawnerDefault = false;
	UPROPERTY()
	FSpawnerDefaultSettings spawnerDefaultSettings;
	
	UPROPERTY()
	bool bSpawnerConcentric = false;
	UPROPERTY()
	FSpawnerConcentricSettings spawnerConcentricSettings;
	
	UPROPERTY()
	bool bSpawnerMulti = false;
	UPROPERTY()
	FSpawnerMultiSettings spawnerMultiSettings;

	UPROPERTY()
	bool bEditProjectileGPE = false;
	UPROPERTY()
	FGPE_ProjectileSettings projectileSettings;

	UPROPERTY()
	bool bEditFloorGPE = false;
	UPROPERTY()
	FGPE_FloorDangerSettings floorDangerSettings;

	UPROPERTY()
	bool bEditHuntRiftGPE = false;
	UPROPERTY()
	FGPE_HuntRiftSettings huntRiftSettings;
};

UCLASS(Blueprintable)
class INSIDEUS_API UP_DT_ZoneDataTable : public UP_DT_PatternDataTable
{
	GENERATED_BODY()

	//Scaling
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Scaling")
	bool bScaling = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Scaling", meta = (EditCondition = "bScaling")) 
	FScalingSettings scalingSettings;
	
	//Spawner
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings", meta = (EditCondition = "bSpawnerDefault || bSpawnerConcentric || bSpawnerMulti"))
	FSpawnerSettings spawnerSettings;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Enable")
	bool bSpawnerDefault = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings", meta = (EditCondition = "bSpawnerDefault"))
	FSpawnerDefaultSettings spawnerDefaultSettings;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Enable")
	bool bSpawnerConcentric = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings", meta = (EditCondition = "bSpawnerConcentric"))
	FSpawnerConcentricSettings spawnerConcentricSettings;
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Enable")
	bool bSpawnerMulti = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | Settings", meta = (EditCondition = "bSpawnerMulti"))
	FSpawnerMultiSettings spawnerMultiSettings;

	//GPE
	
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | GPE")
	bool bEditProjectileGPE = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | GPE", meta = (EditCondition = "bEditProjectileGPE", EditConditionHides)) 
	FGPE_ProjectileSettings projectileSettings;

	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | GPE")
	bool bEditFloorGPE = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | GPE", meta = (EditCondition = "bEditFloorGPE", EditConditionHides)) 
	FGPE_FloorDangerSettings floorDangerSettings;

	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | GPE")
	bool bEditHuntRiftGPE = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Spawner | GPE", meta = (EditCondition = "bEditHuntRiftGPE", EditConditionHides)) 
	FGPE_HuntRiftSettings huntRiftSettings;

public:
	FORCEINLINE const bool& GetScalingState() const { return bScaling; }
	
	FORCEINLINE const bool& GetSpawnerDefaultState() const { return bSpawnerDefault; }
	FORCEINLINE const bool& GetSpawnerConcentricState() const { return bSpawnerConcentric; }
	FORCEINLINE const bool& GetSpawnerMultiState() const { return bSpawnerMulti; }

	FORCEINLINE const bool& GetEditProjectileState() const { return bEditProjectileGPE; }
	FORCEINLINE const bool& GetEditFloorState() const { return bEditFloorGPE; }
	FORCEINLINE const bool& GetEditHuntRiftState() const { return bEditHuntRiftGPE; }
	
	FORCEINLINE const FScalingSettings& GetScalingSettings() const { return scalingSettings; }
	
	FORCEINLINE const FSpawnerSettings& GetSpawnerSettings() const { return spawnerSettings; }
	FORCEINLINE const FSpawnerDefaultSettings& GetSpawnerDefaultSettings() const { return spawnerDefaultSettings; }
	FORCEINLINE const FSpawnerConcentricSettings& GetSpawnerConcentricSettings() const { return spawnerConcentricSettings; }
	FORCEINLINE const FSpawnerMultiSettings& GetSpawnerMultiSettings() const { return spawnerMultiSettings; }
	
	FORCEINLINE const FGPE_ProjectileSettings& GetGPEProjectileSettings() const { return projectileSettings; }
	FORCEINLINE const FGPE_FloorDangerSettings& GetGPEFloorDangerSettings() const { return floorDangerSettings; }
	FORCEINLINE const FGPE_HuntRiftSettings& GetGPEHuntRiftSettings() const { return huntRiftSettings; }

#if WITH_EDITOR
public:
	virtual void SavePattern() override;
	virtual void LoadPattern() override;

	FZoneDataTableJSON GetZoneDataTableJSON() const;
	void LoadZoneDataTableJSON(const FZoneDataTableJSON& _json);
	
	virtual void InitializePatternData(FPatternData& _data) const override;
#endif
};