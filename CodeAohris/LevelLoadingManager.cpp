#include "LevelLoadingManager.h"
#include "EngineUtils.h"
//#include "IU_Macros.h"
#include "IU_Macros.h"
#include "M_CinematicManager.h"
#include "TriggerBoxLevelLoading.h"
#include "Kismet/GameplayStatics.h"

ALevelLoadingManager::ALevelLoadingManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelLoadingManager::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ALevelLoadingManager::Init()
{
	QUICK_SCOPE_CYCLE_COUNTER(ALevelLoadingManager_Init);
	if (const TSubclassOf<ATriggerBoxLevelLoading> _actorClass = ATriggerBoxLevelLoading::StaticClass())
	{
		if (const UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull))
		{
			for (TActorIterator It(World, _actorClass); It; ++It)
			{
				ATriggerBoxLevelLoading* Actor = *It;
				Actor->Init(this);
			}
		}
	}
	
	if (ShouldLoadStartLevelFromManager() && levelsToLoadOnStart.Num() > 0)
	{
		levelsToLoadLeft.Append(levelsToLoadOnStart);
		LoadLevel(levelsToLoadLeft[0].GetAssetName());
	}
}

void ALevelLoadingManager::InitTriggerBoxes(TArray<ATriggerBoxLevelLoading*> _triggerArray)
{
	const int& _max = _triggerArray.Num();
	for (int i = 0; i< _max; i++)
	{
		if (_triggerArray.IsValidIndex(i) && _triggerArray[i])
			_triggerArray[i]->Init(this);
	}
}

void ALevelLoadingManager::AddLevels(TArray<TSoftObjectPtr<UWorld>> _toLoad, TArray<TSoftObjectPtr<UWorld>> _toUnload)
{
	//LOG("AddLevels");
	levelsToLoadLeft.Append(_toLoad);
	levelsToUnloadLeft.Append(_toUnload);
	if(levelsToLoadLeft.Num() > 0 && levelsToLoadLeft.Num() == _toLoad.Num())
		LoadLevel(levelsToLoadLeft[0].GetAssetName());
	else if (levelsToLoadLeft.Num() == 0 && levelsToUnloadLeft.Num() >0 && levelsToUnloadLeft.Num() == _toUnload.Num())
		UnloadLevel(levelsToUnloadLeft[0].GetAssetName());
}

void ALevelLoadingManager::LoadLevel(const FString& _levelName)
{
	//LOG("Load %s",*_levelName);
	FLatentActionInfo _latentInfo;
	_latentInfo.CallbackTarget = this;
	_latentInfo.ExecutionFunction = "LevelLoadStreamFinished";
	_latentInfo.UUID = 123;
	_latentInfo.Linkage = 0;
	UGameplayStatics::LoadStreamLevel(this,FName(*_levelName),true,false,_latentInfo);
}

void ALevelLoadingManager::UnloadLevel(const FString& _levelName)
{
	//LOG("Unload %s",*_levelName);
	FLatentActionInfo _latentInfo;
	_latentInfo.CallbackTarget = this;
	_latentInfo.ExecutionFunction = "LevelUnloadStreamFinished";
	_latentInfo.UUID = 123;
	_latentInfo.Linkage = 0;
	UGameplayStatics::UnloadStreamLevel(this,FName(*_levelName),_latentInfo,false);
}

void ALevelLoadingManager::LevelLoadStreamFinished()
{	
	if (levelsToLoadLeft.Num()  == 0)return;
	
	levelsToLoadLeft.RemoveAt(0);
	if (levelsToLoadLeft.Num() >0)
	{
		
		if (delayBetweenLoads >0 )
			CallLoadingWithDelay("LoadLevel",levelsToLoadLeft[0].GetAssetName(),delayBetweenLoads);
		else
			LoadLevel(levelsToLoadLeft[0].GetAssetName());
	}
	else if (levelsToUnloadLeft.Num() > 0)
	{
		if (delayBetweenLoads > 0.0f )
			CallLoadingWithDelay("UnloadLevel",levelsToUnloadLeft[0].GetAssetName(),delayBetweenLoads);
		else
			UnloadLevel(levelsToUnloadLeft[0].GetAssetName());
	}
	else
	{
		ON_SCREEN_LOG_TIME(15, FColor::Green, "On Finished Loading Broadcast : LevelLoadStreamFinished");	
		onFinishedLoading.Broadcast();
	}
	
}

void ALevelLoadingManager::LevelUnloadStreamFinished()
{
	
	//LOG("Unload finished");
	if (levelsToUnloadLeft.Num() == 0)return;

	levelsToUnloadLeft.RemoveAt(0);
	
	if (levelsToUnloadLeft.Num() > 0)
	{
		if (delayBetweenLoads > 0.0f )
			CallLoadingWithDelay("UnloadLevel",levelsToUnloadLeft[0].GetAssetName(),delayBetweenLoads);
		else
			UnloadLevel(levelsToUnloadLeft[0].GetAssetName());
	}
	else if (levelsToLoadLeft.Num() > 0)
	{
		if (delayBetweenLoads >0 )
			CallLoadingWithDelay("LoadLevel",levelsToLoadLeft[0].GetAssetName(),delayBetweenLoads);
		else
			LoadLevel(levelsToLoadLeft[0].GetAssetName());	
	}
	else
	{
		ON_SCREEN_LOG_TIME(15, FColor::Green, "On Finished Loading Broadcast : LevelUnloadStreamFinished");	
		onFinishedLoading.Broadcast();	
	}
}

void ALevelLoadingManager::CallLoadingWithDelay(const FName& _functionName, const FString& _parameter, const float& _delay)
{
	FTimerDelegate _delegate;
	_delegate.BindUFunction(this,_functionName,_parameter);
	GetWorld()->GetTimerManager().SetTimer(handle,_delegate,_delay,false);
}

bool ALevelLoadingManager::ShouldLoadStartLevelFromManager()
{
	return false;
}
