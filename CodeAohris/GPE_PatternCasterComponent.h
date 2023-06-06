#pragma once
#include "CoreMinimal.h"
#include "P_DT_PatternDataTable.h"
#include "Components/ActorComponent.h"
#include "GPE_PatternCasterComponent.generated.h"

class AGPE_Shot;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UGPE_PatternCasterComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileClear);

	//Suppose time to wait for all projectile to be pooled. Use for PoolingTrigger Disable function
	UPROPERTY(EditAnywhere, Category = "Pattern Cast")
		float fWaitProjectileClear = 15.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
		AP_PatternBase* currentPattern = nullptr;

	UPROPERTY()
		FTimerHandle timerClear = FTimerHandle();

	UPROPERTY()
		FOnProjectileClear onProjectileClear = FOnProjectileClear();
	
	UPROPERTY()
		UWorld* world = nullptr;

	UPROPERTY()
		AGPE_Shot* owner = nullptr;
		
	UPROPERTY()
		AM_PatternsTickManager* patternsTickManager = nullptr;
	UPROPERTY()
		AM_FeedbackManager* feedbackManager = nullptr;

public:
	FORCEINLINE AP_PatternBase* CurrentPattern() const { return currentPattern; }
	FORCEINLINE FOnProjectileClear& OnProjectileClear() { return onProjectileClear; }
	
public:	
	UGPE_PatternCasterComponent();
	
private:
	virtual void BeginPlay() override;
		
	UFUNCTION(BlueprintCallable) void Init();

	//Simple event call when the pattern is ended. Use by GPE_Shot to disable his PoolingTrigger (abort if a new Pattern is active)
	UFUNCTION()  void ProjectileClear();

public:
	//Cast the current given pattern with spawn settings.
	UFUNCTION(BlueprintCallable) void CastPattern(UP_DT_BaseDataTable* _dataTable);
	UFUNCTION(BlueprintCallable) void PatternBehaviorCompleted(AP_PatternBase* _pattern);
};