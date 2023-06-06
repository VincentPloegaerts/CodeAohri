#include "P_PatternCaster.h"

#include "M_GameMode.h"
#include "P_PatternBase.h"
#include "P_DT_BaseDataTable.h"
#include "M_SpawnPointManager.h"
#include "P_PatternSocket.h"
#include "B_Boss.h"

#include "Kismet/KismetMathLibrary.h"
#include "IU_Macros.h"
#include "M_BlackBoardManager.h"
#include "M_BossManager.h"

UP_PatternCaster::UP_PatternCaster()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UP_PatternCaster::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	timerManager = &world->GetTimerManager();

	patternSocket = world->SpawnActor<AP_PatternSocket>(AP_PatternSocket::StaticClass());
	
	timerManager->SetTimer(initTimer, this, &UP_PatternCaster::InitializeManagers, 1, false, 0.05f);
}

void UP_PatternCaster::InitializeManagers()
{
	if (const AM_GameMode* _gameMode = world->GetAuthGameMode<AM_GameMode>())
	{
		spawnPointManager = _gameMode->GetSpawnPointManager();
		patternsTickManager = _gameMode->GetPatternsTickManager();
		feedbackManager = _gameMode->GetFeedbackManager();
		blackBoardManager = _gameMode->GetBlackBoardManager();
		
		if (AM_BossManager* _bossManager = _gameMode->GetBossManager())
		{
			boss = _bossManager->GetCurrentBoss();
			Event_AddUnique(_bossManager->OnBossChanged(), UP_PatternCaster::InitializeBoss);
			bIsOwnerBoss = boss == GetOwner();
		}
	}
}

void UP_PatternCaster::InitializeBoss(AB_Boss* _boss)
{
	boss = _boss;
	bIsOwnerBoss = boss == GetOwner();
}

void UP_PatternCaster::CastPattern(UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack)
{
	ROG_ERROR(!spawnPointManager, "UP_PatternCaster::CastPattern -> Invalid spawn point manager");
	ROG_ERROR(!patternSocket, "UP_PatternCaster::CastPattern -> Invalid pattern socket");
	ROG_ERROR(!_dataTable, "UP_PatternCaster::CastPattern -> Invalid dataTable");
	
	const TSubclassOf<AP_PatternBase>& _patternType = _dataTable->GetPatternType();
	ROG_ERROR(!_patternType, "UP_PatternCaster::CastPattern -> Invalid pattern type");
	if (_spawnSettings.eSpawnPosition == ESpawnSettings::SOCKET)
	{
		CastPatternOnSocket(_dataTable, _spawnSettings, _overridedFeedBack);
		return;
	}
	
	const AActor* _spawnPoint = spawnPointManager->GetPoint(_spawnSettings.eSpawnPosition, _spawnSettings.GetSpawnPointName());
	ROG_ERROR(!_spawnPoint, "UP_PatternCaster::CastPattern -> Spawn Point Actor (%s) is nullptr (location)", *_spawnSettings.GetSpawnPointName());
	
	const FVector& _spawnPointLocation = _spawnPoint->GetActorLocation();
	
	const float& _range = _spawnSettings.fRandomRange;
	const float& _randomRange = FMath::RandRange(-_range, _range);
	const float& randomRadius = FMath::RandRange(-_randomRange, _randomRange);

	const FVector& _right = _spawnPoint->GetActorRightVector();
	const FVector& _up = _spawnPoint->GetActorUpVector();
	
	const FVector& _spawnLocation = _spawnPointLocation + _spawnSettings.vSpawnPositionOffset + _right * randomRadius + _up * randomRadius;	

	const FRotator& _spawnRotation = GetRotation(_spawnSettings, _spawnPoint, _spawnPointLocation);	
	
	if (AP_PatternBase* _pattern = world->SpawnActor<AP_PatternBase>(_patternType, _spawnLocation, _spawnRotation))
	{
		_pattern->SetPatternsTickManager(patternsTickManager);
		_pattern->SetFeedbackManager(feedbackManager);
		_pattern->OnComplete().AddDynamic(this, &UP_PatternCaster::PatternBehaviorCompleted);
		_pattern->DelayedInitializePattern(_dataTable, _overridedFeedBack);
		_pattern->SetPatternIndex(iPatternIndex);
		++iPatternIndex;

		onPatternDataTableCasted.Broadcast(_dataTable, _pattern);
		
		if (_spawnSettings.bRotateToSpawnDirection)
		{
			const FVector& _loc = spawnPointManager->GetPointLocation(_spawnSettings.eSpawnDirection, _spawnSettings.directionArenaPointName);
			onTargetLocation.Broadcast(_loc);
		}
		else
		{
			onTargetLocation.Broadcast(_spawnPointLocation);
		}
		
		allPatternBase.Add(_pattern);

		const bool& _center = _dataTable->GetCenterDuplication();

		if (_center)
			DuplicatePatternCenter(_pattern, _spawnLocation, _dataTable, _spawnSettings, _overridedFeedBack);
		else
			DuplicatePattern(_pattern, _spawnLocation, _dataTable, _spawnSettings, _overridedFeedBack);
		
	}
}

