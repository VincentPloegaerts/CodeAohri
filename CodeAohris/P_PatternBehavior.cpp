#include "P_PatternBehavior.h"

UP_PatternBehavior::UP_PatternBehavior()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UP_PatternBehavior::BeginPlay()
{
	Super::BeginPlay();
	
	owner = GetOwner();
}

void UP_PatternBehavior::ActivateBehavior(UP_DT_BaseDataTable* _dataTable)
{
	if (!owner) return;
	
	bIsEnable = true;
}

void UP_PatternBehavior::ActivateBehavior(FPatternData& _patternData)
{
	if (!owner) return;
	
	bIsEnable = true;
}

void UP_PatternBehavior::DeActivateBehavior()
{
	bIsEnable = false;
}

void UP_PatternBehavior::TickBehavior(const float& _deltaTime)
{
	//BehaviorTick
}