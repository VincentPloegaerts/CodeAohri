#pragma once

#include "CoreMinimal.h"
#include "GPE_BaseGPE.h"
#include "GPE_WallShotBox.generated.h"



UCLASS()
class INSIDEUS_API AGPE_WallShotBox : public AGPE_BaseGPE
{
	GENERATED_BODY()
	// Data table for the pattern shot every shootDelay
	UPROPERTY(EditAnywhere,Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FVector boxRange = FVector(200.0f);
	UPROPERTY(EditAnywhere,Category = "GPE | Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool showDebug = false;
public:
	AGPE_WallShotBox();
private:
	virtual bool ShouldTickIfViewportsOnly() const override;
#if WITH_EDITOR
	virtual void Tick(float DeltaSeconds) override;
#endif
public:
		UFUNCTION(BlueprintCallable) bool IsInsideBox(const FVector& _position);
	
};
