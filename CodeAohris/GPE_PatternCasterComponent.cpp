
#include "GPE_PatternCasterComponent.h"

#include "IU_Macros.h"
#include "M_GameMode.h"
#include "GPE_Shot.h"
#include "P_PatternShoot.h"
#include "P_PatternShoot.h"
#include "P_PB_ShootArc.h"
#include "P_PB_ShootCircle.h"
#include "P_PB_ShootCross.h"
#include "P_PB_ShootLine.h"
#include "P_PB_ShootPoint.h"

UGPE_PatternCasterComponent::UGPE_PatternCasterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGPE_PatternCasterComponent::CastPattern(UP_DT_BaseDataTable* _dataTable)
{
	if (!_dataTable)
	{
		LOG("UGPE_PatternCasterComponent::CastPattern -> Invalid dataTable");
		return;
	}
	const TSubclassOf<AP_PatternBase>& _patternType = _dataTable->GetPatternType();
	if (!_patternType)
	{
		LOG("UGPE_PatternCasterComponent::CastPattern -> Invalid pattern type");
		return;
	}
	
	const FVector& _location = owner->ShootStartPosition();
	const FRotator& _rotation = owner->DirectionArrow()->GetComponentRotation();
	if (AP_PatternBase* _pattern = world->SpawnActor<AP_PatternBase>(_patternType, _location, _rotation))
	{
		_pattern->AttachToComponent(owner->StaticMesh(),FAttachmentTransformRules::KeepWorldTransform);
		_pattern->SetPatternsTickManager(patternsTickManager);
		_pattern->SetFeedbackManager(feedbackManager);
		_pattern->OnComplete().AddDynamic(this, &UGPE_PatternCasterComponent::PatternBehaviorCompleted);
		_pattern->DelayedInitializePattern(_dataTable, nullptr);
		currentPattern = _pattern;
		
	}
}

void UGPE_PatternCasterComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<AGPE_Shot>();
	world = GetWorld();
	FTimerHandle _timer;
	world->GetTimerManager().SetTimer(_timer, this, &UGPE_PatternCasterComponent::Init, 1, false, 0.05f);	
}

void UGPE_PatternCasterComponent::PatternBehaviorCompleted(AP_PatternBase* _pattern)
{
	_pattern->OnComplete().RemoveDynamic(this, &UGPE_PatternCasterComponent::PatternBehaviorCompleted);
	_pattern->Destroy();
	currentPattern = nullptr;

	GetWorld()->GetTimerManager().ClearTimer(timerClear);
	GetWorld()->GetTimerManager().SetTimer(timerClear, this, &UGPE_PatternCasterComponent::ProjectileClear, fWaitProjectileClear);
	//onPatternComplete.Broadcast();
}

void UGPE_PatternCasterComponent::Init()
{
	if (const AM_GameMode* _gameMode = world->GetAuthGameMode<AM_GameMode>())
	{
		patternsTickManager = _gameMode->GetPatternsTickManager();
		feedbackManager = _gameMode->GetFeedbackManager();
	}
	else
	{
		LOG("UGPE_PatternCasterComponent::Init -> Invalid gamemode");
	}
}

void UGPE_PatternCasterComponent::ProjectileClear()
{
	if (currentPattern) return;
	onProjectileClear.Broadcast();
}
