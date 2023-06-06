#include "TriggerBoxLevelLoading.h"

#include "LevelLoadingManager.h"
#include "Components/BoxComponent.h"

ATriggerBoxLevelLoading::ATriggerBoxLevelLoading()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	triggerBoxForward = CreateDefaultSubobject<UBoxComponent>("TriggerBoxForward");
	triggerBoxForward->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
}

void ATriggerBoxLevelLoading::Init(ALevelLoadingManager* _levelLoadingManager)
{
	levelLoadingManagerRef = _levelLoadingManager;
	triggerBoxForward->OnComponentBeginOverlap.AddUniqueDynamic(this, &ATriggerBoxLevelLoading::OnCollisionForward);
	if (bufferLoadLevels && levelLoadingManagerRef)
	{
		bufferLoadLevels = false;
		levelLoadingManagerRef->AddLevels(levelsToLoad,levelsToUnload);
	}
}

void ATriggerBoxLevelLoading::AddLevels()
{
	if (levelLoadingManagerRef)
	{
		levelLoadingManagerRef->AddLevels(levelsToLoad, levelsToUnload);
		return;
	}
	bufferLoadLevels = true;
}

void ATriggerBoxLevelLoading::OnCollisionForward(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* otherComp, int32 _otherBodyIndex,
                                                 bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (levelLoadingManagerRef)
	{
		levelLoadingManagerRef->AddLevels(levelsToLoad, levelsToUnload);
	}
}

void ATriggerBoxLevelLoading::OnCollisionBackward(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* otherComp, int32 _otherBodyIndex,
                                                  bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (levelLoadingManagerRef)
	{
		levelLoadingManagerRef->AddLevels(levelsToUnload, levelsToLoad);
	}
}
