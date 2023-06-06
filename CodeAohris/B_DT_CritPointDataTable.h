#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "STRUCT_BossCritPoint.h"

#include "B_DT_CritPointDataTable.generated.h"

USTRUCT()
struct FCritPointDataTableJSON
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FCritPointData> critPointData = { };
};

UCLASS()
class INSIDEUS_API UB_DT_CritPointDataTable : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Crit Point", meta = (TitleProperty = "Crit Point {actorName} on Socket {socketName}"))
	TArray<FCritPointData> critPointData = { };

public:
	const TArray<FCritPointData>& GetCritPointData() const { return critPointData; }

	void SetCritPointData(const TArray<FCritPointData>& _critPointData) { critPointData = _critPointData; }

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Save") void Save();
	UFUNCTION(CallInEditor, Category = "Save") void Load();
#endif
};
