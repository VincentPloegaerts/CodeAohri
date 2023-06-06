#include "M_LevelStreamerDeluxeEdition.h"

#include "IU_Macros.h"
#include "M_GameMode.h"
#include "IU_Player.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AM_LevelStreamerDeluxeEdition::AM_LevelStreamerDeluxeEdition()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	colliderToGoForward = CreateDefaultSubobject<UBoxComponent>("GoForwardCollider");
	colliderToGoBackward = CreateDefaultSubobject<UBoxComponent>("GoBackwardCollider");

	billBoard = CreateDefaultSubobject<UBillboardComponent>("BillBoard");
	billBoard->SetupAttachment(RootComponent);
	
	colliderToGoForward->SetupAttachment(RootComponent);
	colliderToGoBackward->SetupAttachment(RootComponent);
}

void AM_LevelStreamerDeluxeEdition::BeginPlay()
{
	Super::BeginPlay();

	Event_AddUnique(colliderToGoForward->OnComponentBeginOverlap, AM_LevelStreamerDeluxeEdition::CollideForward);
	Event_AddUnique(colliderToGoBackward->OnComponentBeginOverlap, AM_LevelStreamerDeluxeEdition::CollideBackward);
	colliderToGoBackward->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorld()->GetTimerManager().SetTimer(initPlayerTimer, this, &AM_LevelStreamerDeluxeEdition::InitPlayer, FMath::RandRange(1.f, 2.f));
}

void AM_LevelStreamerDeluxeEdition::InitPlayer()
{
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		player = _gameMode->GetPlayer();
}

void AM_LevelStreamerDeluxeEdition::CollideForward(COMPONENT_START_COLLISION_PARAMS)
{
	if (player != _otherActor)return;
	
	colliderToGoForward->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	colliderToGoBackward->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	const size_t& _maxLoad = levelsToLoad.Num();
	for (size_t i = 0; i < _maxLoad; ++i)
	{
		FLatentActionInfo _latentInfo;
		_latentInfo.Linkage = 0;
		_latentInfo.UUID = 123 + i;
		UGameplayStatics::LoadStreamLevel(this, levelsToLoad[i], true, false, _latentInfo);
	}
	
	const size_t& _maxUnLoad = levelsToUnload.Num();
	for (size_t i = 0; i < _maxUnLoad; ++i)
	{
		FLatentActionInfo _latentInfo;
		_latentInfo.Linkage = 0;
		_latentInfo.UUID = 123 + i + _maxLoad;
		UGameplayStatics::UnloadStreamLevel(this, levelsToUnload[i], _latentInfo, false);
	}
}

void AM_LevelStreamerDeluxeEdition::CollideBackward(COMPONENT_START_COLLISION_PARAMS)
{
	if (player != _otherActor)return;
	
	colliderToGoForward->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	colliderToGoBackward->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	const size_t& _maxLoad = levelsToLoad.Num();
	for (size_t i = 0; i < _maxLoad; ++i)
	{
		FLatentActionInfo _latentInfo;
		_latentInfo.Linkage = 0;
		_latentInfo.UUID = 123 + i;
		UGameplayStatics::UnloadStreamLevel(this, levelsToLoad[i], _latentInfo, false);
	}
	
	const size_t& _maxUnLoad = levelsToUnload.Num();
	for (size_t i = 0; i < _maxUnLoad; ++i)
	{
		FLatentActionInfo _latentInfo;
		_latentInfo.Linkage = 0;
		_latentInfo.UUID = 123 + i + _maxLoad;
		UGameplayStatics::LoadStreamLevel(this, levelsToUnload[i], true, false, _latentInfo);
	}
}

void AM_LevelStreamerDeluxeEdition::ActivateArena()
{
	colliderToGoForward->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	colliderToGoBackward->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	const size_t& _maxLoad = levelsToLoad.Num();
	for (size_t i = 0; i < _maxLoad; ++i)
	{
		FLatentActionInfo _latentInfo;
		_latentInfo.Linkage = 0;
		_latentInfo.UUID = 123 + i;
		UGameplayStatics::LoadStreamLevel(this, levelsToLoad[i], true, false, _latentInfo);
	}
	
	const size_t& _maxUnLoad = levelsToUnload.Num();
	for (size_t i = 0; i < _maxUnLoad; ++i)
	{
		FLatentActionInfo _latentInfo;
		_latentInfo.Linkage = 0;
		_latentInfo.UUID = 123 + i + _maxLoad;
		UGameplayStatics::UnloadStreamLevel(this, levelsToUnload[i], _latentInfo, false);
	}
}