#include "C_InteractionWidgetComponent.h"

#include "IU_CameraManager.h"
#include "M_GameMode.h"

#include "IU_Player.h"

#include "Kismet/KismetMathLibrary.h"
#include "IU_Macros.h"

UC_InteractionWidgetComponent::UC_InteractionWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UC_InteractionWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner();

	FTimerHandle _timerInit;
	GetWorld()->GetTimerManager().SetTimer(_timerInit, this, &UC_InteractionWidgetComponent::InitializeInteractionWidgetComponent, FMath::RandRange(0.5f, 1.0f));
}

void UC_InteractionWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!player || !player->GetCamera() || !owner || !enabled) return;
	
	const FVector& _location = GetComponentLocation();
	const FVector& _targetLocation = player->GetCamera()->GetActorLocation();
	const FVector& _locationZ = FVector(_targetLocation.X, _targetLocation.Y,  _location.Z);
	
	const FRotator& _rotation = Get_LookRotation(_locationZ - _location);

	SetWorldRotation(_rotation);
}

void UC_InteractionWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DisableWidgetInteraction();
	
	Super::EndPlay(EndPlayReason);
}

void UC_InteractionWidgetComponent::InitializeInteractionWidgetComponent()
{
	const UWorld* _w = GetWorld();
	if (const AM_GameMode* _gm = _w->GetAuthGameMode<AM_GameMode>())
	{
		player = _gm->GetPlayer();
		if (player)
		{
			playerInteractionRange = player->GetInteractionRange();
			EnableWidgetInteraction();
		}
	}
}

void UC_InteractionWidgetComponent::CheckPlayerInteractionRange()
{
	if (!player || !enabled) return;
	
	const float _distance = FVector::Distance(player->GetActorLocation(), GetComponentLocation());

	if (_distance < playerInteractionRange + additionalRange)
	{
		if (bHiddenInGame)
			SetHiddenInGame(false);
	}
	else
	{
		if (!bHiddenInGame)
			SetHiddenInGame(true);
	}
}

void UC_InteractionWidgetComponent::EnableWidgetInteraction()
{
	enabled = true;
	//SetHiddenInGame(false);
	GetWorld()->GetTimerManager().SetTimer(checkTimer, this, &UC_InteractionWidgetComponent::CheckPlayerInteractionRange, checkRate, true);
}
void UC_InteractionWidgetComponent::DisableWidgetInteraction()
{
	enabled = false;
	SetHiddenInGame(true);
	GetWorld()->GetTimerManager().ClearTimer(checkTimer);
}

