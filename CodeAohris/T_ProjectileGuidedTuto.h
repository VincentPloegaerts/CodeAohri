#pragma once
#include "CoreMinimal.h"
#include "P_ProjectileGuided.h"
#include "T_ProjectileGuidedTuto.generated.h"



UCLASS()
class INSIDEUS_API AT_ProjectileGuidedTuto : public AP_ProjectileGuided
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachedDistance);

	UPROPERTY(BlueprintAssignable,BlueprintReadWrite,EditAnywhere,meta = (AllowPrivateAccess = True))
		FOnReachedDistance onReachedDistance;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True),Category = "Projectile")
		float minDistanceSlowMow = 200.0f;
public:
	FORCEINLINE FOnReachedDistance& OnReachedDistance() {return  onReachedDistance;}
	UFUNCTION(BlueprintCallable) bool IsAtDistanceSlowMow();
private:
	virtual void MovementBehavior(const float& _deltaTime) override;
};
