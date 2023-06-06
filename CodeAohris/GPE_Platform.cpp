#include "GPE_Platform.h"

#include "IU_Macros.h"
#include "IU_Player.h"
#include "M_GameMode.h"

AGPE_Platform::AGPE_Platform()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	staticMesh->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
}

void AGPE_Platform::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &AGPE_Platform::GetPlayerGamemode, Get_RandFloat(1.0f, 5.0f));
}

void AGPE_Platform::GetPlayerGamemode()
{
	if (const UWorld* _world = GetWorld())
	{
		if (const AM_GameMode* _gameMode = _world->GetAuthGameMode<AM_GameMode>())
		{
			player = _gameMode->GetPlayer();
			if (!player)
			{
				LOG("AGPE_Platform::GetPlayerGamemode -> Invalid player");
				return;
			}
			FTimerHandle _timer;
			GetWorld()->GetTimerManager().SetTimer(_timer, this, &AGPE_Platform::CheckPlayerDistance, 2.0f, true);
		}
	}
}

void AGPE_Platform::CheckPlayerDistance()
{
	if (FVector::Dist(player->GetActorLocation(), GetActorLocation()) < playerDetectionDistance)
	{
		RootComponent->SetVisibility(true, true);
	}
	else
	{
		RootComponent->SetVisibility(false, true);
	}
}