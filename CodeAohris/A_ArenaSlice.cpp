#include "A_ArenaSlice.h"

#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"

AA_ArenaSlice::AA_ArenaSlice()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	staticMesh->SetupAttachment(RootComponent);
	
	staticMeshCollider = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshCollider");
	staticMeshCollider->SetupAttachment(staticMesh);
}

void AA_ArenaSlice::SetArenaOwner(AActor* _arena)
{
	arenaOwner = _arena;
}

void AA_ArenaSlice::BeginPlay()
{
	Super::BeginPlay();

	InitializeSlice();
	
	timerManager = &GetWorld()->GetTimerManager();
	OpenArenaSlice();
}
void AA_ArenaSlice::Tick(float DeltaSeconds)
{
#if WITH_EDITOR
	if (!bDebug) return;
	DebugArenaSlicePlayerPresence();
#endif
	Super::Tick(DeltaSeconds);

		
	if(bClose && bPlayerPresence && healthComponent)
		healthComponent->TakeDamage(sliceSettings.fDamage);
}

void AA_ArenaSlice::InitializeArenaSlice(const FSliceSettings& _settings)
{
	sliceSettings = _settings;
	onArenaSliceBeginClosure.Broadcast();

	timerManager->SetTimer(timerClose, this, &AA_ArenaSlice::CloseArenaSlice, sliceSettings.fTimeBeforeClose);
	
	//Decal or Effet for : before closure feedback

#if WITH_EDITOR
	if (!bDebug) return;
	DebugArenaSlice(FColor::Blue, sliceSettings.fTimeBeforeClose);
#endif
}

void AA_ArenaSlice::InitializeSlice()
{
	staticMeshCollider->SetStaticMesh(staticMesh->GetStaticMesh());
	staticMeshCollider->SetRelativeLocation(staticMesh->GetRelativeLocation() + FVector(0, 0, 50));
	
	if (staticMeshCollider)
	{
		staticMeshCollider->SetVisibility(false);
		staticMeshCollider->OnComponentBeginOverlap.AddDynamic(this, &AA_ArenaSlice::ArenaSliceOverlapBegin);
		staticMeshCollider->OnComponentEndOverlap.AddDynamic(this, &AA_ArenaSlice::ArenaSliceOverlapEnd);
	}
}

void AA_ArenaSlice::CloseArenaSlice()
{
	//if (!staticMeshCollider) return;
	//staticMeshCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	bClose = true;
	
	timerManager->SetTimer(timerClose, this, &AA_ArenaSlice::OpenArenaSlice, sliceSettings.fCloseTime);
	
	onArenaSliceClose.Broadcast();
	
	//Decal or Effet for : close slice feedback

#if WITH_EDITOR
	if (!bDebug) return;
	DebugArenaSlice(FColor::Red, sliceSettings.fCloseTime);
#endif
}
void AA_ArenaSlice::OpenArenaSlice()
{
	//if (!staticMeshCollider) return;
	//staticMeshCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	bClose = false;
	onArenaSliceEndClosure.Broadcast(this);
	
	//Decal of Effet for : Open slice feed
	
#if WITH_EDITOR
	if (!bDebug) return;
	DebugArenaSlice(FColor::Green, 0.5f);
#endif	
}

void AA_ArenaSlice::ArenaSliceOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AIU_Player* _player = Cast<AIU_Player>(OtherActor))
	{
		player = _player;
		healthComponent = _player->HealthComponent();
		bPlayerPresence = true;
	}
}
void AA_ArenaSlice::ArenaSliceOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (player == OtherActor)
	{
		bPlayerPresence = false;
	}
}

#if WITH_EDITOR
void AA_ArenaSlice::DebugArenaSlice(FColor _color, float _time)
{
	DrawDebugSolidBox(GetWorld(), GetActorLocation(),  staticMeshCollider->Bounds.BoxExtent, _color, false, _time, 0);
}

void AA_ArenaSlice::DebugArenaSlicePlayerPresence()
{
	DrawDebugSphere(GetWorld(), GetActorLocation() + FVector(0, 0, 80), 10, 10, bPlayerPresence ? FColor::Green : FColor::Black);
}
#endif