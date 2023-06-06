#include "M_WorldTimeManager.h"

#include "IU_Macros.h"
#include "M_GameMode.h"
#include "M_GPE_PatternTickManager.h"
#include "M_PatternsTickManager.h"
#include "M_ProjectileManager.h"

AM_WorldTimeManager::AM_WorldTimeManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AM_WorldTimeManager::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	gameMode = world->GetAuthGameMode<AM_GameMode>();
	ROG_ERROR(!gameMode, "AM_WorldTimeManager::BeginPlay -> Gamemode not valid.");
	
	timerManager = &world->GetTimerManager();
	timerManager->SetTimer(timer, this, &AM_WorldTimeManager::InitializeManagers, 0.05f);
}

/*void AM_WorldTimeManager::Tick(float DeltaSeconds)
{
	timerManager->Tick(DeltaSeconds * fCurrentWorldSpeed);
}*/

void AM_WorldTimeManager::InitializeManagers()
{
	patternTickManager = gameMode->GetPatternsTickManager();
	gpePatternTickManager = gameMode->GetGPEPatternTickManager();
	projectileManager = gameMode->GetProjectileManager();

	bIsValid = patternTickManager && gpePatternTickManager && projectileManager;
	if (!bIsValid)
	{
		LOG("AM_WorldTimeManager::InitializeManagers -> There is an invalid manager");
		if (!patternTickManager)
			LOG("AM_WorldTimeManager::InitializeManagers -> patternTickManager is invalid");
		if (!gpePatternTickManager)
			LOG("AM_WorldTimeManager::InitializeManagers -> gpePatternTickManager is invalid");
		if (!projectileManager)
			LOG("AM_WorldTimeManager::InitializeManagers -> projectileManager is invalid");
		return;
	}

	patternTickManager->SetWorldSpeed(fCurrentWorldSpeed);
	gpePatternTickManager->SetWorldSpeed(fCurrentWorldSpeed);
	projectileManager->SetWorldSpeed(fCurrentWorldSpeed);
}

void AM_WorldTimeManager::SetWorldInSlowMotion()
{
	ROG_ERROR(!bIsValid, "AM_WorldTimeManager::SetWorldInSlowMotion -> There is an invalid manager");
	
	fCurrentWorldSpeed = fSlowSpeed;
	
	onWorldSpeedChange.Broadcast(fSlowSpeed);
	patternTickManager->SetWorldSpeed(fSlowSpeed);
	gpePatternTickManager->SetWorldSpeed(fSlowSpeed);
	projectileManager->SetWorldSpeed(fSlowSpeed);
}

void AM_WorldTimeManager::SetWorldClampedSpeed(float _speed)
{
	ROG_ERROR(!bIsValid, "AM_WorldTimeManager::SetWorldClampedSpeed -> There is an invalid manager");
	if (_speed < MIN_WORLD_SPEED)
		_speed = MIN_WORLD_SPEED;
	else if (_speed > MAX_WORLD_SPEED)
		_speed = MAX_WORLD_SPEED;

	fCurrentWorldSpeed = _speed;
	
	onWorldSpeedChange.Broadcast(_speed);
	patternTickManager->SetWorldSpeed(_speed);
	gpePatternTickManager->SetWorldSpeed(_speed);
	projectileManager->SetWorldSpeed(_speed);
}

void AM_WorldTimeManager::SetWorldDefaultSpeed()
{
	ROG_ERROR(!bIsValid, "AM_WorldTimeManager::SetWorldDefaultSpeed -> There is an invalid manager");
	
	fCurrentWorldSpeed = MAX_WORLD_SPEED;
	
	onWorldSpeedChange.Broadcast(MAX_WORLD_SPEED);
	patternTickManager->SetWorldSpeed(MAX_WORLD_SPEED);
	gpePatternTickManager->SetWorldSpeed(MAX_WORLD_SPEED);
	projectileManager->SetWorldSpeed(MAX_WORLD_SPEED);	
}