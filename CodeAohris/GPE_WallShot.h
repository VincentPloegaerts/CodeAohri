#pragma once

#include "CoreMinimal.h"
#include "GPE_Shot.h"
#include "GPE_WallShotBox.h"
#include "GPE_WallShot.generated.h"



UCLASS()
class INSIDEUS_API AGPE_WallShot : public AGPE_Shot
{
	GENERATED_BODY()
	//delay before the pattern is shot when player enters the range
	UPROPERTY(EditAnywhere, Category = "GPE | Shoot|Wall Shoot", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		AGPE_WallShotBox* RangeBoxActor = nullptr;
	virtual bool IsInRange(const FVector& _target, const FVector& _currentLocation, const bool& _useBox) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
};
