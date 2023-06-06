#include "M_GameLogic.h"

#include "IU_CameraManager.h"		//Required for template function CreateManager
#include "M_ArenaManager.h"			//Required for template function CreateManager
#include "M_SpawnPointManager.h"	//Required for template function CreateManager
#include "M_ProjectileManager.h"	//Required for template function CreateManager
#include "M_BossRushLogic.h"		//Required for template function CreateManager

AM_GameLogic::AM_GameLogic()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
}

void AM_GameLogic::InitializeManagers()
{
	world = GetWorld();
	
	cameraManager = CreateManager(cameraManagerType, "Manager_Camera");
	arenaManager = CreateManager(arenaManagerType, "Manager_Arena");
	spawnPointManager = CreateManager(spawnPointManagerType, "Manager_SpawnPoint");
	projectileManager = CreateManager(projectileManagerType, "Manager_Projectile");
	bossRushLogic = CreateManager(bossRushLogicType, "BossRushLogic");

#if WITH_EDITOR
	
	if (!cameraManager)
		UE_LOG(LogTemp, Error, TEXT("AM_GameLogic::InitializeManagers -> Failed to create CameraManager"));
	
	if (!arenaManager)
		UE_LOG(LogTemp, Error, TEXT("AM_GameLogic::InitializeManagers -> Failed to create ArenaManager"));
	
	if (!spawnPointManager)
		UE_LOG(LogTemp, Error, TEXT("AM_GameLogic::InitializeManagers -> Failed to create SpawnPointManager"));
	
	if (!projectileManager)
		UE_LOG(LogTemp, Error, TEXT("AM_GameLogic::InitializeManagers -> Failed to create ProjectileManager"));

	if (!bossRushLogic)
		UE_LOG(LogTemp, Error, TEXT("AM_GameLogic::InitializeManagers -> Failed to create ProjectileManager"));

#endif

	const AActor* _playerStart = playerStart.Get();
	if (IsValid(_playerStart))
	{
		const FVector& _actorLocation = _playerStart->GetActorLocation();
		const FRotator& _actorRotation = _playerStart->GetActorRotation();
		
		SetRespawnLocationAndRotation(true, _actorLocation, _actorRotation);
		SetRespawnLocationAndRotation(true, _actorLocation, _actorRotation);
	}
}

void AM_GameLogic::Checkpoint(const bool _saveProgression)
{
	onCheckpointUpdate.Broadcast();
	onCheckpoint.Broadcast(_saveProgression);
}