void UP_PatternCaster::CastSerie(FSerieData& _serieData/*, const FSpawnSettings& _spawnSettings*/)
{
	if (!spawnPointManager)
	{
		LOG("UP_PatternCaster::CastSerie -> Invalid spawn point manager");
		return;
	}
	if (!patternSocket)
	{
		LOG("UP_PatternCaster::CastPattern -> Invalid pattern socket");
		return;
	}
	
	TArray<FSerieElementData>& _serieElementDatas = _serieData.serieElementDatas;
	if (_serieElementDatas.IsEmpty())
	{
		LOG("UP_PatternCaster::CastSerie -> Serie Element Data Array is empty");
		return;
	}
	const uint& _serieElementDataIndex = _serieData.iIndex;
	const uint& _serieElementDatasNum = _serieElementDatas.Num();
	if (_serieElementDataIndex >= _serieElementDatasNum)
	{
		LOG("UP_PatternCaster::CastSerie -> Serie Data index out of bound");
		return;
	}
	
	FSerieElementData& _serieElementData = _serieElementDatas[_serieElementDataIndex];
	TArray<FPatternElementData>& _patternDatas = _serieElementData.patternsElementData;
	const bool& _waitEndOfPattern = _serieElementData.bWaitEndOfPatterns;
	const uint& _max = _patternDatas.Num();

	for (uint i(0); i < _max; ++i)
	{
		FPatternElementData& _currentPatternElementData = _patternDatas[i];
		FPatternData& _currentPatternData = _currentPatternElementData.patternData;
		FSpawnSettings& _currentSpawnSettings = _currentPatternElementData.spawnSettings;
		
		const TSubclassOf<AP_PatternBase>& _patternType = _currentPatternData.GetPatternType();
		if (!_patternType)
		{
			LOG("UP_PatternCaster::CastSerie -> Invalid pattern type");
			return;
		}
		
		//No locSpawnPoint if SpawnSettings == SOCKET
		const AActor* _locSpawnPoint = spawnPointManager->GetPoint(_currentSpawnSettings.eSpawnPosition, _currentSpawnSettings.GetSpawnPointName());
		if (!_locSpawnPoint)
		{
			LOG("UP_PatternCaster::CastSerie -> Spawn Point Actor is nullptr (location)");
			return;
		}
		
		const FVector& _locTarget = _locSpawnPoint->GetActorLocation();
	
		const float& _radius = _currentSpawnSettings.fRandomRange;
		const float&_randomRadius = FMath::RandRange(-_radius, _radius);

		const FVector& _right = _locSpawnPoint->GetActorRightVector();
		const FVector& _up = _locSpawnPoint->GetActorUpVector();

		const float& _random = FMath::RandRange(-_randomRadius, _randomRadius);
	
		const FVector& _location = _locTarget + _currentSpawnSettings.vSpawnPositionOffset + _right * _random + _up * _random;	

		FRotator _rotation;
		if (_currentSpawnSettings.eSpawnDirectionMode == TARGET)
		{
			const FVector& _spawnPointLocation = spawnPointManager->GetPointLocation(_currentSpawnSettings.eSpawnDirection, _currentSpawnSettings.GetDirectionPointName());
		
			const FVector& _lookVector = _spawnPointLocation - _locTarget;
		
			patternSocket->SetActorLocation(_locTarget);
			patternSocket->SetActorRotation(Get_LookRotation(_lookVector));
		
			const FVector& _lookTargetOffset = _currentSpawnSettings.vLookTargetOffset;
			const FVector& _addOffset = patternSocket->GetActorForwardVector() * _lookTargetOffset.X +
										patternSocket->GetActorRightVector() * _lookTargetOffset.Y +
										patternSocket->GetActorUpVector() * _lookTargetOffset.Z;
		
			_rotation = Get_LookRotation(_lookVector + _addOffset);
		}
		else
		{
			_rotation = _locSpawnPoint->GetActorRotation();
			_rotation += _currentSpawnSettings.rSpawnLocalDirectionOffset;
		}
		
		
		if (AP_PatternBase* _pattern = world->SpawnActor<AP_PatternBase>(_patternType, _location, _rotation))
		{
			_pattern->SetPatternsTickManager(patternsTickManager);
			_pattern->SetFeedbackManager(feedbackManager);
			_pattern->OnComplete().AddDynamic(this, &UP_PatternCaster::PatternBehaviorCompleted);
			_pattern->DelayedInitializePatternData(_currentPatternData);
			
			onTargetLocation.Broadcast(_locTarget);
			
			allPatternBase.Add(_pattern);
			if (_waitEndOfPattern)
			{
				allWaitingPatternBase.Add(_pattern);
			}
		}
	}
	
	if (!_waitEndOfPattern)
	{
		FTimerHandle _timer;
		if (_serieElementDataIndex + 1 >= _serieElementDatasNum)
		{
			timerManager->SetTimer(_timer, this, &UP_PatternCaster::WaitEndOfSerie, _serieElementData.fDelay);
		}
		else
		{
			timerManager->SetTimer(_timer, this, &UP_PatternCaster::WaitForNextSerie, _serieElementData.fDelay);
		}
	}
}

