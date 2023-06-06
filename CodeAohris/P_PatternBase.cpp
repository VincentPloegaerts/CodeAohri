#include "P_PatternBase.h"

#include "IU_Macros.h"
#include "M_FeedbackManager.h"
#include "P_DT_BaseDataTable.h"
#include "P_PatternBehavior.h"

AP_PatternBase::AP_PatternBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP_PatternBase::BeginPlay()
{
	Super::BeginPlay();
	//Register to a manager ?
}

void AP_PatternBase::InitializePattern(UP_DT_BaseDataTable* _dataTable)
{
	dataTable = _dataTable;
	//Activate Behavior Component depends on booleans define in DataTable
	//Declaration in Child class
}

void AP_PatternBase::InitializePatternData(FPatternData& _patternData)
{
	
}

void AP_PatternBase::InitializeBehavior(const bool& _state, UP_PatternBehavior* _behavior, UP_DT_BaseDataTable* _dataTable, AM_PatternsTickManager::FOnTick& _onTick)
{
	if (_behavior && _state)
	{
		_behavior->ActivateBehavior(_dataTable);
		_behavior->OnBehaviorComplete().AddDynamic(this, &AP_PatternBase::PatternCompletion);
		_onTick.AddDynamic(_behavior, &UP_PatternBehavior::TickBehavior);
	}
}

void AP_PatternBase::InitializeBehavior(const bool& _state, UP_PatternBehavior* _behavior, FPatternData& _patternData, AM_PatternsTickManager::FOnTick& _onTick)
{
	if (_behavior && _state)
	{
		_behavior->ActivateBehavior(_patternData);
		_behavior->OnBehaviorComplete().AddDynamic(this, &AP_PatternBase::PatternCompletion);
		_onTick.AddDynamic(_behavior, &UP_PatternBehavior::TickBehavior);
	}
}

void AP_PatternBase::DeActivatePatternBehavior()
{
	//DeActivate Behavior Component
	//Declaration in Child class
}
void AP_PatternBase::DeActivateBehavior(UP_PatternBehavior* _behavior, AM_PatternsTickManager::FOnTick& _onTick)
{
	_behavior->DeActivateBehavior();
	_behavior->OnBehaviorComplete().RemoveDynamic(this, &AP_PatternBase::PatternCompletion);
	_onTick.RemoveDynamic(_behavior, &UP_PatternBehavior::TickBehavior);	
}

void AP_PatternBase::DelayedInitializePattern(UP_DT_BaseDataTable* _dataTable, const FTripleDeluxeFeedBack* _overridedFeedBack, const bool& _isDuplicated, const bool& _dupFX)
{
	dataTable = _dataTable;
	float _delay = _dataTable->GetBehaviorActivationDelay();

	if (_overridedFeedBack)
	{
		_delay = _overridedFeedBack->startFeedBack.fBehaviorActivationDelay;
		if (_overridedFeedBack->animRigMode != EAnimRigMode::STONE)
		{
			if (!_isDuplicated)
				feedbackManager->RequestPlayTripleFeedbacks(*_overridedFeedBack,this);
			else if (_dupFX)
				feedbackManager->RequestPlayFX(_overridedFeedBack->startFeedBack,this);
		}
	}
	else
	{
		const FTripleDeluxeFeedBack& _tripleFeedback = _dataTable->GetTripleDeluxeFeedBack();
		if (_tripleFeedback.animRigMode != EAnimRigMode::STONE)
		{
			if (!_isDuplicated)
			{
				if (_tripleFeedback.bIsGPEShotFeedBack)
					feedbackManager->RequestPlayGPEShotFeedBack(_tripleFeedback, this);
				else
					feedbackManager->RequestPlayTripleFeedbacks(_tripleFeedback,this);
				
			}
			else if (_dupFX)
				feedbackManager->RequestPlayFX(_tripleFeedback.startFeedBack,this);
		}
	}
	
	if (_delay > 0.f)
	{
		FTimerHandle _handle;
		FTimerDelegate _delegate;
		_delegate.BindUFunction(this, "InitializePattern", _dataTable);
		GetWorld()->GetTimerManager().SetTimer(_handle,_delegate,_delay,false);
		return;
	}
	InitializePattern(_dataTable);
}

void AP_PatternBase::DelayedInitializePatternData(FPatternData& _patternData)
{
	feedbackManager->RequestPlayTripleFeedbacks(_patternData.GetTripleFeedBackData(),this);
	const float& _delay = _patternData.GetTripleFeedBackData().startFeedBack.fBehaviorActivationDelay;
	if (_delay > 0.f)
	{
		FTimerHandle _handle;
		FTimerDelegate _delegate;
		_delegate.BindUFunction(this,"InitializePatternData",_patternData);
		GetWorld()->GetTimerManager().SetTimer(_handle,_delegate,_delay,false);
		return;
	}
	InitializePatternData(_patternData);
}


void AP_PatternBase::PatternCompletion()
{
	//If Manager reset...
	DeActivatePatternBehavior();
	onComplete.Broadcast(this);
}