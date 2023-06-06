#include "M_LevelThreeTriggerCinematic.h"
#include "IU_Player.h"
#include "M_CinematicManager.h"
#include "M_GameMode.h"
#include "IU_Macros.h"


AM_LevelThreeTriggerCinematic::AM_LevelThreeTriggerCinematic()
{
 
	PrimaryActorTick.bCanEverTick = true;

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxCollider->SetupAttachment(RootComponent);

}


void AM_LevelThreeTriggerCinematic::BeginPlay()
{
	Super::BeginPlay();

	boxCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AM_LevelThreeTriggerCinematic::OnActorOverlap);
	
}


void AM_LevelThreeTriggerCinematic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (canBeClosed && !hasAlreadyClosed)
	{
		hasAlreadyClosed = true;
		FTimerHandle _timer;
		GetWorld()->GetTimerManager().SetTimer(_timer, this, &AM_LevelThreeTriggerCinematic::RotateDoor, 3.f, false);
		//RotateDoor();
	}

}


void AM_LevelThreeTriggerCinematic::OnActorOverlap(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _index, bool _sweep, const FHitResult& _result)
{
	if (Cast<AIU_Player>(_otherActor))
	{
		AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
		if (!_gm)return;

		_gm->GetCinematicManager()->PlayLevelThreeIntroCinematic();

		//RotateDoor();

		canBeClosed = true;

		boxCollider->OnComponentBeginOverlap.RemoveDynamic(this, &AM_LevelThreeTriggerCinematic::OnActorOverlap);
		//Destroy();
	}
}

void AM_LevelThreeTriggerCinematic::RotateDoor()
{
	
	if (!toCloseRight || !toCloseLeft) return;
	
	boxCollider->OnComponentBeginOverlap.RemoveDynamic(this, &AM_LevelThreeTriggerCinematic::OnActorOverlap);
	//LOG("%f  /  %f  /  %f", toCloseRight->GetActorRotation().Roll, toCloseRight->GetActorRotation().Pitch, toCloseRight->GetActorRotation().Yaw);
	toCloseRight->SetActorRotation(toCloseRight->GetActorRotation() + FRotator(0, rotationValuesRight, 0));
	toCloseLeft->SetActorRotation(toCloseLeft->GetActorRotation() + FRotator(0, rotationValuesLeft, 0));
}
