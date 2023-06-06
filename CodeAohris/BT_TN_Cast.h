#pragma once

#include "CoreMinimal.h"
#include "M_FeedbackManager.h"
#include "BehaviorTree/BTTaskNode.h"

#include "STRUCT_SpawnSettings.h"

#include "BT_TN_Cast.generated.h"

class UP_PatternCaster;
class UA_DT_ArenaDataTable;

UCLASS(Abstract)
class INSIDEUS_API UBT_TN_Cast : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	/*The current tree witch this is part of.*/
	UPROPERTY(VisibleAnywhere, Category = "Values")
		UBehaviorTreeComponent* tree = nullptr;
	/*The pattern caster on the boss.*/
	UPROPERTY(VisibleAnywhere, Category = "Values")
		UP_PatternCaster* patternCaster = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Values")
		AM_FeedbackManager* feedbackManager = nullptr;


	// if false je fais rien de plus que le behavior normal et je set bMovementHasFinish to true
	// si je dois attendre du mouvement : je m'abonne au bossmovement || j'attend que le decorator me set ma valeur
	UPROPERTY()
	bool bMovementHasFinish = false;

	/*Set with BehaviorTree Blackboard to show ArenaPoint available.*/
	UPROPERTY()
	UA_DT_ArenaDataTable* arenaDataTable = nullptr;
	UPROPERTY()
	USkeletalMesh* skeletalMesh = nullptr;
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	//Used to Initialize Cast Task Node
	UFUNCTION() virtual EBTNodeResult::Type InitCastTask();

#if WITH_EDITOR
private:
	UFUNCTION(CallInEditor) TArray<FString> GetPointNames();
	UFUNCTION(CallInEditor) TArray<FName> GetSocketNames();
#endif
};
