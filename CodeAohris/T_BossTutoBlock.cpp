#include "T_BossTutoBlock.h"

#include "IU_Player.h"
#include "M_GameMode.h"
#include "T_ProjectileDetectionSphere.h"
#include "Kismet/KismetMathLibrary.h"
#include "UC_PlayerAttackComponent.h"

AT_BossTutoBlock::AT_BossTutoBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	billBoard = CreateDefaultSubobject<UBillboardComponent>("BillBoard Tuto");
	billBoard->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	boxComponent = CreateDefaultSubobject<UBoxComponent>("TriggerBoxTuto");
	boxComponent->AttachToComponent(billBoard, FAttachmentTransformRules::KeepRelativeTransform);

	enterObstacleSpawnLocation = CreateDefaultSubobject<USceneComponent>("EnterObstacleSpawnLocation");
	enterObstacleSpawnLocation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	exitObstacleSpawnLocation = CreateDefaultSubobject<USceneComponent>("ExitObstacleSpawnLocation");
	exitObstacleSpawnLocation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	playerTeleportationLocation = CreateDefaultSubobject<USceneComponent>("playerTeleportationLocation");
	playerTeleportationLocation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	
}

void AT_BossTutoBlock::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle _handle;
	GetWorld()->GetTimerManager().SetTimer(_handle, this, &AT_BossTutoBlock::Init, 2.0f, false);
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AT_BossTutoBlock::OnActorOverlap);
}

void AT_BossTutoBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanOpen)
		OpenDoor();
}

void AT_BossTutoBlock::Init()
{
	if (!GetWorld())return;
	if (!GetWorld()->GetAuthGameMode<AM_GameMode>())return;

	tutoLogicRef = GetWorld()->GetAuthGameMode<AM_GameMode>()->GetTutoLogic();
	playerRef = tutoLogicRef->SendRefPlayerToTutoBlock();
	
	startPosition = exitObstacleSpawnLocation->GetComponentLocation();
}

void AT_BossTutoBlock::TeleportPlayer()
{
	if(playerRef)
		playerRef->SetActorLocation(playerTeleportationLocation->GetComponentLocation());
	
}

void AT_BossTutoBlock::OpenDoor()
{
	FVector _goTo = startPosition + FVector(0, 0, fMaxOpenHeight);
	FVector _newPos = UKismetMathLibrary::VLerp(exitObstacle->GetActorLocation(), _goTo, GetWorld()->DeltaTimeSeconds * fDoorOpenSpeed);

	exitObstacle->SetActorLocation(_newPos);

	if (FVector::Dist(exitObstacle->GetActorLocation(), _goTo) < 50)
		bCanOpen = false;
}

void AT_BossTutoBlock::OnActorOverlap(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp,
                                      int32 _index, bool _sweep, const FHitResult& _result)
{
	
	
	if (AIU_Player* _player = Cast<AIU_Player>(_otherActor))
	{
		if(toSpawn)
		{
			enterObstacle = GetWorld()->SpawnActor<AActor>(toSpawn,
			enterObstacleSpawnLocation->GetComponentLocation(),
			enterObstacleSpawnLocation->GetComponentRotation());

			exitObstacle = GetWorld()->SpawnActor<AActor>(toSpawn,
			exitObstacleSpawnLocation->GetComponentLocation(),
			exitObstacleSpawnLocation->GetComponentRotation());
		
			
		}
		
		AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
		if(!_gm)return;

		if(_gm->GetTutoLogic())
			_gm->GetTutoLogic()->StartTuto();
		_player->GetCharacterAttackComponent()->SetIsInTuto(true);
		TeleportPlayer();
		boxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AT_BossTutoBlock::OnActorOverlap);
	}
}

void AT_BossTutoBlock::DestroyObstacle()
{
	// if(enterObstacle)
	// 	enterObstacle->Destroy();
	if(exitObstacle)
		exitObstacle->Destroy();
}
