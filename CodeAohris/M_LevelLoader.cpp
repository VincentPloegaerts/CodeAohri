#include "M_LevelLoader.h"

#include "M_GameMode.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"

AM_LevelLoader::AM_LevelLoader()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AM_LevelLoader::BeginPlay()
{
	Super::BeginPlay();
	SetTickableWhenPaused(true);

//#if !WITH_EDITOR
	if(AM_CinematicManager* _cinematic = GetCinematicManager())
		_cinematic->OnCinematicFinishedEvent().AddDynamic(this, &AM_LevelLoader::SetEndOfCinematic);
//#endif
}

void AM_LevelLoader::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

//#if !WITH_EDITOR
	StreamLevel(iLoadIndex, "EndLoad", levelsToLoad, iLoadIndex, bCanLoad);

	if(bUseLoader)
		StreamLevel(iUnloadIndex, "EndUnload", levelsToUnload, iUnloadIndex, bCanUnload, false);
//#endif
}

AM_CinematicManager* AM_LevelLoader::GetCinematicManager()
{
	if(const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		return _gameMode->GetCinematicManager();
	
	return nullptr;
}

void AM_LevelLoader::StreamLevel(int _uuidOffset, FName _callbackFunction, TArray<TSoftObjectPtr<UWorld>> _levels, int& _index, bool& _bool, bool _load)
{
	if(!_bool)
		return;
	
	FLatentActionInfo _latentInfo;
	_latentInfo.UUID = 123 + _uuidOffset;
	_latentInfo.Linkage = 0;
	_latentInfo.CallbackTarget = this;
	_latentInfo.ExecutionFunction = _callbackFunction;

	if(_load)
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, _levels[_index], true, false,	_latentInfo);
	else
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, _levels[_index], _latentInfo, false);

	_bool = false;
	_index += 1;
}

void AM_LevelLoader::EndLoad()
{	
	if(iLoadIndex < levelsToLoad.Num())
		bCanLoad = true;
	else
	{
		bCanLoad = false;
		iLoadIndex = 0;

		if(!bUseLoader)
			Destroy();
	}
}

void AM_LevelLoader::EndUnload()
{	
	if(iUnloadIndex < levelsToUnload.Num())
		bCanUnload = true;
	else
	{
		bCanUnload = false;
		iUnloadIndex = 0;
		Destroy();
	}
}

void AM_LevelLoader::SetEndOfCinematic()
{
	if(!bUseLoader)
		return;
	
	bCanUnload = true;
	bCanLoad = false;
	iLoadIndex = 999;
}