void UP_PatternCaster::DestroyEveryPattern()
{
	while (!allPatternBase.IsEmpty())
	{
		AP_PatternBase* _pattern = allPatternBase[0];
		allPatternBase.Remove(_pattern);
		_pattern->Destroy();
	}
	allPatternBase.Shrink();

	while (!allPatternDuplication.IsEmpty())
	{
		AP_PatternBase* _pattern = allPatternDuplication[0];
		allPatternDuplication.Remove(_pattern);
		_pattern->Destroy();
	}
	allPatternDuplication.Shrink();
	
	while (!allWaitingPatternBase.IsEmpty())
	{
		AP_PatternBase* _pattern = allWaitingPatternBase[0];
		allWaitingPatternBase.Remove(_pattern);
		_pattern->Destroy();
	}
	allWaitingPatternBase.Shrink();

	onPatternListEmpty.Broadcast();
}

void UP_PatternCaster::ClearTimer() const
{
	timerManager->ClearAllTimersForObject(this);
}

void UP_PatternCaster::BroadcastFinishedDataTable(const UP_DT_BaseDataTable* _dataTable)
{
#if WITH_LOG_ANIM_FEEDBACK
	LOG_ERROR("-------------------------------------------------------Pattern End-----------------------------------------------------");
#endif
	onDataTableEnd.Broadcast(_dataTable);
}

void UP_PatternCaster::CastPatternOnSocket(UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack)
{
	const TSubclassOf<AP_PatternBase>& _patternType = _dataTable->GetPatternType();
	ROG_ERROR(!_patternType, "UP_PatternCaster::CastPattern -> Invalid pattern type");
	
	if (AP_PatternBase* _pattern = world->SpawnActor<AP_PatternBase>(_patternType, FVector::ZeroVector, FRotator::ZeroRotator))
	{
		spawnPointManager->AttachToSocket(*_spawnSettings.GetSpawnPointName(), _pattern);
		_pattern->SetActorRotation(GetRotation(_spawnSettings, _pattern, _pattern->GetActorLocation()));
		
		_pattern->SetPatternsTickManager(patternsTickManager);
		_pattern->SetFeedbackManager(feedbackManager);
		_pattern->OnComplete().AddDynamic(this, &UP_PatternCaster::PatternBehaviorCompleted);
		_pattern->DelayedInitializePattern(_dataTable, _overridedFeedBack);
		_pattern->SetPatternIndex(iPatternIndex);
		++iPatternIndex;

		onPatternDataTableCasted.Broadcast(_dataTable, _pattern);
		
		if (_spawnSettings.bRotateToSpawnDirection)
		{
			const FVector& _loc = spawnPointManager->GetPointLocation(_spawnSettings.eSpawnDirection, _spawnSettings.directionArenaPointName);
			onTargetLocation.Broadcast(_loc);
		}
		else
		{
			onTargetLocation.Broadcast(_pattern->GetActorLocation());
		}
		
		allPatternBase.Add(_pattern);

		const bool& _center = _dataTable->GetCenterDuplication();

		if (_center)
			DuplicatePatternCenterSocket(_pattern, _dataTable, _spawnSettings, _overridedFeedBack);
		else
			DuplicatePatternSocket(_pattern, _dataTable, _spawnSettings, _overridedFeedBack);
	}
}

