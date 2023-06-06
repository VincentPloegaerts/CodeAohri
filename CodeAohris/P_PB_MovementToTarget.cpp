#include "P_PB_MovementToTarget.h"

#include "P_DT_PatternDataTable.h"
#include "M_GameMode.h"
#include "B_Boss.h"
#include "IU_Player.h"
#include "M_ArenaManager.h"
#include "A_Arena.h"

void UP_PB_MovementToTarget::InitializeMovement(UP_DT_PatternDataTable* _dataTable)
{
	Super::InitializeMovement(_dataTable);
	if (!owner) return;
	
	movementToTargetSettings = _dataTable->GetMovementToTargetSettings();
	
	vStartLocation = owner->GetActorLocation();

	const ESpawnSettings& _target = movementToTargetSettings.target;
	
	if (_target == ESpawnSettings::ARENA_POINT)
		vEndLocation = GetArenaPointLocation(movementToTargetSettings.arenaPointName);
	else if (_target == ESpawnSettings::BOSS)
		vEndLocation = GetBossLocation();
	else
		vEndLocation = GetPlayerLocation();
	
	vTargetLocation = vEndLocation;
}

void UP_PB_MovementToTarget::InitializeMovement(FPatternData& _patternData)
{
	Super::InitializeMovement(_patternData);
	if (!owner) return;
	
	movementToTargetSettings = _patternData.GetMovementToTargetSettings();
	
	vStartLocation = owner->GetActorLocation();

	const ESpawnSettings& _target = movementToTargetSettings.target;
	
	if (_target == ESpawnSettings::ARENA_POINT)
		vEndLocation = GetArenaPointLocation(movementToTargetSettings.arenaPointName);
	else if (_target == ESpawnSettings::BOSS)
		vEndLocation = GetBossLocation();
	else
		vEndLocation = GetPlayerLocation();
	
	vTargetLocation = vEndLocation;
}

void UP_PB_MovementToTarget::MovementBehavior(const float& _deltaTime)
{
	if (!bIsEnable || !owner) return;
	
	const FVector& _location = FMath::VInterpConstantTo(owner->GetActorLocation(), vTargetLocation, _deltaTime, movementToTargetSettings.fMovementSpeed);

	owner->SetActorLocation(_location);

	if (IsAtLocation(_location))
	{
		iRepeatTime++;
		bMoveToEndLocation = !bMoveToEndLocation;
		vTargetLocation = bMoveToEndLocation ? vEndLocation : vStartLocation;
		if (iRepeatTime >= movementToTargetSettings.iRepeat)
		{
			bIsEnable = false;
			onBehaviorComplete.Broadcast();
		}
	}
}

const FVector UP_PB_MovementToTarget::GetPlayerLocation() const
{
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		if (const AIU_Player* _player = _gameMode->GetPlayer())
			return _player->GetActorLocation();
	
	return FVector(0);
}
const FVector UP_PB_MovementToTarget::GetBossLocation() const
{
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		if (const AB_Boss* _boss = _gameMode->GetBoss())
			return _boss->GetActorLocation();
	
	return FVector(0);
}
const FVector UP_PB_MovementToTarget::GetArenaPointLocation(const FString& _pointName) const
{
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		if (const AM_ArenaManager* _arenaManager = _gameMode->GetArenaManager())
			if (const AA_Arena* _arena = _arenaManager->GetArena())
				if (const AActor* _point = _arena->GetArenaPoint(_pointName))
					return _point->GetActorLocation();
	
	return FVector(0);
}