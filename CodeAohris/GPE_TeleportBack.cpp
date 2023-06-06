#include "GPE_TeleportBack.h"

#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "M_GameMode.h"
#include "Components/BoxComponent.h"

AGPE_TeleportBack::AGPE_TeleportBack()
{
	PrimaryActorTick.bCanEverTick = false;

	triggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	AddOwnedComponent(triggerBox);
	RootComponent = triggerBox;

	teleportPlayerLocation = CreateDefaultSubobject<USceneComponent>("Teleport Location");
	teleportPlayerLocation->SetupAttachment(RootComponent);
	
	debugBox = CreateDefaultSubobject<UStaticMeshComponent>("DebugBox");
	debugBox->SetupAttachment(RootComponent);
}

void AGPE_TeleportBack::BeginPlay()
{
	Super::BeginPlay();

	world = GetWorld();
	world->GetTimerManager().SetTimer(initTimer, this, &AGPE_TeleportBack::InitPlayer, 0.6f);
	
	debugBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OnActorBeginOverlap.AddDynamic(this, &AGPE_TeleportBack::OnCollision);
}

void AGPE_TeleportBack::InitPlayer()
{
	if (const AM_GameMode* _gameMode = world->GetAuthGameMode<AM_GameMode>())
	{
		player = _gameMode->GetPlayer();
	}
}

void AGPE_TeleportBack::OnCollision(AActor* _self, AActor* _other)
{
	if (player == _other)
	{
		player->HealthComponent()->TakeFallDamage();
		player->SetActorLocation(teleportPlayerLocation->GetComponentLocation());
	}
}