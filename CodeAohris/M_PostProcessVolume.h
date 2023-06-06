#pragma once

#include "CoreMinimal.h"
#include "Engine/PostProcessVolume.h"
#include "M_PostProcessVolume.generated.h"

class US_PlayerSave;

UCLASS()
class INSIDEUS_API AM_PostProcessVolume : public APostProcessVolume
{
	GENERATED_BODY()

	UPROPERTY()
	US_PlayerSave* playerSave = nullptr;
	
public:
	AM_PostProcessVolume();
	
private:
	virtual void BeginPlay() override;

	void InitializePostProcess();
	
	UFUNCTION() void UpdateExposure(float _value);
};
