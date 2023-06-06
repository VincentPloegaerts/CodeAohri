#include "A_ArenaBlock.h"

#include "C_InteractionWidgetComponent.h"
#include "C_RespawnComponent.h"
#include "HUD_Game.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "IU_HealthSystemComponent.h"
#include "IU_CameraManager.h"
#include "LevelLoadingManager.h"
#include "M_BossManager.h"
#include "M_CinematicManager.h"
#include "M_GameMode.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "UI_PlayerWidget.h"
#include "UI_ZoneWarningWidget.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"

AA_ArenaBlock::AA_ArenaBlock()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	closedDoorStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Closed Door Static Mesh"));
	closedDoorStaticMesh->SetupAttachment(RootComponent);
	
	otherDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Other Door Mesh"));
	otherDoorMesh->SetupAttachment(RootComponent);
	
	cinematicButtonCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Cinematic Button Collider"));
	cinematicButtonCollider->SetupAttachment(RootComponent);

	niagaraSpawnLocation = CreateDefaultSubobject<USceneComponent>("Niagara Spawn Location");
	niagaraSpawnLocation->SetupAttachment(RootComponent);

	playerTeleportLocation = CreateDefaultSubobject<USceneComponent>("Player Teleport Location");
	playerTeleportLocation->SetupAttachment(RootComponent);

	otherDoorCloseLocation = CreateDefaultSubobject<USceneComponent>("Other Door Close Location");
	otherDoorCloseLocation->SetupAttachment(RootComponent);

#if WITH_EDITOR
	billboard = CreateDefaultSubobject<UBillboardComponent>("BillBoard");
	billboard->SetupAttachment(RootComponent);
#endif
}

void AA_ArenaBlock::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();

	world->GetTimerManager().SetTimer(initTimer, this, &AA_ArenaBlock::InitPlayer, 0.1f);
	
	Event_AddUnique(cinematicButtonCollider->OnComponentBeginOverlap, AA_ArenaBlock::StartPlayerCanInteract);
	Event_AddUnique(cinematicButtonCollider->OnComponentEndOverlap, AA_ArenaBlock::StopPlayerCanInteract);
}

void AA_ArenaBlock::InitPlayer()
{
	gameMode = world->GetAuthGameMode<AM_GameMode>();
	hud = world->GetFirstPlayerController()->GetHUD<AHUD_Game>();
	if (hud)
		playerWidget = hud->GetWidget<UUI_PlayerWidget>(WIDGET_PLAYER);
	
	if (gameMode)
	{
		player = gameMode->GetPlayer();
		ROG_ERROR(!player, "AA_ArenaBlock::InitPlayer -> Invalid player");
		playerRespawnComponent = player->RespawnComponent();
		cinematicManager = gameMode->GetCinematicManager();
		levelLoadingManager = gameMode->GetLevelLoadingManager();
	}
}

void AA_ArenaBlock::ActivateArenaBehavior()
{
	if (!bHasSetCameraBoss)
	{
		player->InitCameraByType(CAMERA_BOSS);
		bHasSetCameraBoss = true;
	}

	if (UIU_HealthSystemComponent* _healthComponent = player->HealthComponent())
	{
		_healthComponent->ResetHealth();
	}
	
	if (niagaraComp)
		SpawnNiagaraSystemOnSceneComponent(niagaraComp, niagaraSpawnLocation);

	if (fCloseDelay > 0.f)
		world->GetTimerManager().SetTimer(closeTimer, this, &AA_ArenaBlock::BroadcastArenaBlockTrigger, fCloseDelay);
	else
		BroadcastArenaBlockTrigger();
}

void AA_ArenaBlock::BroadcastArenaBlockTrigger()
{
	onBlockTrigger.Broadcast();
}

