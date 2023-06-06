#pragma once

#include "CoreMinimal.h"
#include "STRUCT_PatternData.h"
#include "Components/ActorComponent.h"
#include "P_PatternBehavior.generated.h"

class UP_DT_BaseDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UP_PatternBehavior : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBehaviorComplete);

protected:
	UPROPERTY(EditAnywhere, Category = "Behavior | Enable")
	bool bIsEnable = false;
	UPROPERTY()
	AActor* owner = nullptr;

	UPROPERTY()
	FOnBehaviorComplete onBehaviorComplete;

public:
	FORCEINLINE FOnBehaviorComplete& OnBehaviorComplete() { return onBehaviorComplete; }
	
public:	
	UP_PatternBehavior();
	
private:
	virtual void BeginPlay() override;

protected:
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void ActivateBehavior(UP_DT_BaseDataTable* _dataTable);
	virtual void ActivateBehavior(FPatternData& _patternData);
	virtual void DeActivateBehavior();
	/*BehaviorTick*/
	UFUNCTION() virtual void TickBehavior(const float& _deltaTime);
};