void UP_PatternCaster::DuplicatePatternSocket(const AP_PatternBase* _initialPattern, UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack)
{
	const int& _duplication = _dataTable->GetDuplication();
	if (_duplication <= 0) return;

	const TSubclassOf<AP_PatternBase>& _patternType = _dataTable->GetPatternType();
	const FVector& _initialLocation = FVector::ZeroVector;
	const FVector& _fwd = _initialPattern->GetActorForwardVector();
	const FVector& _right = _initialPattern->GetActorRightVector();
	const FVector& _up = _initialPattern->GetActorUpVector();	
	const FVector& _duplicateOffset = _dataTable->GetDuplicationOffset();
    const FVector& _offset = _fwd * _duplicateOffset.X + _right * _duplicateOffset.Y + _up * _duplicateOffset.Z;
	
	for (int i = 0; i < _duplication; i++)
	{
		if (AP_PatternBase* _duplicatePattern = world->SpawnActor<AP_PatternBase>(_patternType, _initialLocation, FRotator::ZeroRotator))
		{
			spawnPointManager->AttachToSocket(*_spawnSettings.GetSpawnPointName(), _duplicatePattern);
			const FRotator& _duplicationRotation = _initialPattern->GetActorRotation();
			_duplicatePattern->SetActorRotation(_duplicationRotation);
			const FVector& _offsetLocation = _offset * (i + 1);
			_duplicatePattern->AddActorLocalOffset(_offsetLocation);
			
			_duplicatePattern->SetPatternsTickManager(patternsTickManager);
			_duplicatePattern->SetFeedbackManager(feedbackManager);
			_duplicatePattern->OnComplete().AddDynamic(this, &UP_PatternCaster::DuplicatePatternCompleted);
			_duplicatePattern->DelayedInitializePattern(_dataTable, _overridedFeedBack, true, _dataTable->GetDuplicationFX());

			allPatternDuplication.Add(_duplicatePattern);
		}
	}
}

void UP_PatternCaster::DuplicatePatternCenterSocket(const AP_PatternBase* _initialPattern, UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack)
{
	const int& _duplication = _dataTable->GetDuplication();
	if (_duplication <= 0) return;

	const TSubclassOf<AP_PatternBase>& _patternType = _dataTable->GetPatternType();
	const FVector& _fwd = _initialPattern->GetActorForwardVector();
	const FVector& _right = _initialPattern->GetActorRightVector();
	const FVector& _up = _initialPattern->GetActorUpVector();
	
	const FVector& _duplicateOffset = _dataTable->GetDuplicationOffset();
	const FVector& _offset = _fwd * _duplicateOffset.X + _right * _duplicateOffset.Y + _up * _duplicateOffset.Z;
	
	for (int i = 0; i < _duplication / 2; i++)
	{
		for (int inverse = 0; inverse <= 2; inverse++)
		{
			if (AP_PatternBase* _duplicatePattern = world->SpawnActor<AP_PatternBase>(_patternType, FVector::ZeroVector, FRotator::ZeroRotator))
			{
				spawnPointManager->AttachToSocket(*_spawnSettings.GetSpawnPointName(), _duplicatePattern);
				const FRotator& _duplicationRotation = _initialPattern->GetActorRotation();
				_duplicatePattern->SetActorRotation(_duplicationRotation);
				const FVector& _offsetLocation = _offset * (i + 1);
				_duplicatePattern->AddActorLocalOffset(_offsetLocation * (inverse ? 1 : -1));
			
				_duplicatePattern->SetPatternsTickManager(patternsTickManager);
				_duplicatePattern->SetFeedbackManager(feedbackManager);
				_duplicatePattern->OnComplete().AddDynamic(this, &UP_PatternCaster::DuplicatePatternCompleted);
				_duplicatePattern->DelayedInitializePattern(_dataTable, _overridedFeedBack, true, _dataTable->GetDuplicationFX());

				allPatternDuplication.Add(_duplicatePattern);
			}
		}
	}
}

