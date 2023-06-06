#include "M_DifficultyManager.h"

#include "M_GameInstance.h"
#include "S_GameSave.h"

void UM_DifficultyManager::SetDifficulty(const EDifficultyType& _difficulty)
{
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		currentDifficulty = _difficulty;
		
		if (US_GameSave* _gameSave = _gi->GetCurrentGameSave())
			_gameSave->SetDifficulty(currentDifficulty);
		
		FDifficultyData _data;
		if (GetCurrentDifficultyData(_data))
			onDifficultyChanged.Broadcast(currentDifficulty, _data);
	}
}
