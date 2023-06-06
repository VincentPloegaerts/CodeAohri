#pragma once

#include "CoreMinimal.h"
#include "B_BossCritPoint.h"
#include "Components/BoxComponent.h"
#include "B_BossCritPointBox.generated.h"

UCLASS()
class INSIDEUS_API AB_BossCritPointBox : public AB_BossCritPoint
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* boxCollider = nullptr;

public:
	AB_BossCritPointBox();
	
private:
	virtual void GetCritPointSettings(FCritPointSettings& _critPointSettings) const override;
	virtual void SetCritPointSettings(const FCritPointSettings& _critPointSettings) override;

	virtual void DrawCritPointDebug() override;
};
