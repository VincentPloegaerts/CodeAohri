#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BT_KeyNames.h"
#include "B_AICasterBase.generated.h"

class AB_Boss;
class UP_PatternCaster;
class AM_BlackBoardManager;

UCLASS(Abstract)
class INSIDEUS_API AB_AICasterBase : public AAIController
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGarbageAllProjectile);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopBehaviorTree);
	
protected:
	/*The pattern caster component. Automatically set.*/
	UPROPERTY(VisibleAnywhere, Category = "AI Caster Base")
	UP_PatternCaster* patternCaster = nullptr;
	
	/*The current Tree used by the AI.*/
	UPROPERTY(EditAnywhere, Category = "AI Caster Base")
	UBehaviorTree* tree = nullptr;

	
	UPROPERTY(VisibleAnywhere, Category = "AI Caster Base")
	AM_BlackBoardManager* blackBoardManager = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "AI Caster Base")
	FName bossKeyName = KEY_BOSS;
	
	UPROPERTY(VisibleAnywhere, Category = "AI Caster Base")
	FName playerKeyName = KEY_PLAYER;

	UPROPERTY(VisibleAnywhere, Category = "AI Caster Base")
	FName isStunKeyName = KEY_STUN;
	
	
	UPROPERTY()
	UWorld* world = nullptr;
	
protected:
	UPROPERTY(BlueprintCallable, BlueprintReadWrite)
	FOnGarbageAllProjectile onGarbageAllProjectile;
	
	UPROPERTY(BlueprintCallable, BlueprintReadWrite)
	FOnStopBehaviorTree onStopBehaviorTree;

public:
	FORCEINLINE FOnGarbageAllProjectile& OnGarbageAllProjectile() { return onGarbageAllProjectile; }
	FORCEINLINE FOnStopBehaviorTree& OnStopBehaviorTree() { return onStopBehaviorTree; }
	FORCEINLINE UP_PatternCaster* GetPatternCaster() const { return patternCaster; }

protected:
	virtual void BeginPlay() override;
	UFUNCTION() virtual void InitAIBossPhase();

public:
	//Call when the AI will change BehaviorTree (Change Arena)
	void SetBehaviorTree(UBehaviorTree* _tree);
	
protected:
	//Initialize BlackBoard
	void InitializeBlackBoard() const;
	//Initiliaze BlackBoard Boss & Player Keys
	void InitializeBlackBoardDefaultKeys() const;

public:
	//Start the AI Controller (Run BehaviorTree)
	UFUNCTION(BlueprintCallable) void ActivateAIBaseController();
	UFUNCTION() void ResetAIBaseController();

	UFUNCTION() void StunBehaviorTree();
	UFUNCTION() void EndStun();
protected:
	UFUNCTION() void DeactivateBehaviorTree();
	UFUNCTION() void DestroyGPEAndProjectile();
};