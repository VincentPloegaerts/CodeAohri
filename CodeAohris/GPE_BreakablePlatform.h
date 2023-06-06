#pragma once

#include "CoreMinimal.h"
#include "GPE_Platform.h"
#include "GPE_BreakablePlatform.generated.h"



UCLASS()
class INSIDEUS_API AGPE_BreakablePlatform : public AGPE_Platform
{
	GENERATED_BODY()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlatformStateChange);
#pragma region Properties
#pragma region GPE Values
	//time before the platform collides when something enter the detection box
	UPROPERTY(EditAnywhere,Category = "GPE|Breakable Platform", meta = (ClampMin = "0.0", ClampMax = "10000.0"),BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float timeBeforeDestruction = 1.0f;
	//time before the reconstruction of the platform after its destruction
	UPROPERTY(EditAnywhere,Category = "GPE|Breakable Platform",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		bool shouldReconstruct  = true;
	//time before the reconstruction of the platform after its destruction
	UPROPERTY(EditAnywhere,Category = "GPE|Breakable Platform", meta = (ClampMin = "0.0", ClampMax = "10000.0"),BlueprintReadWrite,meta = (AllowPrivateAccess = True)
				, meta = (EditCondition = "shouldReconstruct", EditConditionHides))
		float timeBeforeReconstruction = 2.0f;
	//FTimerHandle used for both timer of reconstruction and destruction
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FTimerHandle timerHandle;

#pragma endregion
#pragma region Events
	UPROPERTY(EditAnywhere,BlueprintAssignable,BlueprintReadWrite,Category = "Events",meta = (AllowPrivateAccess = True))
		FOnPlatformStateChange onPlatformBreaks;
	UPROPERTY(EditAnywhere,BlueprintAssignable,BlueprintReadWrite,Category = "Events",meta = (AllowPrivateAccess = True))
		FOnPlatformStateChange onPlatformReconstruct;
#pragma endregion 
#pragma region Components
	//box used to trigger the destruction of the platform
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UStaticMeshComponent* meshCollider = nullptr;
#pragma endregion 
#pragma endregion 
public:
	AGPE_BreakablePlatform();
public:
	FORCEINLINE FOnPlatformStateChange& OnPlatformBreaks() {return onPlatformBreaks;}
	FORCEINLINE FOnPlatformStateChange& OnPlatformReconstruct() {return onPlatformReconstruct;}
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable) void HitPlatform(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);
	UFUNCTION(BlueprintCallable) void DestroyPlatform();
	UFUNCTION(BlueprintCallable) void ReformPlatform();
	UFUNCTION(BlueprintCallable) void Init();
	
};
