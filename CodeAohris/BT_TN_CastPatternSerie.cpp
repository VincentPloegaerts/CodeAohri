#include "BT_TN_CastPatternSerie.h"

#if WITH_EDITORONLY_DATA
#include "EditorWidgetCreator/EditorWidgetCreatorLibrary.h"
#include "EditorWidgetCreator/EditorUtilityWidget_Creator.h"
#endif

#include "BT_BlackBoard.h"
#include "BT_D_CheckBossLife.h"
#include "IU_Macros.h"
#include "B_AIBoss.h"
#include "B_Boss.h"
#include "B_BossMovement.h"
#include "P_PatternCaster.h"

#include "P_DT_BaseDataTable.h"
#include "P_DT_SerieDataTable.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTService.h"

UBT_TN_CastPatternSerie::UBT_TN_CastPatternSerie()
{
#if WITH_EDITOR
	const ConstructorHelpers::FObjectFinder<UWidgetBlueprint> _blueprint(TEXT("/EditorWidgetCreator/BP_EditorUtilityWidget_Creator.BP_EditorUtilityWidget_Creator"));
	WidgetBlueprint = _blueprint.Object;
	
	
#endif
}

EBTNodeResult::Type UBT_TN_CastPatternSerie::InitCastTask()
{
	if (!patternCaster)
		return EBTNodeResult::Failed;

	const UBlackboardComponent* _bb = tree->GetBlackboardComponent();
	
	if (_bb->GetValueAsBool("IsStun"))
	{
		boss = patternCaster->GetBoss();
		Event_Add(boss->OnBossStunnedEnd(), UBT_TN_CastPatternSerie::PostStunInitTask);
		Event_Add(boss->OnBossMidLifeAnimEnd(), UBT_TN_CastPatternSerie::PostStunInitTask);
		return EBTNodeResult::InProgress;
	}

	if (_bb->GetValueAsBool("NodeWillWaitForEndOfMovement"))//
	{
		const bool& _isMoving = _bb->GetValueAsBool("IsMoving");
		if (!_isMoving)
		{
			//cast directly
			FeedBackInitTask();
		}
		
		bMovementHasFinish = false;
		
		boss = patternCaster->GetBoss();
		if (boss)
		{
			bossMovement = boss->GetBossMovement();
			if (bossMovement)
			{
				UB_BossMovement::FOnReachGoal& _onReachGoal = bossMovement->OnReachGoal();
				Event_AddUnique(_onReachGoal, UBT_TN_CastPatternSerie::SetMovementHasFinishTrue);
				
				if (_isMoving) // init only if he hasn't shoot
					Event_AddUnique(_onReachGoal, UBT_TN_CastPatternSerie::FeedBackInitTask);
			}
		}
	}
	else
	{
		bMovementHasFinish = true;//
		FeedBackInitTask();
	}
		
	return EBTNodeResult::InProgress;
}

void UBT_TN_CastPatternSerie::PostStunInitTask()
{
	boss = patternCaster->GetBoss();
	Event_Remove(boss->OnBossStunnedEnd(), UBT_TN_CastPatternSerie::PostStunInitTask);
	Event_Remove(boss->OnBossMidLifeAnimEnd(), UBT_TN_CastPatternSerie::PostStunInitTask);
	
	const UBlackboardComponent* _bb = tree->GetBlackboardComponent();
	if (_bb->GetValueAsBool("NodeWillWaitForEndOfMovement"))//
	{
		const bool& _isMoving = _bb->GetValueAsBool("IsMoving");
		if (!_isMoving)
		{
			//shoot directly
			FeedBackInitTask();
		}
		
		bMovementHasFinish = false;
		
		boss = patternCaster->GetBoss();
		if (boss)
		{
			bossMovement = boss->GetBossMovement();
			if (bossMovement)
			{
				UB_BossMovement::FOnReachGoal& _onReachGoal = bossMovement->OnReachGoal();
				Event_AddUnique(_onReachGoal, UBT_TN_CastPatternSerie::SetMovementHasFinishTrue);
				
				if (_isMoving) // init only if he hasn't shoot
					Event_AddUnique(_onReachGoal, UBT_TN_CastPatternSerie::FeedBackInitTask);
			}
		}
	}
	else
	{
		bMovementHasFinish = true;//
		FeedBackInitTask();
	}
}

