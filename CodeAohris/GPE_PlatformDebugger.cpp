#include "GPE_PlatformDebugger.h"

#include "GPE_Breakableplatform.h"
#include "GPE_Platform.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IU_Macros.h"
AGPE_PlatformDebugger::AGPE_PlatformDebugger()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void AGPE_PlatformDebugger::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &AGPE_PlatformDebugger::CheckPlatform, fCheckSpeed);
}

void AGPE_PlatformDebugger::CheckPlatform()
{
	ROG_ERROR(platformLayer.Num() < 1, "AGPE_PlatformDebugger::CheckPlatform -> Missing Layer");
	ROG_ERROR(!platformToSpawn.Get(), "AGPE_PlatformDebugger::CheckPlatform -> Missing Platform to spawn");
	
	FHitResult _hitResult;
	if (!UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(), GetActorLocation(), GetActorLocation() - FVector(0, 0, 5000),
		platformLayer, false, TArray<AActor*>(), EDrawDebugTrace::None, _hitResult, true))
	{
		if (bHasFail)
		{
			SpawnPlatform();
			return;
		}
		
		bHasFail = true;
#if WITH_LOG_DEBUG
		LOG_ERROR("AGPE_PlatformDebugger::CheckPlatform -> Missing Platform at %s", *GetName());
#endif
		FTimerHandle _timer;
		GetWorld()->GetTimerManager().SetTimer(_timer, this, &AGPE_PlatformDebugger::CheckPlatform, fCheckSpeed);
	}
	else
	{
		Destroy();
	}
}

void AGPE_PlatformDebugger::SpawnPlatform()
{
	AGPE_BreakablePlatform* _platform = GetWorld()->SpawnActor<AGPE_BreakablePlatform>(platformToSpawn, vTargetLocation, rTargetRotation);
	ROG_ERROR(!_platform, "AGPE_PlatformDebugger::SpawnPlatform -> invalid platform spawn");
	_platform->SetActorScale3D(vTargetScale);
	_platform->SetPlayerDetectionRange(fPlayerDetectionRange);
	Destroy();
}