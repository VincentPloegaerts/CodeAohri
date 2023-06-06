#pragma once
#include "CoreMinimal.h"

#include "A_DT_ArenaDataTable.h"
#include "STRUCT_SerieData.h"
#include "STRUCT_SerieElement.h"

#include "Engine/DataAsset.h"
#include "P_DT_SerieDataTable.generated.h"

USTRUCT()
struct FSerieDataTableJSON
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FSerieElement> serieElements = TArray<FSerieElement>();
	UPROPERTY()
	UA_DT_ArenaDataTable* arenaDataTable = nullptr;
};

UCLASS()
class INSIDEUS_API UP_DT_SerieDataTable : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		TArray<FSerieElement> serieElements = TArray<FSerieElement>();
	
	UPROPERTY(EditAnywhere)
		UA_DT_ArenaDataTable* arenaDataTable = nullptr;
	
public:
	FORCEINLINE TArray<FSerieElement> GetSerieElements() const { return serieElements; }
	
#if WITH_EDITOR
public:
	FSerieData GetSerieData() const;
	FSerieElementData GetSerieElementData(const FSerieElement& _rip) const;

	UFUNCTION(CallInEditor, Category = "Save") void SaveSerie();
	UFUNCTION(CallInEditor, Category = "Save") void LoadSerie();
	
	UFUNCTION() TArray<FString> GetPointNames() const;
#endif
};