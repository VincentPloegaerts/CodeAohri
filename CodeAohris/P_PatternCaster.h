#pragma once

#include "CoreMinimal.h"
#include "IU_Macros.h"
#include "STRUCT_SerieData.h"
#include "STRUCT_SpawnSettings.h"
#include "TimerManager.h"
#include "Components/ActorComponent.h"
#include "P_PatternCaster.generated.h"

class AM_BlackBoardManager;
class AB_Boss;
class AP_PatternBase;
class UP_DT_BaseDataTable;
class AM_PatternsTickManager;
class AM_SpawnPointManager;
class AP_PatternSocket;
class AM_FeedbackManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UP_PatternCaster : public UActorComponent
{
	GENERATED_BODY()

	friend class AM_BlackBoardManager;
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPatternComplete, const AP_PatternBase*, _pattern); // End Solo pattern
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSerieElementDataEnd); // End of a Serie Element Data using timer
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSerieAndPatternsEnd); // End of the serie and the pattern spawned
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSerieEnd); // End of the serie
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPatternListEmpty); // End of the serie
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllWaitingPatternFinish); // All of the pattern we are waiting to are finish
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetLocation, const FVector&, _location); // BroadCast the location of the target
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataTableEnd, const UP_DT_BaseDataTable*, _dataTable); //BroadCast the TN_Cast end datatable
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPatternDataTableCasted, const UP_DT_BaseDataTable*, _dataTable, AP_PatternBase*, _pattern);

private:
	UPROPERTY(VisibleAnywhere, Category = "Pattern Caster")
	AM_SpawnPointManager* spawnPointManager = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Pattern Caster")
	AM_PatternsTickManager* patternsTickManager = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Pattern Caster")
	AM_FeedbackManager* feedbackManager = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Pattern Caster")
	unsigned iPatternIndex = 0;

	UPROPERTY(VisibleAnywhere) bool bIsOwnerBoss = false;
	UPROPERTY(VisibleAnywhere) AB_Boss* boss = nullptr;
	UPROPERTY()	AP_PatternSocket* patternSocket = nullptr;
	UPROPERTY() AM_BlackBoardManager* blackBoardManager = nullptr;

	UPROPERTY(VisibleAnywhere) TArray<AP_PatternBase*> allWaitingPatternBase;
	UPROPERTY(VisibleAnywhere) TArray<AP_PatternBase*> allPatternBase;
	UPROPERTY(VisibleAnywhere) TArray<AP_PatternBase*> allPatternDuplication;

	//No UPROPERTY, can cause issue with garbage probably
	FTimerManager* timerManager = nullptr;

	UPROPERTY()
	UWorld* world = nullptr;
	
	UPROPERTY()
	FTimerHandle initTimer;
	
protected:
	/*Event used to switch to the next task node.*/
	UPROPERTY(BlueprintCallable, BlueprintReadWrite) FOnPatternComplete onPatternComplete                 = FOnPatternComplete();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite) FOnSerieElementDataEnd onSerieElementDataEnd         = FOnSerieElementDataEnd();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite) FOnSerieAndPatternsEnd onSerieAndPatternsEnd         = FOnSerieAndPatternsEnd();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite) FOnSerieEnd onSerieEnd                               = FOnSerieEnd();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite) FOnPatternListEmpty onPatternListEmpty               = FOnPatternListEmpty();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite) FOnAllWaitingPatternFinish onAllWaitingPatternFinish = FOnAllWaitingPatternFinish();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite) FOnTargetLocation onTargetLocation                   = FOnTargetLocation();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite) FOnDataTableEnd onDataTableEnd                       = FOnDataTableEnd();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite) FOnPatternDataTableCasted onPatternDataTableCasted   = FOnPatternDataTableCasted();
	
public:
	FORCEINLINE FOnPatternComplete& OnPatternComplete() { return onPatternComplete; }
	FORCEINLINE FOnSerieElementDataEnd& OnSerieElementDataEnd() { return onSerieElementDataEnd; }
	FORCEINLINE FOnSerieAndPatternsEnd& OnSerieAndPatternsEnd() { return onSerieAndPatternsEnd; }
	FORCEINLINE FOnSerieEnd& OnSerieEnd() { return onSerieEnd; }
	FORCEINLINE FOnPatternListEmpty& OnPatternListEmpty() { return onPatternListEmpty; }
	FORCEINLINE FOnAllWaitingPatternFinish& OnAllWaitingPatternFinish() { return onAllWaitingPatternFinish; }
	FORCEINLINE FOnTargetLocation& OnTargetLocation() { return onTargetLocation; }
	FORCEINLINE FOnDataTableEnd& OnDataTableEnd() { return onDataTableEnd; }
	FORCEINLINE FOnPatternDataTableCasted& OnPatternDataTableCasted() { return onPatternDataTableCasted; }
	FORCEINLINE const uint& GetPatternIndex() const { return iPatternIndex; }

	FORCEINLINE bool GetIsPatternListEmpty() const { return allPatternBase.IsEmpty(); }

	FORCEINLINE AB_Boss* GetBoss() const { return boss; }

public:	
	UP_PatternCaster();

private:
	virtual void BeginPlay() override;
	void InitializeManagers();
	UFUNCTION() void InitializeBoss(AB_Boss* _boss);
	
public:
	//Cast the current given pattern with spawn settings.
	void CastPattern(UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack);

	//Cast the given serie. Spawn only part of it.
	void CastSerie(FSerieData& _serieData/*, const FSpawnSettings& _spawnSettings*/);

	UFUNCTION() void DestroyEveryPattern();
	UFUNCTION() void ClearTimer() const;

	//Broadcast a datatable we are currently waiting for end.
	UFUNCTION() void BroadcastFinishedDataTable(const UP_DT_BaseDataTable* _dataTable);

private:
	void CastPatternOnSocket(UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack);
	void DuplicatePatternSocket(const AP_PatternBase* _initialPattern, UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack);
	void DuplicatePatternCenterSocket(const AP_PatternBase* _initialPattern, UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack);
	
	void DuplicatePattern(const AP_PatternBase* _initialPattern, const FVector& _initialLocation,  UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack);
	void DuplicatePatternCenter(AP_PatternBase* _initialPattern, const FVector& _initialLocation, UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack);
	AActor* SpawnPatternDuplicata(const TSubclassOf<AP_PatternBase>& _patternType, const FVector& _location, const FRotator& _rotation, UP_DT_BaseDataTable* _dataTable, const FTripleDeluxeFeedBack* _overridedFeedBack);
	
	FVector GetAxisDirection(const EAxisMode& _axis, const AActor* _actor) const;
	FRotator GetRotation(const FSpawnSettings& _spawnSettings, const AActor* _spawnPoint, const FVector& _spawnPointLocation) const;
	
	//The event broadcast function.
	UFUNCTION() void PatternBehaviorCompleted(AP_PatternBase* _pattern);
	//Only used by pattern duplication
	UFUNCTION() void DuplicatePatternCompleted(AP_PatternBase* _pattern);

	UFUNCTION() void WaitForNextSerie();
	UFUNCTION() void WaitEndOfSerie();
	UFUNCTION() void OnSerieAndPatternEndWaitingDelay();

public:
	UFUNCTION() void WaitSerieDataElementDelay(const FSerieData& _serieData);
};