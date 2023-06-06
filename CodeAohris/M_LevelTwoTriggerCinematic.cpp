// Fill out your copyright notice in the Description page of Project Settings.


#include "M_LevelTwoTriggerCinematic.h"

#include "API_SteamAchievementsManager.h"
#include "IU_Player.h"
#include "M_CinematicManager.h"
#include "M_GameMode.h"
#include "M_GameInstance.h"
#include "IU_Macros.h"

AM_LevelTwoTriggerCinematic::AM_LevelTwoTriggerCinematic()
{
	PrimaryActorTick.bCanEverTick = true;

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxCollider->SetupAttachment(RootComponent);
}

void AM_LevelTwoTriggerCinematic::BeginPlay()
{
	Super::BeginPlay();
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AM_LevelTwoTriggerCinematic::OnActorOverlap);
	
}

void AM_LevelTwoTriggerCinematic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AM_LevelTwoTriggerCinematic::OnActorOverlap(UPrimitiveComponent* _this, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _index, bool _sweep, const FHitResult& _result)
{

	if (Cast<AIU_Player>(_otherActor))
	{
		AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
		if(!_gm)return;
		
		_gm->GetCinematicManager()->PlayLevelTwoIntroCinematic();

		Destroy();
	}
}

