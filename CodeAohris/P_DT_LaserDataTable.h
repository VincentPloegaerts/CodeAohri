#pragma once

#include "CoreMinimal.h"
#include "P_DT_PatternDataTable.h"

#include "STRUCT_LaserSettings.h"

#include "P_DT_LaserDataTable.generated.h"

USTRUCT()
struct FLaserDataTableJSON : public FPatternDataTableJSON
{
	GENERATED_BODY()

	UPROPERTY()
	bool bEditLaser = false;
	UPROPERTY()
	FLaserSettings laserSettings;
};

UCLASS()
class INSIDEUS_API UP_DT_LaserDataTable : public UP_DT_PatternDataTable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Enable")
	bool bEditLaser = false;
	UPROPERTY(EditAnywhere, Category = "Behavior | Laser | Settings", meta = (EditCondition = "bEditLaser"))
	FLaserSettings laserSettings;

public:
	FORCEINLINE bool GetLaserState() const { return bEditLaser; }
	
	FORCEINLINE FLaserSettings GetLaserSettings() const { return laserSettings; }

#if WITH_EDITOR
public:
	virtual void SavePattern() override;
	virtual void LoadPattern() override;
	
	FLaserDataTableJSON GetLaserDataTableJSON() const;
	void LoadLaserDataTableJSON(const FLaserDataTableJSON& _json); 
	
	
	virtual void InitializePatternData(FPatternData& _data) const override;
#endif
};
