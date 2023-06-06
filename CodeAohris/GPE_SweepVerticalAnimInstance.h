#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GPE_SweepVerticalAnimInstance.generated.h"


UCLASS()
class INSIDEUS_API UGPE_SweepVerticalAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSweepAnimEnd);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
		float fSweepPlayRate = 1.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
		float fStopReversePlayRate = 1.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
		float fPauseDuration = 1.0f;

public:
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetPlayRateSweep(const float& _rate) {fSweepPlayRate = _rate;}
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetPlayRateStopReverse(const float& _rate) {fStopReversePlayRate = _rate;}
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetPauseDuration(const float& _duration) {fPauseDuration = _duration;}
};
