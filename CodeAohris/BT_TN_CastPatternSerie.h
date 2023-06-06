#pragma once

#include "CoreMinimal.h"
#include "BT_TN_Cast.h"
#include "Editor_OverrideSerieWidget.h"
#include "STRUCT_PatternMovementData.h"

#include "STRUCT_SerieData.h"
#include "STRUCT_SerieElement.h"

#if WITH_EDITORONLY_DATA
#include "WidgetBlueprint.h"
#endif

#include "BT_TN_CastPatternSerie.generated.h"

class UB_BossMovement;
class AB_Boss;
class UP_DT_SerieDataTable;

UCLASS()
class INSIDEUS_API UBT_TN_CastPatternSerie : public UBT_TN_Cast
{
	GENERATED_BODY()

protected:
	/*The Serie that will be send to Pattern caster*/
	UPROPERTY(EditAnywhere, Category = "Cast | Serie")
	UP_DT_SerieDataTable* serie = nullptr;

	/*The GIGA STRUCT (contains every settings from the serie)*/
	UPROPERTY(EditAnywhere, Category = "Serie Data Generated")
	FSerieData serieData;

	UPROPERTY(EditAnywhere, Category = "Feedback Serie")
		FFeedBackData feedbackSerie;

#if WITH_EDITORONLY_DATA
	UWidgetBlueprint* WidgetBlueprint = nullptr;

	UPROPERTY(EditAnywhere, Category = "Override")
	TSubclassOf<UEditor_OverrideSerieWidget> overrideWidgetType;
#endif

	
	UPROPERTY()
	AB_Boss* boss = nullptr;

	UPROPERTY()
	UB_BossMovement* bossMovement = nullptr;


protected:
	virtual EBTNodeResult::Type InitCastTask() override;

	UFUNCTION() void PostStunInitTask();
	
	UFUNCTION() void OnSerieEnd();
	UFUNCTION() void OnSerieEndButCheckMovementEnd();
	UFUNCTION() void OnSerieElementEnd();
	UFUNCTION() void OnStartForWaitingPattern();

	UFUNCTION() void StartCastSerie();

	UFUNCTION() void FeedBackInitTask();

	UFUNCTION() void SetMovementHasFinishTrue();

public:
	UFUNCTION() void SkipSerie();

protected:
	virtual void PostInitProperties() override;
	virtual FString GetStaticDescription() const override;

public:
	UBT_TN_CastPatternSerie();

#if WITH_EDITOR
#pragma region SerieData Generation
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
public:
	UFUNCTION(CallInEditor) void GenerateSerieData();
	UFUNCTION(CallInEditor) void SaveSerieData();
	UFUNCTION(CallInEditor) void LoadSerieData();
	
protected:
	FSerieElementData GetSerieElementData(const FSerieElement& _element) const;
	FPatternData GetPatternData(const UP_DT_BaseDataTable* _pattern) const;
#pragma endregion

	UFUNCTION(CallInEditor, Category = "Override") void OverrideSerie();
	UFUNCTION() void SetSerieData(FSerieData _data);
#endif
};