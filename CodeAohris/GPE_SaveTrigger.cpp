#include "GPE_SaveTrigger.h"

#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "M_GameInstance.h"
#include "M_GameMode.h"
#include "M_LevelStreamerDeluxeEdition.h"
#include "S_GameSave.h"
#include "Kismet/GameplayStatics.h"

AGPE_SaveTrigger::AGPE_SaveTrigger()
{
	triggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	AddOwnedComponent(triggerBox);
	
	RootComponent = triggerBox;
	
	billboardComponent = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	billboardComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	respawnPoint = CreateDefaultSubobject<USceneComponent>("RespawnPoint");
	respawnPoint->AttachToComponent(billboardComponent, FAttachmentTransformRules::KeepRelativeTransform);

	arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	arrow->AttachToComponent(respawnPoint, FAttachmentTransformRules::KeepRelativeTransform);
}

void AGPE_SaveTrigger::RollbackLevelStreamer()
{
	const int& _levelStreamerCount = levelStreamers.Num();

	for(int i = 0; i < _levelStreamerCount; i++)
	{
		if (AM_LevelStreamer* _levelStreamer = levelStreamers[i].Get())
			_levelStreamer->Rollback();
	}
}

void AGPE_SaveTrigger::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AGPE_SaveTrigger::OnCollision);

	const UWorld* _world = GetWorld();
	gameMode = _world->GetAuthGameMode<AM_GameMode>();
	if (gameMode)
		gameLogic = gameMode->GetGameLogic();

	timerManager = &_world->GetTimerManager();
	FTimerHandle _timer;
	gameSave = GetGameInstance<UM_GameInstance>()->GetCurrentGameSave();
	timerManager->SetTimer(_timer, this, &AGPE_SaveTrigger::GetPlayerGameMode, Get_RandFloat(1.0f, 2.0f));
}

void AGPE_SaveTrigger::OnCollision(AActor* _self, AActor* _other)
{
	if (player == _other)
	{
		ActivateSave();
		if (bIsASaveTriggerForDeath)
			player->HealthComponent()->ResetHealth();
	}
}

void AGPE_SaveTrigger::GetPlayerGameMode()
{
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		player = _gameMode->GetPlayer();
		if (!player)
		{
			LOG("AGPE_SaveTrigger::GetPlayerGamemode -> Invalid player");
			return;
		}
		FTimerHandle _timer;
		timerManager->SetTimer(_timer, this, &AGPE_SaveTrigger::CheckPlayerDistance, 2.0f, true);
	}
}

void AGPE_SaveTrigger::CheckPlayerDistance() const
{
	const bool& _isInRange = FVector::Dist(player->GetActorLocation(), GetActorLocation()) < fPlayerDetectionDistance;
	RootComponent->SetVisibility(_isInRange, true);
}

void AGPE_SaveTrigger::ActivateSave()
{
	onSaved.Broadcast();
	if (gameMode && gameLogic)
	{
//#if WITH_EDITOR
		if (gameSave)
		{
			gameSave->SetPlayerStartingTransform(FTransform(arrow->GetComponentRotation(),respawnPoint->GetComponentLocation(),FVector::OneVector));
			gameSave->SetSaveTrigger(this);
			if (bIsASaveTriggerForDeath)
				gameSave->SetSaveTriggerDeath(this);
			gameSave->SetProgressDone(true);
		}
//#endif
		gameLogic->Checkpoint(bSaveProgression);
		gameMode->SetRespawnPoint(bIsASaveTriggerForDeath, respawnPoint->GetComponentLocation(), arrow->GetComponentRotation(), this);
	}
}
