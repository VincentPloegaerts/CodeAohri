

#include "C_PowerOrb.h"
#include "M_GameMode.h"
#include "IU_Player.h"
#include "UC_PlayerAttackComponent.h"

AC_PowerOrb::AC_PowerOrb()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Graphics"));
	orbCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollider"));

	staticMesh->SetupAttachment(RootComponent);
	orbCollider->SetupAttachment(RootComponent);
}

void AC_PowerOrb::BeginPlay()
{
	Super::BeginPlay();
	orbCollider->OnComponentBeginOverlap.AddDynamic(this,&AC_PowerOrb::OnOverlap );
	OnDestroyed.AddDynamic(this,&AC_PowerOrb::MyDestroyed);
}

void AC_PowerOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_PowerOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AC_PowerOrb::MyDestroyed(AActor* Act)
{
	AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
	if(!_gm)return;
	
	AIU_Player* _player = _gm->GetPlayer();
	if(!_player)return;

	//_player->GetCharacterAttackComponent()->FOnPowerOrbCollected().Broadcast();

}

void AC_PowerOrb::SetTwin(AC_PowerOrb* _twin)
{
	twin = _twin;
}

