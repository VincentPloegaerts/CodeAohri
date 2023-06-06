#include "C_Collectible.h"
#include "C_CollectibleComponent.h"
#include "M_CollectibleManager.h"
#include "M_GameMode.h"

AC_Collectible::AC_Collectible()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	collider->SetupAttachment(RootComponent);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mesh->SetupAttachment(RootComponent);
}

void AC_Collectible::BeginPlay()
{
	Super::BeginPlay();
	collider->OnComponentBeginOverlap.AddDynamic(this, &AC_Collectible::OnOverlap);

	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &AC_Collectible::RegsiterToManager, FMath::FRandRange(0.0f, 1.0f));
}

void AC_Collectible::RegsiterToManager()
{
	if(const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if(AM_CollectibleManager* _collectibleManager = _gameMode->GetCollectibleManager())
			_collectibleManager->RegisterToManager(this);
	}
}

void AC_Collectible::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UActorComponent* _actorComponent = OtherActor->GetComponentByClass(UC_CollectibleComponent::StaticClass());
	UC_CollectibleComponent* _collectibleComponent = Cast<UC_CollectibleComponent>(_actorComponent);
	
	if (!_collectibleComponent)
		return;

	_collectibleComponent->AddCollectible();
	Destroy();
}

