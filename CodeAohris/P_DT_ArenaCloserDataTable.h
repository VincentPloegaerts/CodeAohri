#pragma once

#include "CoreMinimal.h"
#include "P_DT_BaseDataTable.h"

#include "STRUCT_ArenaCloserSettings.h"

#include "P_DT_ArenaCloserDataTable.generated.h"

USTRUCT()
struct FArenaCloserDataTableJSON : public FBaseDataTableJSON
{
	GENERATED_BODY()
	UPROPERTY()
    FArenaCloserSettings arenaCloserSettings;
	
	void InitBaseJSON(FBaseDataTableJSON _baseJSON)
	{
		patternType = _baseJSON.patternType;
	}
};

UCLASS()
class INSIDEUS_API UP_DT_ArenaCloserDataTable : public UP_DT_BaseDataTable
{
	GENERATED_BODY()
	
    	UPROPERTY(EditAnywhere, Category = "Behavior | Arena Closer")
    	FArenaCloserSettings arenaCloserSettings;

public:
	FORCEINLINE const FArenaCloserSettings& GetArenaCloserSettings() const { return arenaCloserSettings; }

#if WITH_EDITOR
public:
	virtual void SavePattern() override;
	virtual void LoadPattern() override;
	
	FArenaCloserDataTableJSON GetArenaCloserDataTableJSON() const;
	void LoadArenaCloserDataTableJSON(const FArenaCloserDataTableJSON& _json);
	
	virtual void InitializePatternData(FPatternData& _data) const override;
#endif
};
