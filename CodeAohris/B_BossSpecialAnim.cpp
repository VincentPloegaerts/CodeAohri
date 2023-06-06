#include "B_BossSpecialAnim.h"

#include "B_Boss.h"
#include "P_PatternCaster.h"
#include "B_BossAnimatedStone.h"
#include "M_GameMode.h"
#include "P_DT_BaseDataTable.h"
#include "P_PatternBase.h"

UB_BossSpecialAnim::UB_BossSpecialAnim()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UB_BossSpecialAnim::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	timerManager = &world->GetTimerManager();
	ROG_ERROR(!timerManager, "UB_BossSpecialAnim::BeginPlay -> Timer manager doesn't exist");

	timerManager->SetTimer(initTimer, this, &UB_BossSpecialAnim::Init, 0.24f);
}

void UB_BossSpecialAnim::Init()
{
	boss = Cast<AB_Boss>(GetOwner());
	ROG_ERROR(!boss, "UB_BossSpecialAnim::Init -> Invalid boss owner");
	
	patternCaster = boss->GetPatternCaster();
	ROG_ERROR(!patternCaster, "UB_BossSpecialAnim::Init -> PatternCaster");

	if (const AM_GameMode* _gameMode = world->GetAuthGameMode<AM_GameMode>())
		feedBackManager = _gameMode->GetFeedbackManager();

	ROG_ERROR(!feedBackManager, "UB_BossSpecialAnim::Init -> Invalid feedback manager");
	ROG_ERROR(!bossStoneToSpawn.Get(), "UB_BossSpecialAnim::Init -> Invalid boss stone to spawn");

	Event_AddUnique(boss->OnBossStunned(), UB_BossSpecialAnim::DestroyEveryBossStone);
	Event_AddUnique(boss->OnBossDead(), UB_BossSpecialAnim::DestroyEveryBossStone);
	Event_AddUnique(boss->OnBossMidLifeAnimStart(), UB_BossSpecialAnim::DestroyEveryBossStone);
	Event_AddUnique(boss->OnBossReset(), UB_BossSpecialAnim::DestroyEveryBossStone);
	
	Event_AddUnique(patternCaster->OnPatternDataTableCasted(), UB_BossSpecialAnim::AddNewSpecialAnimation);
}

void UB_BossSpecialAnim::AddNewSpecialAnimation(const UP_DT_BaseDataTable* _dataTable, AP_PatternBase* _pattern)
{
	ROG_ERROR(!_dataTable, "UB_BossSpecialAnim::AddNewSpecialAnimation -> Invalid data table");
	ROG_ERROR(!_pattern, "UB_BossSpecialAnim::AddNewSpecialAnimation -> Invalid pattern");

	const FTripleDeluxeFeedBack& _feedBack = _dataTable->GetTripleDeluxeFeedBack();
	if (_feedBack.animRigMode == EAnimRigMode::BOSS__)return;

	const FBossAnimatedStoneData& _stoneData = _feedBack.animatedStoneData;
	
	FVector _location;
	if (_stoneData.bIsRelativeLocation)
	{
		const FVector& _offset = _stoneData.vLocationOffset;
		_location = _pattern->GetActorForwardVector() * _offset + _pattern->GetActorRightVector() * _offset + _pattern->GetActorUpVector() * _offset;
	}
	else
	{
		_location = _pattern->GetActorLocation() + _stoneData.vLocationOffset;
	}

	const FRotator& _rotation = _pattern->GetActorRotation() + _stoneData.vRotationOffset;
	
	AB_BossAnimatedStone* _bossStone = world->SpawnActor<AB_BossAnimatedStone>(bossStoneToSpawn.Get(), _location, _rotation);
	
	ROG_ERROR(!_bossStone, "UB_BossSpecialAnim::AddNewSpecialAnimation -> Spawn failed");
	
	Event_AddUnique(_pattern->OnComplete(), UB_BossSpecialAnim::RemoveSpecialAnimation);

	if (_feedBack.animatedStoneData.bAttachToPattern)
	{
		const FAttachmentTransformRules _rules(EAttachmentRule::SnapToTarget, false);
		_bossStone->AttachToActor(_pattern, _rules);
	}
	_bossStone->SetDataTable(_dataTable);
	_bossStone->SetPatternIndex(_pattern->GetPatternIndex());
	
	Event_AddUnique(_bossStone->OnEndAnimation(), UB_BossSpecialAnim::DestroyBossStone);
	
	_bossStone->PlayStartAnimation(feedBackManager);
	
	allBossStone.Add(_bossStone);
}

void UB_BossSpecialAnim::RemoveSpecialAnimation(AP_PatternBase* _pattern)
{
	ROG_ERROR(!_pattern, "UB_BossSpecialAnim::RemoveSpecialAnimation -> Pattern nullptr");
	Event_Remove(_pattern->OnComplete(), UB_BossSpecialAnim::RemoveSpecialAnimation);
	
	AB_BossAnimatedStone* _bossStone = GetSameIDBossStone(_pattern->GetPatternIndex());
	ROG_ERROR(!_bossStone, "UB_BossSpecialAnim::RemoveSpecialAnimation -> Missing correspondant boss stone and pattern id");

	Event_AddUnique(_bossStone->OnEndAnimation(), UB_BossSpecialAnim::DestroyBossStone);
	
	_bossStone->PlayEndAnimation();
}

void UB_BossSpecialAnim::DestroyBossStone(AB_BossAnimatedStone* _bossStone)
{
	ROG_ERROR(!_bossStone, "UB_BossSpecialAnim::DestroyBossStone -> boss Stone is nullptr");
	Event_Remove(_bossStone->OnEndAnimation(), UB_BossSpecialAnim::DestroyBossStone);

	allBossStone.Remove(_bossStone);
	allBossStone.Shrink();

	_bossStone->Destroy();
}

void UB_BossSpecialAnim::DestroyEveryBossStone()
{
	const uint& _max = allBossStone.Num();
	
	for (uint i = 0; i < _max; ++i)
		allBossStone[i]->Destroy();
	
	allBossStone.Empty();
}

AB_BossAnimatedStone* UB_BossSpecialAnim::GetSameIDBossStone(const uint& _id) const
{
	for (AB_BossAnimatedStone* _bossStone : allBossStone)
	{
		ROG_ERROR_NULLPTR(!_bossStone, "UB_BossSpecialAnim::GetSameIDBossStone -> Boss stone is nullptr");
		if (_bossStone->GetPatternIndex() == _id)
			return _bossStone;
	}
	return nullptr;
}