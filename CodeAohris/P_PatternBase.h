#pragma once

#include "CoreMinimal.h"
#include "IU_Macros.h"
#include "GameFramework/Actor.h"

#include "M_PatternsTickManager.h"
#include "STRUCT_PatternData.h"

#include "P_PatternBase.generated.h"

class UP_DT_BaseDataTable;
class UP_PatternBehavior;
class AM_FeedbackManager;

UCLASS(Abstract)
class INSIDEUS_API AP_PatternBase : public AActor
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComplete, AP_PatternBase*, _pattern);

protected:
	UPROPERTY(VisibleAnywhere)
	AM_PatternsTickManager* patternsTickManager = nullptr;
	
	UPROPERTY()
	AM_FeedbackManager* feedbackManager = nullptr;
	
	UPROPERTY()
	UP_DT_BaseDataTable* dataTable = nullptr;
	
	UPROPERTY()
	unsigned iPatternIndex = 0;
	
	
	UPROPERTY()
	FOnComplete onComplete = FOnComplete();
	
public:
	FORCEINLINE UP_DT_BaseDataTable* GetDataTable() const { return dataTable; }
	FORCEINLINE const uint& GetPatternIndex() const { return iPatternIndex; }
	
	FORCEINLINE void SetPatternsTickManager(AM_PatternsTickManager* _patternsTickManager) { patternsTickManager = _patternsTickManager; }
	FORCEINLINE void SetFeedbackManager(AM_FeedbackManager* _feedbackManager) { feedbackManager = _feedbackManager; }
	FORCEINLINE void SetPatternIndex(const uint& _patternIndex) { iPatternIndex = _patternIndex; }
	
	FORCEINLINE FOnComplete& OnComplete() { return onComplete; }

public:	
	AP_PatternBase();

protected:
	virtual void BeginPlay() override;
	
public:
	//Called by TaskNode from boss BehaviorTree
	void DelayedInitializePattern(UP_DT_BaseDataTable* _dataTable, const FTripleDeluxeFeedBack* _overridedFeedBack, const bool& _isDuplicated = false, const bool& _dupFX = false);
	UFUNCTION() virtual void InitializePattern(UP_DT_BaseDataTable* _dataTable);
	
	UFUNCTION() virtual void InitializePatternData(FPatternData& _patternData);
	UFUNCTION() void DelayedInitializePatternData(FPatternData& _patternData);
	
protected:
	//Initialize the given Pattern Behavior
	void InitializeBehavior(const bool& _state, UP_PatternBehavior* _behavior, UP_DT_BaseDataTable* _dataTable, AM_PatternsTickManager::FOnTick& _onTick);
	void InitializeBehavior(const bool& _state, UP_PatternBehavior* _behavior, FPatternData& _patternData, AM_PatternsTickManager::FOnTick& _onTick);
	
	//Used to deactivate every pattern behavior
	virtual void DeActivatePatternBehavior();
	//DeActivate the given Pattern Behavior
	void DeActivateBehavior(UP_PatternBehavior* _behavior, AM_PatternsTickManager::FOnTick& _onTick);

public:
	//Called when a Pattern Behavior have completed its specific task
	UFUNCTION() virtual void PatternCompletion();
};