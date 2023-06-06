#include "P_PB_SpawnerConcentric.h"

#include "P_DT_ZoneDataTable.h"

#include "Kismet/KismetMathLibrary.h"
#include "IU_Macros.h"

void UP_PB_SpawnerConcentric::InitializeSpawner()
{
	Super::InitializeSpawner();
	if (!zoneDataTable) return;
	
	spawnerConcentricSettings = zoneDataTable->GetSpawnerConcentricSettings();

	iSpawned = 0;
	iToSpawn = spawnerConcentricSettings.circleSettings.Num();
}

void UP_PB_SpawnerConcentric::InitializeSpawner(FPatternData& _patternData)
{
	Super::InitializeSpawner(_patternData);
	spawnerConcentricSettings = _patternData.GetSpawnerConcentricSettings();

	iSpawned = 0;
	iToSpawn = spawnerConcentricSettings.circleSettings.Num();
}

void UP_PB_SpawnerConcentric::SpawnBehavior()
{
	if (!bIsEnable) return;

	SpawnCircle(iSpawned);
}

void UP_PB_SpawnerConcentric::SpawnCircle(const int& _index)
{
	const float& _radius = GetTotalSpacing(_index);
	const float& _amount = GetSpawnAmount(_index);
	
	const float& _halfRange = 360.0f / 2.0f;

	const FVector& _center = owner->GetActorLocation();
	const FVector& _fwd = owner->GetActorForwardVector();
	const FVector& _up = owner->GetActorUpVector();

	const float& _parse = 360.0f / _amount;

	const TSubclassOf<AGPE_Zone>& _type = spawnerSettings.zoneType;

	FTimerManager& _timerManager = GetWorld()->GetTimerManager();
	
	for	(int _current(0); _current < _amount; ++_current)
	{
		const float& _angle = FMath::Lerp(-_halfRange, _halfRange, _current / float(_amount));
		
		const FVector& _location = _center + _fwd.RotateAngleAxis(_angle + _parse / 2.0f, _up) * _radius;
		const FVector& _direction((_location - _center).GetSafeNormal());

		const FRotator& _rotation = Get_LookRotation(_direction);

		FTimerHandle _timer;
		FTimerDelegate _delegate;
		_delegate.BindUFunction(this, "SpawnZoneGPE", _type, _location, _rotation);
		_timerManager.SetTimer(_timer, _delegate, FMath::RandRange(0.01f, 0.03f), false);
	}
}

float UP_PB_SpawnerConcentric::GetTotalSpacing(const int& _index) const
{
	float _total(0.0f);
	const TArray<FConcentricCircleSettings>& _settings =  spawnerConcentricSettings.circleSettings;
	for (int i(0); i <= _index; ++i)
		_total += _settings[i].spacing;

	return _total;
}

int UP_PB_SpawnerConcentric::GetSpawnAmount(const int& _index) const
{
	return spawnerConcentricSettings.circleSettings[_index].quantity;
}