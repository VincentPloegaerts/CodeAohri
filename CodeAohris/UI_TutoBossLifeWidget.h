
#pragma once

#include "CoreMinimal.h"
#include "UI_TutoElementWidget.h"

#include "Components/ProgressBar.h"

#include "UI_TutoBossLifeWidget.generated.h"

class UIU_HealthSystemComponent;

UCLASS()
class INSIDEUS_API UUI_TutoBossLifeWidget : public UUI_TutoElementWidget
{
	GENERATED_BODY()

protected:
		UPROPERTY(meta = (Bindwidget))
		UProgressBar* bossLife = nullptr;

public:
	void InitializeTutoBossLifeWidget(UIU_HealthSystemComponent* _health);

private:

	UFUNCTION() void UpdateLifeBar(float _life, float _max);
};
