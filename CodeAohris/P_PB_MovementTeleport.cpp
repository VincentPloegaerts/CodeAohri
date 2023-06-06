#include "P_PB_MovementTeleport.h"

#include "P_DT_PatternDataTable.h"
#include "M_GameMode.h"
#include "IU_Player.h"

void UP_PB_MovementTeleport::InitializeMovement(UP_DT_PatternDataTable* _dataTable)
{
	Super::InitializeMovement(_dataTable);
	movementTeleportSettings = _dataTable->GetMovementTeleportSettings();

	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		target = _gameMode->GetPlayer();

	Teleport();
}

void UP_PB_MovementTeleport::InitializeMovement(FPatternData& _patternData)
{
	Super::InitializeMovement(_patternData);
	movementTeleportSettings = _patternData.GetMovementTeleportSettings();

	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		target = _gameMode->GetPlayer();

	Teleport();
}

void UP_PB_MovementTeleport::MovementBehavior(const float& _deltaTime)
{
	if (!bIsEnable) return;
	
	fElapsedTime += _deltaTime;
	if (fElapsedTime > movementTeleportSettings.fRepeatRate)
	{
		fElapsedTime = 0;
		Teleport();
	}
}

void UP_PB_MovementTeleport::Teleport()
{
	if (!bIsEnable || !owner || !target) return;

	owner->SetActorLocation(target->GetActorLocation() + movementTeleportSettings.vOffset);
	
	iTeleportTime++;
	onTeleport.Broadcast();
	if (iTeleportTime >= movementTeleportSettings.iRepeat)
	{
		bIsEnable = false;
		onBehaviorComplete.Broadcast();
	}
}
