#include "GPE_Chest.h"

#include "IU_HealthSystemComponent.h"
#include "IU_PlayerMovementComponent.h"
#include "M_GameMode.h"
#include "TimerManager.h"
#include "UC_PlayerAttackComponent.h"
#include "UI_PlayerWidget.h"
#include "M_GameInstance.h"
#include "S_GameSave.h"

AGPE_Chest::AGPE_Chest()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	skeletalMesh->SetupAttachment(RootComponent);

	animTPLocation = CreateDefaultSubobject<USceneComponent>("AnimTPLocation");
	animTPLocation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	miniGame = CreateDefaultSubobject<UGPE_ChestMiniGame>(TEXT("ChestMinigame"));
	AddOwnedComponent(miniGame);
}

void AGPE_Chest::StartChestMiniGame()
{
	if (!active || miniGameStarted || isAlreadyOpen || !miniGame || !player || !player->MovementComponent()) return;

	player->SetActorLocation(animTPLocation->GetComponentLocation());
	player->SetActorRotation(animTPLocation->GetComponentRotation());
	player->MovementComponent()->SetCanMove(false);

	if (playerWidget)
	{
		isWidgetHidden = true;	
		playerWidget->HideInteractionWidget();
	}

	miniGame->SetChestOwner(this);
	miniGame->StartMiniGame();
	miniGameStarted = true;
}

void AGPE_Chest::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &AGPE_Chest::InitializeChest, 0.5f, false);
}

void AGPE_Chest::InitializeChest()
{
	if (const AM_GameMode* _gm = GetWorld()->GetAuthGameMode<AM_GameMode>())
		player = _gm->GetPlayer();

	if (player)
		playerInteractionRange = player->GetInteractionRange();

	if (skeletalMesh)
	{
		chestAnimInstance = Cast<UGPE_ChestAnimInstance>(skeletalMesh->GetAnimInstance());
	}

	miniGame->OnGameEnded().AddUniqueDynamic(this, &AGPE_Chest::MiniGameFinished);

	if (UM_GameInstance* _gameInstance = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		if (!_gameInstance->GetCurrentGameSave())return;

		if (_gameInstance->GetCurrentGameSave()->GetGPEChestData().Contains(this->GetName()))
			_gameInstance->GetCurrentGameSave()->GetGPEChestData().Emplace(this->GetName(), false);
		else
			_gameInstance->GetCurrentGameSave()->GetGPEChestData().Add(this->GetName(), false);

	}

	if (AHUD_Main* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>())
		playerWidget = _hud->GetWidget<UUI_PlayerWidget>(WIDGET_PLAYER);

	GetWorld()->GetTimerManager().SetTimer(timerDistance, this, &AGPE_Chest::CheckPlayerInteractionRange, 0.2f, true);
}

void AGPE_Chest::MiniGameFinished(bool _success)
{
	if (_success)
	{

		player->GetPlayerAnimInstance()->UpdateIsOpeningChest(false);
		FTimerHandle _timer;
		GetWorldTimerManager().SetTimer(_timer,this,&AGPE_Chest::MiniGameSucceed,2.5f,false);
		
	}
	else
	{
		FTimerHandle _timer;
		GetWorldTimerManager().SetTimer(_timer, this, &AGPE_Chest::MiniGameFailed, 1.f, false);
		
	}
}

void AGPE_Chest::MiniGameSucceed()
{
	GetWorld()->GetTimerManager().ClearTimer(timerDistance);
	chestAnimInstance->UpdateIsOpen(true);
	isAlreadyOpen = true;

	active = false;
	miniGameStarted = false;
	player->MovementComponent()->SetCanMove(true);
	player->SetPlayerInputs(true);
	player->StopInteraction();

	UWorld* _world = GEngine->GameViewport->GetWorld();

	if (UM_GameInstance* _gameInstance = _world->GetGameInstance<UM_GameInstance>())
	{
		
		US_GameSave* gameSave = _gameInstance->GetCurrentGameSave();

		if (gameSave)
		{
			if (gameSave->GetGPEChestData().Contains(this->GetName()))
				gameSave->GetGPEChestData().Emplace(this->GetName(), true);

			gameSave->CheckForOpenChestSuccess();
		}

	}
}

void AGPE_Chest::MiniGameFailed()
{
	if (playerWidget)
		playerWidget->ShowInteractionWidget();
	
	miniGameStarted = false;
	canInteract = true;
	player->MovementComponent()->SetCanMove(true);
	player->SetPlayerInputs(true);
	player->StopInteraction();
}

void AGPE_Chest::CheckPlayerInteractionRange()
{
	if (!player || !playerWidget || miniGameStarted) return;
	
	const float _distance = FVector::Distance(player->GetActorLocation(), animTPLocation->GetComponentLocation());

	if (_distance < playerInteractionRange/* + additionalRange*/)
	{
		if (isWidgetHidden)
		{
			isWidgetHidden = false;
			playerWidget->ShowInteractionWidget();
		}
	}
	else if (!isWidgetHidden)
	{
		isWidgetHidden = true;
		playerWidget->HideInteractionWidget();
	}
}