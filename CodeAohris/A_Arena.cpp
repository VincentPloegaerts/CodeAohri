#include "A_Arena.h"

#include "IU_Macros.h"
#include "M_GameMode.h"
#include "M_ArenaManager.h"
#include "P_PoolingTrigger.h"
#include "A_DT_ArenaDataTable.h"
#include "A_ArenaBlock.h"
#include "IU_Camera.h"
#include "IU_Player.h"
#include "M_BossManager.h"

AA_Arena::AA_Arena()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	if (!IsValid(RootComponent)) return;

	arenaMesh = CreateDefaultSubobject<UStaticMeshComponent>("Arena Mesh");
	arenaMesh->SetupAttachment(RootComponent);

#if WITH_EDITOR
	
	billboard = CreateDefaultSubobject<UBillboardComponent>("BillBoard");
	billboard->SetupAttachment(RootComponent);

#endif
}

void AA_Arena::BeginPlay()
{
	Super::BeginPlay();
	InitializeArena();
}

void AA_Arena::InitializeArena()
{
	InitializeArenaBlock();
	FTimerHandle _initTimer;
	GetWorld()->GetTimerManager().SetTimer(_initTimer, this, &AA_Arena::RegisterToArenaManager, 0.1f);
	InitializePoolingTrigger();
	
}

void AA_Arena::InitializeArenaBlock()
{
	if (!arenaBlock)
	{
		LOG("AA_Arena::InitializeArena -> ArenaBlock is nullptr, Arena Init abord.");
		return;
	}
	
	//onArenaOpen.AddDynamic(arenaBlock, &AA_ArenaBlock::OpenArenaBlock);
	//onArenaClose.AddDynamic(arenaBlock, &AA_ArenaBlock::CloseArenaBlock);
}

void AA_Arena::RegisterToArenaManager()
{
	if (const UWorld* _world = GetWorld())
	{
		if (const AM_GameMode* _gameMode = _world->GetAuthGameMode<AM_GameMode>())
		{
			if (AM_BossManager* _bossManager = _gameMode->GetBossManager())
			{
				_bossManager->SetArena(this, bossPhase);
			}
		}
		else
			LOG("AA_Arena::RegisterToArenaManager -> GameMode not valid")
	}
	else
		LOG("AA_Arena::RegisterToArenaManager -> World not valid")
	
}

void AA_Arena::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(canPlayDeathAnimation)
	{
		deathAnimationTimer += GetWorld()->DeltaTimeSeconds;
		if(deathAnimationTimer > 4)
		{
			CloseArena();
			deathAnimationTimer = 0;
			canPlayDeathAnimation = false;
		}
			
		
	}
		
}

void AA_Arena::BeginOpenArena()
{
	if(canPlayDeathAnimation)return;
	
	canPlayDeathAnimation = true;
}

void AA_Arena::SetPlayerAnimInstanceIsInArena(bool _status)
{
	if(AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode()))
		
		if(AIU_Player* _player = Cast<AIU_Player>(_gm->GetPlayer()))

			_player->GetPlayerAnimInstance()->UpdateIsInArena(_status);
			
}


void AA_Arena::InitializePoolingTrigger()
{
	if (!poolingTrigger)
	{
		LOG("AA_Arena::InitializePoolingTrigger => No Pooling Trigger is set.");
		return;
	}
	
	onArenaClose.AddDynamic(poolingTrigger, &AP_PoolingTrigger::EnablePoolingTrigger);
	onArenaOpen.AddDynamic(poolingTrigger, &AP_PoolingTrigger::BeginDisablePoolingTrigger);
}

void AA_Arena::OpenArena()
{
	onArenaOpen.Broadcast();
	SetPlayerAnimInstanceIsInArena(false);
	
	
}

void AA_Arena::CloseArena()
{
	
	onArenaClose.Broadcast();
	SetPlayerAnimInstanceIsInArena(true);
	
}

#if WITH_EDITOR

void AA_Arena::PreEditChange(FProperty* PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);

	if (PropertyAboutToChange->GetFName() == FName("arenaPoints"))
		oldArenaPoints = arenaPoints;
	if (PropertyAboutToChange->GetFName() == FName("arenaSlices"))
		oldArenaSlices = arenaSlices;
	if (PropertyAboutToChange->GetFName() == FName("movementPoints"))
		oldArenaMovementPoints = movementPoints;
}

