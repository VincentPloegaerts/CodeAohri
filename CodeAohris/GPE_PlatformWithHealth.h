#pragma once

#include "CoreMinimal.h"
#include "GPE_Platform.h"
#include "IU_HealthSystemComponent.h"
#include "GPE_PlatformWithHealth.generated.h"

UCLASS(Abstract)
class INSIDEUS_API AGPE_PlatformWithHealth : public AGPE_Platform
{
	GENERATED_BODY()
protected:
	//Health Component of this GPE to be shot at
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UIU_HealthSystemComponent* healthSystemComponent = nullptr;
public:	
	FORCEINLINE UIU_HealthSystemComponent* HealthSystemComponent() const {return healthSystemComponent;}
	AGPE_PlatformWithHealth();
};