void AA_ArenaBlock::StartPlayerCanInteract(COMPONENT_START_COLLISION_PARAMS)
{
	if (player != _otherActor) return;
	if (playerWidget)
		playerWidget->ShowInteractionWidget();
	Event_AddUnique(player->OnInterract(), AA_ArenaBlock::Interacted);
}

void AA_ArenaBlock::StopPlayerCanInteract(COMPONENT_STOP_COLLISION_PARAMS)
{
	if (player != _otherActor) return;
	if (playerWidget)
		playerWidget->HideInteractionWidget();
	Event_Remove(player->OnInterract(), AA_ArenaBlock::Interacted);
}

void AA_ArenaBlock::Interacted()
{
	Event_Remove(player->OnInterract(), AA_ArenaBlock::Interacted);
	hud->ShowLoadScreen();
	if (levelLoadingManager)
	{
		levelLoadingManager->OnFinishedLoading().AddUniqueDynamic(this,&AA_ArenaBlock::LaunchCinematic);
		levelLoadingManager->AddLevels(levelsToLoad,levelsToUnload);
	}
	else
		LaunchCinematic();
}

void AA_ArenaBlock::LaunchCinematic()
{
	if (levelLoadingManager)
		levelLoadingManager->OnFinishedLoading().RemoveDynamic(this,&AA_ArenaBlock::LaunchCinematic);
	if (hud)
		hud->HideLoadScreen();
	if (cinematicManager && !bHasPlayCinematic)
	{
		if (!bossManager)
			bossManager = gameMode->GetBossManager();
		if (bossManager)
		{
			// disable button collision
			Event_AddUnique(cinematicManager->OnCinematicFinishedEvent(), AA_ArenaBlock::EndCinematicBehavior);
				
			cinematicManager->PlayBossIntroCinematic(bossManager->GetCurrentBossPhase());
			bHasPlayCinematic = true;
		}
	}
	else
	{
		Event_AddUnique(playerRespawnComponent->OnFadeInFinish(), AA_ArenaBlock::TeleportWithoutCinematic);
		playerRespawnComponent->SetIsFading();
	}
}

void AA_ArenaBlock::EndCinematicBehavior()
{
	Event_Remove(cinematicManager->OnCinematicFinishedEvent(), AA_ArenaBlock::EndCinematicBehavior);
	UUI_ZoneWarningWidget* _zoneWarning = Cast<UUI_ZoneWarningWidget>(hud->ShowGameWidget(WIDGET_WARNINGS_ZONE));
	//tp player
	const FVector& _pos = playerTeleportLocation->GetComponentLocation();
	const FRotator& _rot = playerTeleportLocation->GetComponentRotation();
	player->SetActorLocationAndRotation(_pos, _rot);
	ActivateArenaBehavior();
	if (const AIU_CameraManager* _cameraManager = gameMode->GetCameraManager())
	{
		if (AIU_Camera* _camera = _cameraManager->GetCamera())
		{	
			_camera->ResetCamera(_pos, _rot.Euler().Z);
			if (_zoneWarning)
				_zoneWarning->Init(player,_camera);
		}
	}	
	
	
	_zoneWarning->AddBossWarning(bossManager->GetCurrentBoss());
	
	//activate checkpoint
	if (AGPE_SaveTrigger* _saveTrigger = Cast<AGPE_SaveTrigger>(saveTrigger.Get()))
	{
		_saveTrigger->ActivateSave();
	}
	else
		LOG_ERROR("AA_ArenaBlock::EndCinematicBehavior -> Invalid Save point");

	//close instant all door
	otherDoorMesh->SetWorldLocation(otherDoorCloseLocation->GetComponentLocation());
}

void AA_ArenaBlock::TeleportWithoutCinematic()
{
	Event_Remove(playerRespawnComponent->OnFadeInFinish(), AA_ArenaBlock::TeleportWithoutCinematic);
	player->SetActorLocation(playerTeleportLocation->GetComponentLocation());
	playerRespawnComponent->SetIsFading();
	ActivateArenaBehavior();
}