void UBT_TN_CastPatternSerie::StartCastSerie()
{
	Event_Add(patternCaster->OnSerieElementDataEnd(), UBT_TN_CastPatternSerie::OnSerieElementEnd);
	Event_Add(patternCaster->OnSerieAndPatternsEnd(), UBT_TN_CastPatternSerie::OnSerieEndButCheckMovementEnd);
	Event_Add(patternCaster->OnSerieEnd(), UBT_TN_CastPatternSerie::OnSerieEndButCheckMovementEnd);
	Event_Add(patternCaster->OnAllWaitingPatternFinish(), UBT_TN_CastPatternSerie::OnStartForWaitingPattern);
	
	patternCaster->CastSerie(serieData);
}

void UBT_TN_CastPatternSerie::FeedBackInitTask()//
{
	if (feedbackSerie.fBehaviorActivationDelay > 0)
	{
		FTimerHandle _handle;
		GetWorld()->GetTimerManager().SetTimer(_handle,this,&UBT_TN_CastPatternSerie::StartCastSerie,feedbackSerie.fBehaviorActivationDelay,false);
	}
	else
		StartCastSerie();
}

void UBT_TN_CastPatternSerie::SetMovementHasFinishTrue()
{
	Event_Remove(bossMovement->OnReachGoal(), UBT_TN_CastPatternSerie::SetMovementHasFinishTrue);
	bMovementHasFinish = true;
}

void UBT_TN_CastPatternSerie::OnSerieEnd()
{
	Event_Remove(patternCaster->OnSerieElementDataEnd(), UBT_TN_CastPatternSerie::OnSerieElementEnd);
	Event_Remove(patternCaster->OnSerieAndPatternsEnd(), UBT_TN_CastPatternSerie::OnSerieEndButCheckMovementEnd);
	Event_Remove(patternCaster->OnSerieEnd(), UBT_TN_CastPatternSerie::OnSerieEndButCheckMovementEnd);
	Event_Remove(patternCaster->OnAllWaitingPatternFinish(), UBT_TN_CastPatternSerie::OnStartForWaitingPattern);
	
	if (bossMovement)//
	{
		UB_BossMovement::FOnReachGoal& _onReachGoal = bossMovement->OnReachGoal();
		Event_Remove(_onReachGoal, UBT_TN_CastPatternSerie::SetMovementHasFinishTrue);
		Event_Remove(_onReachGoal, UBT_TN_CastPatternSerie::FeedBackInitTask);
		Event_Remove(_onReachGoal, UBT_TN_CastPatternSerie::OnSerieEnd);
	}
	
	serieData.iIndex = 0;
	
	tree->OnTaskFinished(this, EBTNodeResult::Succeeded);
}

void UBT_TN_CastPatternSerie::OnSerieEndButCheckMovementEnd()//
{
	if (!bMovementHasFinish)
	{
		Event_AddUnique(bossMovement->OnReachGoal(), UBT_TN_CastPatternSerie::OnSerieEnd);
		return;
	}
	OnSerieEnd();
}

void UBT_TN_CastPatternSerie::OnSerieElementEnd()
{
	++serieData.iIndex;
	patternCaster->CastSerie(serieData);
}

void UBT_TN_CastPatternSerie::OnStartForWaitingPattern()
{
	patternCaster->WaitSerieDataElementDelay(serieData);
}

void UBT_TN_CastPatternSerie::SkipSerie()
{
	ROG(!patternCaster, "UBT_TN_CastPatternSerie::SkipSerie -> Invalid pattern caster");
	
	if (AB_Boss* _boss = patternCaster->GetBoss())
	{
		Event_Remove(boss->OnBossStunnedEnd(), UBT_TN_CastPatternSerie::PostStunInitTask);
		Event_Remove(boss->OnBossMidLifeAnimEnd(), UBT_TN_CastPatternSerie::PostStunInitTask);
	}
	else
	{
		LOG("UBT_TN_CastPatternSerie::SkipSerie -> Invalid boss in pattern serie caster");
		return;
	}

	OnSerieEnd();
}

