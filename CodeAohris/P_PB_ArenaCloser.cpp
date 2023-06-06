#include "P_PB_ArenaCloser.h"

#include "IU_Macros.h"
#include "M_GameMode.h"
#include "M_ArenaManager.h"
#include "A_Arena.h"
#include "A_ArenaSlice.h"
#include "P_DT_ArenaCloserDataTable.h"
#include "P_DT_ZoneDataTable.h"

void UP_PB_ArenaCloser::ActivateBehavior(UP_DT_BaseDataTable* _dataTable)
{
	Super::ActivateBehavior(_dataTable);
	const UP_DT_ArenaCloserDataTable* _arenaCloserDatatable = Cast<UP_DT_ArenaCloserDataTable>(_dataTable);
	if (!_arenaCloserDatatable) return;
	
    arenaCloserSettings = _arenaCloserDatatable->GetArenaCloserSettings();

	if (const AM_GameMode* _gm = GetWorld()->GetAuthGameMode<AM_GameMode>())
		if (const AM_ArenaManager* _arenaManager = _gm->GetArenaManager())
			if (const AA_Arena* _arena = _arenaManager->GetArena())
			{
				slices = _arena->GetArenaSlices(arenaCloserSettings.arenaSlices);
				if (arenaCloserSettings.bClosePlayerSlice)
				{
					if (AA_ArenaSlice* _slice = _arena->GetArenaSliceWithPlayerPresence())
						if (!slices.Contains(_slice))
							slices.Add(_slice);
				}
			}
			else
			{
				LOG("UP_PB_ArenaCloser::ActivateBehavior : Arena not found in Arena Manager");	
				return;
			}
		else
		{
			LOG("UP_PB_ArenaCloser::ActivateBehavior : Arena Manager not found in GameMode");	
			return;
		}
	else
	{
		LOG("UP_PB_ArenaCloser::ActivateBehavior : GameMode not found");
		return;		
	}

	StartArenaSliceClosureProcess();
}

void UP_PB_ArenaCloser::ActivateBehavior(FPatternData& _patternData)
{
	Super::ActivateBehavior(_patternData);
	
	arenaCloserSettings = _patternData.GetArenaCloserSettings();

	if (const AM_GameMode* _gm = GetWorld()->GetAuthGameMode<AM_GameMode>())
		if (const AM_ArenaManager* _arenaManager = _gm->GetArenaManager())
			if (const AA_Arena* _arena = _arenaManager->GetArena())
			{
				slices = _arena->GetArenaSlices(arenaCloserSettings.arenaSlices);
				if (arenaCloserSettings.bClosePlayerSlice)
				{
					if (AA_ArenaSlice* _slice = _arena->GetArenaSliceWithPlayerPresence())
						if (!slices.Contains(_slice))
							slices.Add(_slice);
				}
			}
			else
			{
				LOG("UP_PB_ArenaCloser::ActivateBehavior : Arena not found in Arena Manager");	
				return;
			}
		else
		{
			LOG("UP_PB_ArenaCloser::ActivateBehavior : Arena Manager not found in GameMode");	
			return;
		}
	else
	{
		LOG("UP_PB_ArenaCloser::ActivateBehavior : GameMode not found");
		return;		
	}

	StartArenaSliceClosureProcess();
}


void UP_PB_ArenaCloser::StartArenaSliceClosureProcess()
{
	iSliceToClose = slices.Num();
	const FSliceSettings& _sliceSettings = arenaCloserSettings.sliceSettings;
	
	for (int i = 0; i < iSliceToClose; i++)
	{
		AA_ArenaSlice* _slice = slices[i];
		if (!_slice) continue;
		_slice->OnEndClosure().AddDynamic(this, &UP_PB_ArenaCloser::AreaSliceClosureProcessEnd);
		_slice->InitializeArenaSlice(_sliceSettings);
	}
}

void UP_PB_ArenaCloser::AreaSliceClosureProcessEnd(AA_ArenaSlice* _slice)
{
	_slice->OnEndClosure().RemoveDynamic(this, &UP_PB_ArenaCloser::AreaSliceClosureProcessEnd);
	iSliceOpen++;

	if (iSliceOpen >= iSliceToClose)
		onBehaviorComplete.Broadcast();
}
