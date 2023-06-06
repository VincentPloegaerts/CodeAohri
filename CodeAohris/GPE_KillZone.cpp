#include "GPE_KillZone.h"
#include "IU_Player.h"
#include "C_RespawnComponent.h"
#include "Components/BoxComponent.h"
#include "IU_HealthSystemComponent.h"
#include "M_GameInstance.h"
#include "API_SteamAchievementsManager.h"
#include "M_GameMode.h"

AGPE_KillZone::AGPE_KillZone()
{
	PrimaryActorTick.bCanEverTick = false;

	triggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	AddOwnedComponent(triggerBox);
	RootComponent = triggerBox;
	
	debugBox = CreateDefaultSubobject<UStaticMeshComponent>("DebugBox");
	debugBox->SetupAttachment(RootComponent);
}

void AGPE_KillZone::BeginPlay()
{
	Super::BeginPlay();

	world = GetWorld();
	world->GetTimerManager().SetTimer(initTimer, this, &AGPE_KillZone::InitPlayerAndSteamManager, 0.6f);
	
	debugBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OnActorBeginOverlap.AddDynamic(this, &AGPE_KillZone::OnCollision);
}

void AGPE_KillZone::InitPlayerAndSteamManager()
{
	if (const AM_GameMode* _gameMode = world->GetAuthGameMode<AM_GameMode>())
	{
		player = _gameMode->GetPlayer();

	}

}

void AGPE_KillZone::OnCollision(AActor* _self, AActor* _other)
{
	if (player == _other)
	{
		if (UIU_HealthSystemComponent* _health = player->HealthComponent())
		{
			_health->TakeFallDamage();

			if (!_health->IsDead())
				player->RespawnComponent()->Death(false);
		}

		if (UM_GameInstance* _gameInstance = GetWorld()->GetGameInstance<UM_GameInstance>())
		{
			UAPI_SteamAchievementsManager* steamManager = _gameInstance->GetSteamIntegration()->GetAchievementsManager();

			if (steamManager)
			{
				steamManager->IncreaseCurrentFallDeath();
				
				steamManager->CheckForPlatformingAchievementsValidation(_gameInstance);
			}
		}
		
	}
}