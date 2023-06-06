#include "M_GPEManager.h"

#include "M_GameInstance.h"
#include "S_GameSave.h"

#include "GPE_BaseGPE.h"

#if WITH_EDITOR
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#endif

#include "C_RespawnComponent.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "M_GameMode.h"

AM_GPEManager::AM_GPEManager()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");

#if WITH_EDITOR
	billboard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	billboard->SetupAttachment(RootComponent);
#endif
}
void AM_GPEManager::BeginPlay()
{
	Super::BeginPlay();
	InitializeGPEFromGameSave();

	FTimerHandle _timerInit;
	GetWorld()->GetTimerManager().SetTimer(_timerInit, this, &AM_GPEManager::InitializeManager, 0.5f);
}

void AM_GPEManager::InitializeManager()
{
	if (const AM_GameMode* _gm = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if (AM_GameLogic* _gl = _gm->GetGameLogic())
		{
			_gl->OnCheckpoint().AddUniqueDynamic(this, &AM_GPEManager::OnCheckpoint);
			
			if (const AIU_Player* _player = _gl->GetPlayer())
				_player->RespawnComponent()->OnRespawn().AddUniqueDynamic(this, &AM_GPEManager::BroadcastResetGPE);
		}
	}
}

void AM_GPEManager::InitializeGPEFromGameSave()
{
	if (const UM_GameInstance* _gm = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		if (US_GameSave* _gameSave = _gm->GetCurrentGameSave())
		{
			const TMap<int32, FString>& _gpeData = _gameSave->GetGPEData();
		
			for (auto _currentGPE : gpe)
			{
				const int32& _index = _currentGPE.Key;

				if (AGPE_BaseGPE* _gpe = _currentGPE.Value)
				{
					_gpe->SetGameSave(_gameSave);

					if (_gpeData.Contains(_index))
						_gpe->InitializeGPE(_gpeData[_index]);
					else
						_gpe->SelfSaveGPE();
				
					onSaveGPE.AddDynamic(_gpe, &AGPE_BaseGPE::SaveGPE);
					onCheckPointGPE.AddDynamic(_gpe, &AGPE_BaseGPE::SelfSaveGPE);
					onResetGPE.AddDynamic(_gpe, &AGPE_BaseGPE::ResetGPE);
				}
			}
		}
		#if WITH_EDITOR	//Allow to test Self Save / Reset without a Current Save (only in Editor)
		else
		{
			for (const auto _currentGPE : gpe)
			{
				if (AGPE_BaseGPE* _gpe = _currentGPE.Value)
				{
					_gpe->SelfSaveGPE();
				
					onSaveGPE.AddDynamic(_gpe, &AGPE_BaseGPE::SaveGPE);
					onCheckPointGPE.AddDynamic(_gpe, &AGPE_BaseGPE::SelfSaveGPE);
					onResetGPE.AddDynamic(_gpe, &AGPE_BaseGPE::ResetGPE);
				}
			}
		}
		#endif
	}
}

void AM_GPEManager::OnCheckpoint(const bool _saveProgression)
{
	if (_saveProgression)
		BroadcastSaveGPE();
	else
		BroadcastCheckPointGPE();
}

void AM_GPEManager::BroadcastSaveGPE()
{
	onSaveGPE.Broadcast();
}
void AM_GPEManager::BroadcastCheckPointGPE()
{
	onCheckPointGPE.Broadcast();
}
void AM_GPEManager::BroadcastResetGPE()
{
	onResetGPE.Broadcast();
}

#if WITH_EDITOR
void AM_GPEManager::InitializeGPE()
{
	gpe.Empty();

	TArray<AActor*> _gpe = { };
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGPE_BaseGPE::StaticClass(), _gpe);

	const int _max = _gpe.Num();
	for (int i = 0; i < _max; i++)
	{
		AGPE_BaseGPE* _current = Cast<AGPE_BaseGPE>(_gpe[i]);
		if (!_current || IsToIgnore(_current)) continue;
		gpe.Add(i, _current);
		_current->SetIndex(i);
	}
	
	LOG("GPE Manager => GPE Initialization done (%d GPE)", gpe.Num());
}

bool AM_GPEManager::IsToIgnore(const AGPE_BaseGPE* _gpe)
{
	const int _max = gpeToIgnore.Num();
	for (int i = 0; i < _max; i++)
		if (_gpe->GetClass()->IsChildOf(gpeToIgnore[i]))
			return true;
	
	return false;
}

void AM_GPEManager::SimulateSaveGPE()
{
	BroadcastSaveGPE();
}
void AM_GPEManager::SimulateCheckPointGPE()
{
	BroadcastCheckPointGPE();
}
void AM_GPEManager::SimulateResetGPE()
{
	BroadcastResetGPE();
}
#endif
