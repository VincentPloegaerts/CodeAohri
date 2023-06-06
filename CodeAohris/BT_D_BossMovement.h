#pragma once

#include "CoreMinimal.h"

#include "ENUM_BossMovementMode.h"
#include "STRUCT_PatternMovementData.h"

#include "BehaviorTree/BTDecorator.h"
#include "BT_D_BossMovement.generated.h"

class UB_BossAnim;
class UB_BossMovement;
class AB_Boss;
class UBT_BlackBoard;
class AM_BlackBoardManager;

UCLASS()
class INSIDEUS_API UBT_D_BossMovement : public UBTDecorator
{
	GENERATED_BODY()
	
	friend class AM_BlackBoardManager;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStopMovingAndNotStun, const bool, _isStun);

private:

	UPROPERTY()
	AB_Boss* boss = nullptr;
	
	UPROPERTY()
	UB_BossMovement* bossMovement = nullptr;

	UPROPERTY()
	UB_BossAnim* bossAnim = nullptr;

	UPROPERTY()
	UBlackboardComponent* blackBoard = nullptr;

	UPROPERTY()
	AM_BlackBoardManager* blackBoardManager = nullptr;
	
	
	//Arena Movement Point
	UPROPERTY(EditAnywhere, Category = "Boss Movement")
	bool bIfAlreadyOnPointJustWait = false;
	
	UPROPERTY(EditAnywhere, Category = "Boss Movement")
	bool bIsRandomPoint = false;

	UPROPERTY(EditAnywhere, Category = "Boss Movement",	meta = (GetOptions = "GetMovementPointNames"))
	FString arenaPointName = "";

	UPROPERTY(EditAnywhere, Category = "Boss Movement",	meta = (GetOptions = "GetMovementPointNames"))
	TArray<FString> randomArenaPointName;

	UPROPERTY(EditAnywhere, Category = "Boss Movement", meta = (ClampMin = "0", ClampMax = "999999"))
	float fRandomRange = 0.0f;


	//Player
	UPROPERTY(EditAnywhere, Category = "Boss Movement")
	bool bActivatePlayerDetection = true;
	
	UPROPERTY(EditAnywhere, Category = "Boss Movement")
	bool bOverridePlayerDetection = false;
	
	UPROPERTY(EditAnywhere, Category = "Boss Movement", meta = (ClampMin = "0", ClampMax = "999999"))
	float fPlayerDetectionRange = 250.0f;
	

	//Deprecated
	UPROPERTY()
	bool bShootBeforeMoving = false;

	//Rotation
	UPROPERTY(EditAnywhere, Category = "Boss Movement")
	bool bLookGoalMovement = false;
	
	UPROPERTY(EditAnywhere, Category = "Boss Movement")
	bool bLookShootedPattern = true;
	
	UPROPERTY(EditAnywhere, Category = "Boss Movement")
	bool bLookPlayer = false;

	//Default Value
	UPROPERTY(EditAnywhere, Category = "Boss Movement", meta = (ClampMin = "0", ClampMax = "999999"))
	float fSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Boss Movement")
	TEnumAsByte<EBossMovementMode> bossMovementMode = EBossMovementMode::MOVE_ON_GROUND;


	UPROPERTY()
	FOnStopMovingAndNotStun onStopMovingAndNotStun;

public:
	FORCEINLINE FOnStopMovingAndNotStun& OnStopMovingAndNotStun() { return onStopMovingAndNotStun; }
	
public:
	UBT_D_BossMovement();
	
private:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
	virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;

	UFUNCTION() void Init(bool _isstun);
	
	UFUNCTION() void StartMovement();
	UFUNCTION() void StartShoot();

	UFUNCTION() void SetIsMovingKeyTrue();
	UFUNCTION() void SetIsMovingKeyFalse();

	UFUNCTION() void SetOwnerCasterPattern(const FPatternMovementData& _patternMovementData);

public:
	UFUNCTION() void UnActivate();


#if WITH_EDITOR
	UFUNCTION(CallInEditor) TArray<FString> GetMovementPointNames() const;
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif
};