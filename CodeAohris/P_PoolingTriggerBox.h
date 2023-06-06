#pragma once

#include "CoreMinimal.h"
#include "P_PoolingTrigger.h"
#include "Components/BoxComponent.h"
#include "P_PoolingTriggerBox.generated.h"

UCLASS()
class INSIDEUS_API AP_PoolingTriggerBox : public AP_PoolingTrigger
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* boxComponent = nullptr;

public:
	AP_PoolingTriggerBox();
	
	virtual void EnablePoolingTrigger() override;
	virtual void DisablePoolingTrigger() override;

#if WITH_EDITOR
	virtual void DrawPoolingTriggerDebug() override;
#endif
};
