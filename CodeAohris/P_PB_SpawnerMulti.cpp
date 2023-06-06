#include "P_PB_SpawnerMulti.h"

#include "IU_Macros.h"
#include "IU_Player.h"
#include "M_GameMode.h"
#include "P_DT_ZoneDataTable.h"

void UP_PB_SpawnerMulti::InitializeSpawner()
{
	Super::InitializeSpawner();
	if (!zoneDataTable) return;

	spawnerMultiSettings = zoneDataTable->GetSpawnerMultiSettings();
	locationTaken.Empty();

	iSpawned = 0;
	iToSpawn = spawnerMultiSettings.quantityBySpawn.Num();
}

void UP_PB_SpawnerMulti::InitializeSpawner(FPatternData& _patternData)
{
	Super::InitializeSpawner(_patternData);
	
	spawnerMultiSettings = _patternData.GetSpawnerMultiSettings();
	locationTaken.Empty();

	iSpawned = 0;
	iToSpawn = spawnerMultiSettings.quantityBySpawn.Num();
}

void UP_PB_SpawnerMulti::SpawnBehavior()
{
	if (!bIsEnable) return;
	
	SpawnMulti(iSpawned);
}

void UP_PB_SpawnerMulti::SpawnMulti(const int& _index)
{
	if (!owner) return;

	const bool& _isValidArray = spawnerMultiSettings.quantityBySpawn.Num() > 0;

	if (!_isValidArray)
		LOG_ERROR("UP_PB_SpawnerMulti::SpawnMulti -> Array QuantityBySpawn size is 0, so we take default 1 value instead");
	
	const float& _radius = spawnerMultiSettings.fRange / 2.0f;
	const float& _minDistance = spawnerMultiSettings.fMinDistance;
	const float& _amount = _isValidArray ? spawnerMultiSettings.quantityBySpawn[_index] : 1;
	
	const FVector& _center = owner->GetActorLocation();
	const FVector& _right = owner->GetActorRightVector();
	const FVector& _fwd = owner->GetActorForwardVector();
	
	const TSubclassOf<AGPE_Zone>& _type = spawnerSettings.zoneType;
	const FRotator& _rotation = owner->GetActorRotation();

	FTimerManager& _timerManager = GetWorld()->GetTimerManager();
	
	//locationTaken.Empty();	//Uncomment if need to reset locationtaken to check only in one quake
	for (int _current(0); _current < _amount; ++_current)
	{
		FVector _location(0);

		if (!GetLocation(_location, _fwd, _right, _radius, _minDistance))
			continue;

		locationTaken.Add(_location);

		if (spawnerMultiSettings.bSpawnOnPlayer)
		{
			if (const AIU_Player* _player = world->GetAuthGameMode<AM_GameMode>()->GetPlayer())
			{
				const FVector& _playerLocation = _player->GetActorLocation();
				_location.X = _playerLocation.X;
				_location.Y = _playerLocation.Y;
				_location.Z = _center.Z;
			}
		}
		else
			_location += _center;
		
		FTimerHandle _timer;
		FTimerDelegate _delegate;
		_delegate.BindUFunction(this, "SpawnZoneGPE", _type, _location, _rotation);
		_timerManager.SetTimer(_timer, _delegate, FMath::RandRange(0.01f, 0.03f), false);
	}
}

bool UP_PB_SpawnerMulti::IsLocationCorrect(const FVector& _location, const float& _minDistance) const
{
	const float& _max = locationTaken.Num();
	
	for (int i(0); i < _max; ++i)
	{
		const FVector& _taken = locationTaken[i];
		if (FVector::Dist(_taken, _location) < _minDistance)
			return false;
	}
	return true;
}

bool UP_PB_SpawnerMulti::GetLocation(FVector& _location, const FVector& _fwd, const FVector& _right, const float& _radius, const float& _minDistance, int _currentTry)
{
	if (_currentTry >= 10)	//Max try
		return false;

	_location = _right * FMath::RandRange(-_radius, _radius) + _fwd * FMath::RandRange(-_radius, _radius);
	
	if (IsLocationCorrect(_location, _minDistance) || spawnerMultiSettings.bSpawnOnPlayer)
		return true;
	
	_currentTry++;	
	return GetLocation(_location, _fwd, _right, _radius, _minDistance, _currentTry);
}