void UBT_TN_CastPatternSerie::PostInitProperties()
{
	Super::PostInitProperties();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	NodeName = FString("Cast Pattern Serie");
#endif
}

FString UBT_TN_CastPatternSerie::GetStaticDescription() const
{
	const FString& _serieName = serie ? serie->GetName() : FString("NONE");

	return FString("Cast Pattern Serie : ") + _serieName + FString(" ");
}

#pragma region SerieData Generation
#if WITH_EDITOR
void UBT_TN_CastPatternSerie::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == FName("serie"))
	{
		GenerateSerieData();
	}
}

void UBT_TN_CastPatternSerie::GenerateSerieData()
{
	serieData.serieElementDatas.Empty();
		
	if (!serie) return;
	const TArray<FSerieElement>& _serieElements = serie->GetSerieElements();

	const int& _max = _serieElements.Num();
	for (int i = 0; i < _max; i++)
	{
		const FSerieElement& _element = _serieElements[i];
		const FSerieElementData& _elementData = GetSerieElementData(_element);
		//UE_LOG(LogTemp, Warning, TEXT("FSerieElementData Generated"));
		serieData.AddSerieElementData(_elementData);
	}

	//UE_LOG(LogTemp, Warning, TEXT("FSeriaData Generated"));
}

void UBT_TN_CastPatternSerie::SaveSerieData()
{
	UM_SaveManager::Save(serieData, Save_BehaviorTree_Task, GetName());
}

void UBT_TN_CastPatternSerie::LoadSerieData()
{
	UM_SaveManager::Load(serieData, Save_BehaviorTree_Task, GetName());
}

FPatternData UBT_TN_CastPatternSerie::GetPatternData(const UP_DT_BaseDataTable* _pattern) const
{
	FPatternData _patternData;
	if (!_pattern) return _patternData;
	
	_pattern->InitializePatternData(_patternData);
	
	return _patternData;
}

void UBT_TN_CastPatternSerie::OverrideSerie()
{
#if WITH_EDITOR
	UA_DT_ArenaDataTable* _arenaDataTable = nullptr;
	if (const UBT_BlackBoard* _blackBoard = Cast<UBT_BlackBoard>(GetTreeAsset()->GetBlackboardAsset()))
		_arenaDataTable = _blackBoard->GetArenaDataTable();
	
	UEditorUtilityWidget_Creator* _utilityWidget = UEditorWidgetCreatorLibrary::CreateWidget(WidgetBlueprint);
	if (!_utilityWidget) return;
	
	UUserWidget* _widget =  _utilityWidget->InitializeWidget(overrideWidgetType);
	UEditor_OverrideSerieWidget* _overrideWidget  = Cast<UEditor_OverrideSerieWidget>(_widget);
	if (!_overrideWidget) return;
	
	_overrideWidget->InitializeOverrideData(serieData, _arenaDataTable);
	
	_overrideWidget->OnSaveAll().AddDynamic(this, &UBT_TN_CastPatternSerie::SetSerieData);
#endif
}

void UBT_TN_CastPatternSerie::SetSerieData(FSerieData _data)
{
	serieData = _data;
}

FSerieElementData UBT_TN_CastPatternSerie::GetSerieElementData(const FSerieElement& _element) const
{
	FSerieElementData _serieElementData;
	
	const TArray<UP_DT_BaseDataTable*>& _elementPatterns = _element.GetPatterns();
	const FSpawnSettings& _spawnSettings = _element.spawnSettings;
	
	const int& _max = _elementPatterns.Num();

	for (int i = 0; i < _max; ++i)
	{
		const UP_DT_BaseDataTable* _pattern = _elementPatterns[i];
		if (!_pattern) continue;
		
		const FPatternData& _patternData = GetPatternData(_pattern);
		
		const FPatternElementData _patternElementData(_patternData, _spawnSettings);
		//UE_LOG(LogTemp, Warning, TEXT("FPatternElementData Generated"));
		
		_serieElementData.AddData(_patternElementData);
	}
	
	_serieElementData.SetDelay(_element.fDelay);
	_serieElementData.SetIsWaiting(_element.bWaitEndOfPatterns);
	return _serieElementData;
}
#endif
#pragma endregion 