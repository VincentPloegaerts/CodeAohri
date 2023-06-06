#include "A_ArenaPoint.h"

AA_ArenaPoint::AA_ArenaPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	staticMesh->SetupAttachment(RootComponent);

#if WITH_EDITOR
	billboard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	billboard->SetupAttachment(RootComponent);
#endif
}

void AA_ArenaPoint::SetArenaOwner(AActor* _arena)
{
	arenaOwner = _arena;
}
