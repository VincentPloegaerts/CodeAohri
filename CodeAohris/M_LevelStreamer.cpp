#include "M_LevelStreamer.h"

#include "IU_Macros.h"
#include "Kismet/GameplayStatics.h"

AM_LevelStreamer::AM_LevelStreamer()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	arrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction"));
	
	triggerBox->SetupAttachment(RootComponent);
	arrowComponent->SetupAttachment(RootComponent);
}

void AM_LevelStreamer::Rollback()
{
	currentLevelsToLoad = levelsToUnload;
	iLoadIndex = 0;
	bCanLoad = currentLevelsToLoad.Num() > 0;
	
	currentLevelsToUnload = levelsToLoad;
	iUnloadIndex = 0;
	bCanUnload = currentLevelsToUnload.Num() > 0;
}

void AM_LevelStreamer::BeginPlay()
{
	Super::BeginPlay();

//#if !WITH_EDITOR
	currentLevelsToLoad = levelsToLoad;
	currentLevelsToUnload = levelsToUnload;
	triggerBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AM_LevelStreamer::OverlapBegins);
	triggerBox->OnComponentEndOverlap.AddUniqueDynamic(this, &AM_LevelStreamer::OverlapEnds);
//#endif
}

void AM_LevelStreamer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

//#if !WITH_EDITOR
	StreamLevel(iLoadIndex, "EndLoad", currentLevelsToLoad, iLoadIndex, bCanLoad);
	StreamLevel(iLoadIndex + iUnloadIndex + 1, "EndUnload", currentLevelsToUnload, iUnloadIndex, bCanUnload, false);
//#endif
}

void AM_LevelStreamer::StreamLevel(int _uuidOffset, FName _callbackFunction, TArray<TSoftObjectPtr<UWorld>> _levels, int& _index, bool& _bool, bool _load)
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

void AM_LevelStreamer::DetectOverlapLoad(FVector _location)
{
	if(FVector::DotProduct(GetActorLocation() - _location, GetActorForwardVector()) > 0)
		currentLevelsToLoad = levelsToLoad;
	else
		currentLevelsToLoad = levelsToUnload;
}

void AM_LevelStreamer::DetectOverlapUnload(FVector _location)
{
	if(FVector::DotProduct(GetActorLocation() - _location, GetActorForwardVector()) > 0)
		currentLevelsToUnload = levelsToLoad;
	else
		currentLevelsToUnload = levelsToUnload;
}

void AM_LevelStreamer::OverlapBegins(COMPONENT_START_COLLISION_PARAMS)
{
	DetectOverlapLoad(_otherActor->GetActorLocation());
	
	if(currentLevelsToLoad.Num() <= 0)
		return;

	iLoadIndex = 0;
	bCanLoad = true;
}

void AM_LevelStreamer::OverlapEnds(COMPONENT_STOP_COLLISION_PARAMS)
{
	DetectOverlapUnload(_otherActor->GetActorLocation());
	
	if(currentLevelsToUnload.Num() <= 0)
		return;

	iUnloadIndex = 0;
	bCanUnload = true;
}

void AM_LevelStreamer::EndLoad()
{
	if(iLoadIndex < currentLevelsToLoad.Num())
		bCanLoad = true;
	else
	{
		bCanLoad = false;
		iLoadIndex = 0;
	}
}

void AM_LevelStreamer::EndUnload()
{
	if(iUnloadIndex < currentLevelsToUnload.Num())
		bCanUnload = true;
	else
	{
		bCanUnload = false;
		iUnloadIndex = 0;
	}
}

void AM_LevelStreamer::UnloadLevelsBehindBoss()
{
	const uint& _maxUnLoad = currentLevelsToUnload.Num();
	if(_maxUnLoad > 0)
	{
		for (uint i = 0; i < _maxUnLoad; ++i)
		{
			FLatentActionInfo _latentInfo;
			_latentInfo.Linkage = 0;
			_latentInfo.UUID = 123 + iLoadIndex + i + 1;
			UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, currentLevelsToUnload[i], _latentInfo, false);
		}
	}
	
	const uint& _maxLoad = currentLevelsToLoad.Num();
	if (_maxLoad > 0)
	{
		for (uint i = 0; i < _maxLoad; ++i)
		{
			FLatentActionInfo _latentInfo;
			_latentInfo.Linkage = 0;
			_latentInfo.UUID = 123 + iLoadIndex + i;
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, currentLevelsToLoad[i], true, false, _latentInfo);
		}
	}
}