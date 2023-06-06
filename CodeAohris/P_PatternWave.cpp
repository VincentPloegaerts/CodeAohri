#include "P_PatternWave.h"

#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "M_PatternsTickManager.h"
#include "P_DT_WaveDataTable.h"
#include "P_PB_Wave.h"
#include "IU_Macros.h"
#include "Components/BoxComponent.h"

AP_PatternWave::AP_PatternWave()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	poseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMeshComponent"));
	poseableMesh->SetupAttachment(RootComponent);
	
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	boxCollider->SetupAttachment(RootComponent);
	boxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	waveBehavior = CreateDefaultSubobject<UP_PB_Wave>(TEXT("WaveComponent"));
	AddOwnedComponent(waveBehavior);
}

void AP_PatternWave::InitializePattern(UP_DT_BaseDataTable* _dataTable)
{
	Super::InitializePattern(_dataTable);

	UP_DT_WaveDataTable* _waveDataTable = Cast<UP_DT_WaveDataTable>(_dataTable);
	if (!_waveDataTable)
	{
		LOG("AP_PatternWave::InitializePattern -> Invalid Data Table");
		return;
	}

	InitializeBehavior(_waveDataTable->GetOverrideWave(), waveBehavior, _waveDataTable, patternsTickManager->GetOnTick());
}

void AP_PatternWave::InitializePatternData(FPatternData& _patternData)
{
	Super::InitializePatternData(_patternData);

	InitializeBehavior(_patternData.GetOverrideWave(), waveBehavior, _patternData, patternsTickManager->GetOnTick());
}

void AP_PatternWave::PatternCompletion()
{
	Super::PatternCompletion();
	patternsTickManager->GetOnTick().RemoveDynamic(waveBehavior, &UP_PB_Wave::TickBehavior);
	waveBehavior->OnBehaviorComplete().RemoveDynamic(this, &AP_PatternBase::PatternCompletion);
	waveBehavior->DeActivateBehavior();
	Destroy();
}
