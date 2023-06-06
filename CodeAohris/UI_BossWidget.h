#pragma once

#include "CoreMinimal.h"
#include "UI_Widget.h"

#include "Components/ProgressBar.h"

#include "UI_BossWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_BossWidget : public UUI_Widget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UProgressBar* bossLifePB = nullptr;

public:
	void InitializeBossWidget(class AB_Boss* _boss);

private:
	UFUNCTION() void UpdateBossLifeBar(float _life, float _maxLife);
};
