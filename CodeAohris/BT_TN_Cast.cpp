#include "BT_TN_Cast.h"

#include "A_DT_ArenaDataTable.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Engine/SkeletalMeshSocket.h"

#include "BT_BlackBoard.h"
#include "B_AIBoss.h"

#include "IU_Macros.h"
#include "M_GameMode.h"

EBTNodeResult::Type UBT_TN_Cast::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	bNotifyTaskFinished = true;
	tree = &OwnerComp;
	feedbackManager = GetWorld()->GetAuthGameMode<AM_GameMode>()->GetFeedbackManager();
	if (const AB_AICasterBase* _aiCasterBase = Cast<AB_AICasterBase>(tree->GetAIOwner()))
	{
		patternCaster = _aiCasterBase->GetPatternCaster();
		
		if (!patternCaster)
		{
			LOG("UBT_TN_CastPattern::ExecuteTask -> Invalid pattern caster")
			return EBTNodeResult::Failed;			
		}
	}
	else
	{
		LOG("UBT_TN_CastPattern::ExecuteTask -> Invalid AIController")
		return EBTNodeResult::Failed;
	}
	
	return InitCastTask();
}

EBTNodeResult::Type UBT_TN_Cast::InitCastTask()
{
	//virtual
	return EBTNodeResult::Failed;
}

#if WITH_EDITOR
TArray<FString> UBT_TN_Cast::GetPointNames()
{
	if (const UBT_BlackBoard* _blackBoard = Cast<UBT_BlackBoard>(GetTreeAsset()->GetBlackboardAsset()))
	{
		arenaDataTable = _blackBoard->GetArenaDataTable();
		if (arenaDataTable)
			return arenaDataTable->GetArenaSpawnPoint();
	}
	return TArray<FString>();
}

TArray<FName> UBT_TN_Cast::GetSocketNames()
{
	if (!skeletalMesh)
	{
		if (const UBT_BlackBoard* _blackBoard = Cast<UBT_BlackBoard>(GetTreeAsset()->GetBlackboardAsset()))
			skeletalMesh = _blackBoard->GetSkeletalMesh();
		else
			return { "BLACKBOARD NOT VALID" };

		if (!skeletalMesh)
			return { "NO SKELETAL MESH IN BLACKBOARD" };
	}

	TArray<FName> _names;
	const TArray<USkeletalMeshSocket*> _sockets = skeletalMesh->GetActiveSocketList();
	const int _max = _sockets.Num();
	for (int i = 0; i < _max; i++)
	{
		const USkeletalMeshSocket* _socket = _sockets[i];
		_names.Add(_socket->SocketName);
	}
	if (_names.IsEmpty())
		return { "NO SOCKET FOUND" };
	
	return _names;
}
#endif