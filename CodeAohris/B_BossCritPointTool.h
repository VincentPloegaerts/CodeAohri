#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "STRUCT_BossCritPoint.h"

#include "B_BossCritPointTool.generated.h"

class AB_Boss;
class AB_BossCritPoint;

UCLASS()
class INSIDEUS_API AB_BossCritPointTool : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "CRIT POINT TOOL | Settings")
	AB_Boss* boss = nullptr;
	UPROPERTY(EditAnywhere, Category = "CRIT POINT TOOL | Create")
	FString critPointName = ""; 
	UPROPERTY(EditAnywhere, Category = "CRIT POINT TOOL | Create", meta = (GetOptions = GetSocketNames))
	FName socketName = "NONE";
	UPROPERTY(EditAnywhere, Category = "CRIT POINT TOOL | Create")
	TSubclassOf<AB_BossCritPoint> critPointType;
	UPROPERTY(EditAnywhere, Category = "CRIT POINT TOOL | Auto Save")
	bool autoSave = false;
	UPROPERTY(EditAnywhere, Category = "CRIT POINT TOOL | Auto Save", meta = (ClampMin = 0, UIMin = 0, Units = "s", EditCondition = "autoSave", EditConditionHides))
	float autoSaveTime = 20;
	UPROPERTY(VisibleAnywhere, Category = "CRIT POINT TOOL | Auto Save", meta = (Units = "s", EditCondition = "autoSave", EditConditionHides))
	float elapsedTimeAutoSave = 0;
	
	UPROPERTY(EditAnywhere, Category = "CRIT POINT | Values", meta = (TitleProperty = "Socket : {socketName}"))
	TMap<AB_BossCritPoint*, FCritPointData> critPoints = { };
	
	
	
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TMap<AB_BossCritPoint*, FCritPointData> oldCritPoints = { };
#endif
	
public:	
	AB_BossCritPointTool();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual bool ShouldTickIfViewportsOnly() const override { return autoSave; }
	
	virtual void Destroyed() override;

	UFUNCTION(CallInEditor) TArray<FName> GetSocketNames() const;
	FORCEINLINE bool IsSocketValid(const FName& _socketName) const { return GetSocketNames().Contains(_socketName); }

	UFUNCTION(CallInEditor, Category = "CRIT POINT TOOL", DisplayName = Create) void CreateCritPoint();
	UFUNCTION(CallInEditor, Category = "CRIT POINT TOOL", DisplayName = Save) void SaveCritPoint();
	UFUNCTION(CallInEditor, Category = "CRIT POINT TOOL", DisplayName = Load) void LoadCritPoint();
	UFUNCTION(CallInEditor, Category = "CRIT POINT TOOL", DisplayName = Clear) void ClearCritPoint();
	UFUNCTION(CallInEditor, Category = "CRIT POINT TOOL", DisplayName = Reset) void ResetCritPoint();

#if WITH_EDITOR
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void CheckDeletedCritPoint();
#endif
};