void UP_PatternCaster::DuplicatePattern(const AP_PatternBase* _initialPattern, const FVector& _initialLocation, UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack)
{
	const int& _duplication = _dataTable->GetDuplication();
	if (_duplication <= 0) return;
	
	const TSubclassOf<AP_PatternBase>& _patternType = _dataTable->GetPatternType();
	const FVector& _fwd = _initialPattern->GetActorForwardVector();
	const FVector& _right = _initialPattern->GetActorRightVector();
	const FVector& _up = _initialPattern->GetActorUpVector();

	const FVector& _duplicateOffset = _dataTable->GetDuplicationOffset();
	const FVector& _offset = _fwd * _duplicateOffset.X + _right * _duplicateOffset.Y + _up * _duplicateOffset.Z;
	
	for (int i = 0; i < _duplication; i++)
	{
		const FVector& _offsetLocation = _offset * (i + 1);
		const FVector& _duplicationLocation = _initialLocation + _offsetLocation;

		const FRotator& _duplicationRotation = _initialPattern->GetActorRotation();
		
		SpawnPatternDuplicata(_patternType, _duplicationLocation, _duplicationRotation, _dataTable, _overridedFeedBack);
	}
}

void UP_PatternCaster::DuplicatePatternCenter(AP_PatternBase* _initialPattern, const FVector& _initialLocation, UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack)
{
	const int& _duplication = _dataTable->GetDuplication();
	if (_duplication <= 0) return;
	
	const TSubclassOf<AP_PatternBase>& _patternType = _dataTable->GetPatternType();
	const FVector& _fwd = _initialPattern->GetActorForwardVector();
	const FVector& _right = _initialPattern->GetActorRightVector();
	const FVector& _up = _initialPattern->GetActorUpVector();

	const FVector& _duplicateOffset = _dataTable->GetDuplicationOffset();
	const FVector& _offset = _fwd * _duplicateOffset.X + _right * _duplicateOffset.Y + _up * _duplicateOffset.Z;
	
	for (int i = 0; i < _duplication / 2; i++)
	{
		const FVector& _offsetLocation = _offset * (i + 1);

		for (int inverse = 0; inverse <= 2; inverse++)
		{
			const FVector& _duplicationLocation = _initialLocation + _offsetLocation * (inverse ? 1 : -1);
			const FRotator& _duplicationRotation = _initialPattern->GetActorRotation();

			SpawnPatternDuplicata(_patternType, _duplicationLocation, _duplicationRotation, _dataTable, _overridedFeedBack);
		}
	}
}

AActor* UP_PatternCaster::SpawnPatternDuplicata(const TSubclassOf<AP_PatternBase>& _patternType, const FVector& _location, const FRotator& _rotation, UP_DT_BaseDataTable* _dataTable, const FTripleDeluxeFeedBack* _overridedFeedBack)
{
	if (AP_PatternBase* _duplicatePattern = world->SpawnActor<AP_PatternBase>(_patternType, _location, _rotation))
	{
		_duplicatePattern->SetPatternsTickManager(patternsTickManager);
		_duplicatePattern->SetFeedbackManager(feedbackManager);
		_duplicatePattern->OnComplete().AddDynamic(this, &UP_PatternCaster::DuplicatePatternCompleted);
		_duplicatePattern->DelayedInitializePattern(_dataTable, _overridedFeedBack, true, _dataTable->GetDuplicationFX());

		allPatternDuplication.Add(_duplicatePattern);

		return _duplicatePattern;
	}
	return nullptr;
}

FVector UP_PatternCaster::GetAxisDirection(const EAxisMode& _axis, const AActor* _actor) const
{
	if (_axis == EAxisMode::FORWARD_AXIS)
		return _actor->GetActorForwardVector();
	if (_axis == EAxisMode::RIGHT_AXIS)
		return _actor->GetActorRightVector();
	if (_axis == EAxisMode::UP_AXIS)
		return _actor->GetActorUpVector();
	
	return FVector::Zero();
}

