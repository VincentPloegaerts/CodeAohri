#pragma once

#include "CoreMinimal.h"
#include "ENUM_AxisMode.h"
#include "M_SaveManager.h"
#include "NiagaraComponent.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundCue.h"
#include "STRUCT_FeedBackData.h"
#include "STRUCT_TripleDeluxeFeedBack.h"
#include "P_DT_BaseDataTable.generated.h"

class AP_PatternBase;
struct FPatternData;

USTRUCT()
struct FBaseDataTableJSON
{
	GENERATED_BODY()
	
	UPROPERTY()
		TSubclassOf<AP_PatternBase> patternType;
	UPROPERTY()
		FTripleDeluxeFeedBack tripleFeedBackData;
	UPROPERTY()
		float fDelay = 0.0f;
	//Range between 0 and this added to 'fDelay'
	UPROPERTY()
		float fRangeRandom = 0.0f;

	UPROPERTY()
	bool bDuplicateFX = false;
	UPROPERTY()
	int duplication = 0;
	UPROPERTY()
	bool centerDuplication = false;
	UPROPERTY()
	FVector duplicationOffset = FVector::ZeroVector;
	
	void InitBaseJSON(FBaseDataTableJSON _json)
	{
		patternType = _json.patternType;
		tripleFeedBackData = _json.tripleFeedBackData;
		duplication = _json.duplication;
		centerDuplication = _json.centerDuplication;
		duplicationOffset = _json.duplicationOffset;
	}
};

UCLASS(Abstract)
class INSIDEUS_API UP_DT_BaseDataTable : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Duplication")
	bool bDuplicateFX = false;
	UPROPERTY(EditAnywhere, Category = "Duplication")
	int duplication = 0;
	UPROPERTY(EditAnywhere, Category = "Duplication", meta = (EditCondition = "duplication > 0", EditConditionHides))
	bool centerDuplication = false;
	UPROPERTY(EditAnywhere, Category = "Duplication", meta = (EditCondition = "duplication > 0", EditConditionHides))
	FVector duplicationOffset = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, Category = "Global")
		TSubclassOf<AP_PatternBase> patternType;
	UPROPERTY(EditAnywhere, Category = "Feedback")
		mutable FTripleDeluxeFeedBack tripleFeedBackData = FTripleDeluxeFeedBack();
	//FDD
	UPROPERTY(EditAnywhere,Category="Global")
		float fDelay = 0.0f;
	//Range between 0 and this added to 'fDelay'
	UPROPERTY(EditAnywhere,Category="Global")
		float fRangeRandom = 0.0f;
public:
	FORCEINLINE const TSubclassOf<AP_PatternBase>& GetPatternType() const { return patternType; }

	FORCEINLINE const bool& GetDuplicationFX() const { return bDuplicateFX; }
	FORCEINLINE const int& GetDuplication() const { return duplication; }
	FORCEINLINE const bool& GetCenterDuplication() const { return centerDuplication; }
	FORCEINLINE const FVector& GetDuplicationOffset() const { return duplicationOffset; }
	
	FORCEINLINE const float& GetBehaviorActivationDelay() const { return tripleFeedBackData.startFeedBack.fBehaviorActivationDelay; }
	FORCEINLINE const float& GetfDelay() const { return fDelay; }
	FORCEINLINE const float& GetfRangeRandom() const { return fRangeRandom; }
	FORCEINLINE FTripleDeluxeFeedBack& GetTripleDeluxeFeedBack() const { return tripleFeedBackData; }

#if WITH_EDITOR
public:
	UFUNCTION(CallInEditor, Category = "Save") virtual void SavePattern();
	UFUNCTION(CallInEditor, Category = "Save") virtual void LoadPattern();
	
	FBaseDataTableJSON GetBaseDataTableJSON() const;
	void LoadBaseDataTableJSON(const FBaseDataTableJSON& _json);
	
	virtual void InitializePatternData(FPatternData& _data) const;
#endif
};
