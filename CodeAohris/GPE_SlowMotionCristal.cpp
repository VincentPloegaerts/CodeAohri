// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE_SlowMotionCristal.h"
#include "UC_PlayerAttackComponent.h"
#include "M_GameMode.h"

AGPE_SlowMotionCristal::AGPE_SlowMotionCristal()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	staticMesh->SetupAttachment(RootComponent);
	
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxCollider->SetupAttachment(RootComponent);

	healthComponent = CreateDefaultSubobject<UIU_HealthSystemComponent>("Health Component");
	AddOwnedComponent(healthComponent);
}

void AGPE_SlowMotionCristal::BeginPlay()
{

	Super::BeginPlay();

	

	healthComponent->OnDeath()->AddDynamic(this,&AGPE_SlowMotionCristal::DeathBehaviour);
	
}

void AGPE_SlowMotionCristal::DeathBehaviour()
{


	AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
	if(!_gm)return;

	AIU_Player* _player = _gm->GetPlayer();
	if(!_player)return;

	_player->GetCharacterAttackComponent()->IncreaseCurrentSlowMoOrbCount(amountOfSlowMoPowerAddedWhenBroken);


	FTimerHandle _respawnTimer;
	GetWorld()->GetTimerManager().SetTimer(_respawnTimer, this, &AGPE_SlowMotionCristal::RespawnBehaviour, timeBeforeRespawn, false);
	
	boxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	staticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	staticMesh->SetVisibility(false);
	boxCollider->SetVisibility(false);

	
	
}

void AGPE_SlowMotionCristal::RespawnBehaviour()
{


	staticMesh->SetVisibility(true);
	boxCollider->SetVisibility(true);
	staticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	healthComponent->ResetHealth();
	
}