FRotator UP_PatternCaster::GetRotation(const FSpawnSettings& _spawnSettings, const AActor* _spawnPoint, const FVector& _spawnPointLocation) const
{
	FRotator _spawnRotation = _spawnPoint->GetActorRotation();
	
	if (_spawnSettings.eSpawnDirectionMode == TARGET)
	{
		const FVector& _directionPointLocation = spawnPointManager->GetPointLocation(_spawnSettings.eSpawnDirection, _spawnSettings.GetDirectionPointName());
		FVector _direction(_directionPointLocation - _spawnPointLocation);

		if (_spawnSettings.bRotateOnlyZ)
			_direction.Z = 0;
		
		patternSocket->SetActorLocation(_spawnPointLocation);
		patternSocket->SetActorRotation(Get_LookRotation(_direction));
			
		const FVector& _lookTargetOffset = _spawnSettings.vLookTargetOffset;
		const FVector& _addOffset = patternSocket->GetActorForwardVector() * _lookTargetOffset.X +
									patternSocket->GetActorRightVector() * _lookTargetOffset.Y +
									patternSocket->GetActorUpVector() * _lookTargetOffset.Z;
			
		_spawnRotation = Get_LookRotation(_direction + _addOffset);
	}
	else
		_spawnRotation += _spawnSettings.rSpawnLocalDirectionOffset;

	return _spawnRotation;
}

void UP_PatternCaster::PatternBehaviorCompleted(AP_PatternBase* _pattern)
{
	allPatternBase.Remove(_pattern);
	allPatternBase.Shrink();
	if (allPatternBase.IsEmpty())
	{
		onPatternListEmpty.Broadcast();
	}
	
	_pattern->OnComplete().RemoveDynamic(this, &UP_PatternCaster::PatternBehaviorCompleted);
	_pattern->Destroy();
	
	if (allWaitingPatternBase.Remove(_pattern) > 0)
	{
		allWaitingPatternBase.Shrink();
		if (allWaitingPatternBase.IsEmpty())
		{
			onAllWaitingPatternFinish.Broadcast();
		}
	}
	else
	{
		if (blackBoardManager && bIsOwnerBoss)
			blackBoardManager->Broadcast_PatternCaster_OnPatternComplete();
		onPatternComplete.Broadcast(_pattern);
	}
}

void UP_PatternCaster::DuplicatePatternCompleted(AP_PatternBase* _pattern)
{
	allPatternDuplication.Remove(_pattern);
	allPatternDuplication.Shrink();
	
	_pattern->OnComplete().RemoveDynamic(this, &UP_PatternCaster::DuplicatePatternCompleted);
	_pattern->Destroy();
}

void UP_PatternCaster::WaitSerieDataElementDelay(const FSerieData& _serieData)
{
	const TArray<FSerieElementData>& _serieElementDatas = _serieData.serieElementDatas;
	if (_serieElementDatas.IsEmpty())
	{
		LOG("UP_PatternCaster::WaitSerieDataElementDelay -> Serie Element Data Array is empty");
		return;
	}
	const uint& _serieElementDataIndex = _serieData.iIndex;

	if (_serieElementDataIndex >= (uint)_serieElementDatas.Num())return;
	const FSerieElementData& _serieElementData = _serieElementDatas[_serieElementDataIndex];

	FTimerHandle _timer;
	if (_serieElementDataIndex + 1 >= (uint)_serieElementDatas.Num())
	{
		timerManager->SetTimer(_timer, this, &UP_PatternCaster::OnSerieAndPatternEndWaitingDelay, _serieElementData.fDelay);
	}
	else
	{
		timerManager->SetTimer(_timer, this, &UP_PatternCaster::WaitForNextSerie, _serieElementData.fDelay);
	}
}

void UP_PatternCaster::WaitForNextSerie()
{
	onSerieElementDataEnd.Broadcast();
}

void UP_PatternCaster::WaitEndOfSerie()
{
	onSerieEnd.Broadcast();
}

void UP_PatternCaster::OnSerieAndPatternEndWaitingDelay()
{
	onSerieAndPatternsEnd.Broadcast();
}