#include "B_AICasterBase.h"

#include "BrainComponent.h"
#include "B_Boss.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "M_GameMode.h"
#include "P_PatternCaster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BT_BlackBoard.h"
#include "M_ProjectileManager.h"

void AB_AICasterBase::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	if (const AM_GameMode* _gameMode = world->GetAuthGameMode<AM_GameMode>())
	{
		blackBoardManager = _gameMode->GetBlackBoardManager();
	}
}

void AB_AICasterBase::InitAIBossPhase()
{
	//virtual
}

void AB_AICasterBase::SetBehaviorTree(UBehaviorTree* _tree)
{
	if (tree)
		BrainComponent->StopLogic("Behavior Tree on AIBoss changed.");
	
	tree = _tree;
}

void AB_AICasterBase::InitializeBlackBoard() const
{
	InitializeBlackBoardDefaultKeys();
}

void AB_AICasterBase::InitializeBlackBoardDefaultKeys() const
{
	if (!Blackboard) return;
	
	if (const AM_GameMode* _gameMode = world->GetAuthGameMode<AM_GameMode>())
	{
		if (AB_Boss* _boss = _gameMode->GetBoss())
		{
			Blackboard->SetValueAsObject(bossKeyName, _boss);
		}
		else
			LOG("AB_AICasterBase::InitializeBlackBoardDefaultKeys : Pawn not valid.");
	
		if (AIU_Player* _player = _gameMode->GetPlayer())
			Blackboard->SetValueAsObject(playerKeyName, _player);
		else
			LOG("AB_AICasterBase::InitializeBlackBoardDefaultKeys : IU_Player in M_Gamemode not valid.");
	}
	else
		LOG("AB_AICasterBase::InitializeBlackBoardDefaultKeys : M_GameMode not found.");
}

void AB_AICasterBase::ActivateAIBaseController()
{
	if (!tree) return;
	
	RunBehaviorTree(tree);
	InitializeBlackBoard();
	
	if (blackBoardManager)
		InitAIBossPhase();
}

void AB_AICasterBase::ResetAIBaseController()
{
	DeactivateBehaviorTree();
}

void AB_AICasterBase::StunBehaviorTree()
{
	ROG_ERROR(!Blackboard, "AB_AICasterBase::StunBehaviorTree -> Impossible to stun cause Blackboard invalid");
	Blackboard->SetValueAsBool(isStunKeyName, true);
	ROG_ERROR(!patternCaster, "AB_AICasterBase::StunBehaviorTree -> Impossible to stun cause pattern caster invalid");
	patternCaster->OnSerieEnd().Broadcast();
}

void AB_AICasterBase::EndStun()
{
	Blackboard->SetValueAsBool(isStunKeyName, false);
}

void AB_AICasterBase::DeactivateBehaviorTree()
{
	ROG_ERROR(!patternCaster, "AB_AICasterBase::DeactivateBehaviorTree -> Pattern Caster is nullptr, so we can't stun it");
	
	patternCaster->ClearTimer();
	if (BrainComponent)
		BrainComponent->StopLogic("Because");
	
	FTimerHandle _timer;
	world->GetTimerManager().SetTimer(_timer, this, &AB_AICasterBase::DestroyGPEAndProjectile, BOSS_RESET_COOLDOWN);
	onStopBehaviorTree.Broadcast();
}

void AB_AICasterBase::DestroyGPEAndProjectile()
{
	patternCaster->DestroyEveryPattern();
	if (const AM_GameMode* _gameMode = world->GetAuthGameMode<AM_GameMode>())
	{
		if (AM_ProjectileManager* _projectileManager = _gameMode->GetProjectileManager())
		{
			_projectileManager->ReturnAllProjectile();
		}
	}
}