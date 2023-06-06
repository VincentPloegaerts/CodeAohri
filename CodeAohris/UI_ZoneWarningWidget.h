#pragma once
#include "CoreMinimal.h"
#include "UI_Widget.h"
#include "Components/CanvasPanel.h"
#include "UI_WarningWidget.h"
#include "UI_WarningBossWidget.h"
#include "UI_ZoneWarningWidget.generated.h"

class AIU_Camera;
class AIU_Player;
class AGPE_Pattern;

UCLASS()
class INSIDEUS_API UUI_ZoneWarningWidget : public UUI_Widget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,meta = (BindWidget))
		UCanvasPanel* canvas = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUI_WarningWidget> warningWidgetType;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUI_WarningBossWidget> warningBossWidgetType;
	UPROPERTY()
		AIU_Player* player;
	UPROPERTY()
		AIU_Camera* cam;	
public:
	UFUNCTION(BlueprintCallable) void Init(AIU_Player* _player,AIU_Camera* _cam);
	UFUNCTION(BlueprintCallable) void AddWarning(AGPE_Pattern* _gpePattern);
	UFUNCTION(BlueprintCallable) void AddBossWarning(AB_Boss* _boss);
};
