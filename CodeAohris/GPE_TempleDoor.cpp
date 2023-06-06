#include "GPE_TempleDoor.h"

#include "LevelLoadingManager.h"
#include "M_GameMode.h"

AGPE_TempleDoor::AGPE_TempleDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	triggerBoxDoor = CreateDefaultSubobject<UBoxComponent>("TriggerBoxDoor");
	triggerBoxDoor->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
}

void AGPE_TempleDoor::BeginPlay()
{
	Super::BeginPlay();
	gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>();
}

void AGPE_TempleDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGPE_TempleDoor::LoadAndUnloadLevels()
{
	if (gameMode)
	{
		gameMode->GetLevelLoadingManager()->AddLevels(levelsToLoad,levelsToUnload);
	}
}