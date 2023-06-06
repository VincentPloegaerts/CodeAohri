#include "M_SpawnPointManager.h"

#include "A_Arena.h"
#include "M_ArenaManager.h"
#include "M_GameMode.h"
#include "B_Boss.h"
#include "IU_Player.h"
#include "Engine/SkeletalMeshSocket.h"
#include "IU_Macros.h"
#include "M_BossManager.h"

AM_SpawnPointManager::AM_SpawnPointManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AM_SpawnPointManager::BeginPlay()
{
	Super::BeginPlay();
	
	world = GetWorld();
	world->GetTimerManager().SetTimer(timer, this, &AM_SpawnPointManager::InitializeSpawnPoints, 1, false, 0.05f);
}

const AActor* AM_SpawnPointManager::GetArenaPoint(const FString& _pointName) const
{
	if (const AA_Arena* _arena = arenaManager->GetArena())
		return _arena->GetArenaPoint(_pointName);
	
	return nullptr;
}

bool AM_SpawnPointManager::AttachToSocket(const FName& _socketName, AActor* _toAttach) const
{
	if (boss)
		if (USkeletalMeshComponent* _skeletalMesh = boss->GetSkeletalMeshComponent())
			if (const USkeletalMeshSocket* _socket = _skeletalMesh->GetSocketByName(_socketName))
				return _socket->AttachActor(_toAttach, _skeletalMesh);

	return false;
}

FVector AM_SpawnPointManager::GetSocketLocation(const FName& _socketName) const
{
	if (boss)
		if (const USkeletalMeshComponent* _skeletalMesh = boss->GetSkeletalMeshComponent())
			if (const USkeletalMeshSocket* _socket = _skeletalMesh->GetSocketByName(_socketName))
				return _socket->GetSocketLocation(_skeletalMesh);
	
	return FVector::ZeroVector;
}

const AActor* AM_SpawnPointManager::GetPoint(const ESpawnSettings& _spawnSettings, const FString& _pointName) const
{
	if (_spawnSettings == ESpawnSettings::BOSS)
		return boss;
	if (_spawnSettings == ESpawnSettings::PLAYER)
		return player;
	if (_spawnSettings == ESpawnSettings::ARENA_POINT)
		return GetArenaPoint(_pointName);

	return nullptr;
}

FVector AM_SpawnPointManager::GetPointLocation(const ESpawnSettings& _spawnSettings, const FString& _pointName) const
{
	if (_spawnSettings == ESpawnSettings::BOSS)
		return boss->GetActorLocation();
	if (_spawnSettings == ESpawnSettings::PLAYER)
		return player->GetActorLocation();
	if (_spawnSettings == ESpawnSettings::ARENA_POINT)
		return GetArenaPoint(_pointName)->GetActorLocation();

	return GetSocketLocation(*_pointName);
}

void AM_SpawnPointManager::InitializeSpawnPoints()
{
	gameMode = world->GetAuthGameMode<AM_GameMode>();
	arenaManager = gameMode->GetArenaManager();
	player = gameMode->GetPlayer();
	if (AM_BossManager* _bossManager = gameMode->GetBossManager())
	{
		boss = _bossManager->GetCurrentBoss();
		Event_AddUnique(_bossManager->OnBossChanged(), AM_SpawnPointManager::ChangeCurrentBoss);
	}
}

void AM_SpawnPointManager::ChangeCurrentBoss(AB_Boss* _boss)
{
	boss = _boss;
}