void AA_Arena::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(PropertyChangedEvent.Property->GetFName() == FName("arenaPoints"))
	{
		if (arenaPoints.Num() < oldArenaPoints.Num())
		{
			TArray<FString> _oldKeys, _keys;
			oldArenaPoints.GetKeys(_oldKeys);
			arenaPoints.GetKeys(_keys);

			const int& _max = _oldKeys.Num();
			for (int i = 0; i < _max; ++i)
				if (!_keys.Contains(_oldKeys[i]))
				{
					AActor* _removedPoint = oldArenaPoints.FindChecked(_oldKeys[i]);
					if (!_removedPoint) continue;
					_removedPoint->Destroy();
				}
		}
	}
	
	if(PropertyChangedEvent.Property->GetFName() == FName("movementPoints"))
	{
		if (movementPoints.Num() < oldArenaMovementPoints.Num())
		{
			TArray<FString> _oldKeys, _keys;
			oldArenaMovementPoints.GetKeys(_oldKeys);
			movementPoints.GetKeys(_keys);

			const int& _max = _oldKeys.Num();
			for (int i = 0; i < _max; ++i)
				if (!_keys.Contains(_oldKeys[i]))
				{
					AActor* _removedPoint = oldArenaMovementPoints.FindChecked(_oldKeys[i]);
					if (!_removedPoint) continue;
					_removedPoint->Destroy();
				}
		}
	}

	if(PropertyChangedEvent.Property->GetFName() == FName("arenaSlices"))
	{
		if (arenaSlices.Num() < oldArenaSlices.Num())
		{
			TArray<FString> _oldKeys, _keys;
			oldArenaSlices.GetKeys(_oldKeys);
			arenaSlices.GetKeys(_keys);

			const int& _max = _oldKeys.Num();
			for (int i = 0; i < _max; ++i)
				if (!_keys.Contains(_oldKeys[i]))
				{
					AActor* _removedSlice = oldArenaSlices.FindChecked(_oldKeys[i]);
					if (!_removedSlice) continue;
					_removedSlice->Destroy();
				}
		}
	}
	RefreshArenaData();
}

void AA_Arena::SpawnArenaPoint()
{
	if (!arenaPointType.GetDefaultObject())
	{
		LOG_ERROR("Spawn Arena Point : Point Type Invalid.");
		return;
	}

	if (arenaPoints.Contains(pointName))
	{
		LOG_ERROR("Spawn Arena Point : Point Name not valid => Already taken.");
		return;
	}
	if (pointName.IsEmpty())
	{
		LOG_ERROR("Spawn Arena Point : Point Name not valid => Empty name.");
		return;
	}
	
	AA_ArenaPoint* _spawnedPoint = SpawnAndAttach(arenaPointType, pointName);
	_spawnedPoint->SetArenaOwner(this);
	arenaPoints.Add(pointName, _spawnedPoint);

	RefreshArenaData();
}

void AA_Arena::SpawnArenaSlice()
{
	if (!arenaSliceType.GetDefaultObject())
	{
		LOG_ERROR("Spawn Arena Slice : Slice Type Invalid.");
		return;
	}

	if (arenaSlices.Contains(sliceName) || movementPoints.Contains(pointName))
	{
		LOG_ERROR("Spawn Arena Point : Point Name not valid => Already taken in Arena Points or Movement Points.");
		return;
	}
	if (sliceName.IsEmpty())
	{
		LOG_ERROR("Spawn Arena Slice : Slice Name not valid => Empty name.");
		return;
	}
	
	AA_ArenaSlice* _spawnedSlice = SpawnAndAttach(arenaSliceType, sliceName);
	_spawnedSlice->SetArenaOwner(this);
	arenaSlices.Add(sliceName, _spawnedSlice);

	RefreshArenaData();
}

void AA_Arena::SpawnArenaBlock()
{
	if (!arenaBlockType.GetDefaultObject())
	{
		LOG_ERROR("Spawn Arena Block : Block Type Invalid.");
		return;
	}

	if (arenaBlock)
	{
		LOG_ERROR("Spawn Arena Block : Block already exist.");
		return;
	}
	if (blockName.IsEmpty())
	{
		LOG_ERROR("Spawn Arena Slice : Block Name not valid => Empty name.");
		return;
	}

	arenaBlock = SpawnAndAttach(arenaBlockType, blockName);
}

void AA_Arena::SpawnArenaMovementPoint()
{
	if (!arenaPointType.GetDefaultObject())
	{
		LOG_ERROR("Spawn Arena Point : Point Type Invalid.");
		return;
	}

	if (arenaPoints.Contains(pointName) || movementPoints.Contains(pointName))
	{
		LOG_ERROR("Spawn Arena Point : Point Name not valid => Already taken in Arena Points or Movement Points.");
		return;
	}
	if (pointName.IsEmpty())
	{
		LOG_ERROR("Spawn Arena Point : Point Name not valid => Empty name.");
		return;
	}
	
	AA_ArenaPoint* _spawnedPoint = SpawnAndAttach(arenaPointType, pointName);
	_spawnedPoint->SetArenaOwner(this);
	movementPoints.Add(pointName, _spawnedPoint);

	RefreshArenaData();
}

void AA_Arena::RefreshArenaData() const
{
	if (!arenaData)
	{
		LOG_ERROR("Refresh Arena Data : No Arena Data Table");
		return;
	}

	TArray<FString> _pointNames;
	arenaPoints.GetKeys(_pointNames);
	arenaData->SetPointNames(_pointNames);
	
	TArray<FString> _sliceNames;
	arenaSlices.GetKeys(_sliceNames);
	arenaData->SetSliceNames(_sliceNames);
	
	TArray<FString> _movementPointNames;
	movementPoints.GetKeys(_movementPointNames);
	arenaData->SetMovementPointNames(_movementPointNames);	
}

#endif