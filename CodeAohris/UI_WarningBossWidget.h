#pragma once
#include "CoreMinimal.h"
#include "UI_Widget.h"
#include "UI_WarningBossWidget.generated.h"

class AIU_Camera;
class AIU_Player;
class AB_Boss;
class UCanvasPanelSlot;


UCLASS()
class INSIDEUS_API UUI_WarningBossWidget : public UUI_Widget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
		FVector2D imageSize = FVector2D();	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float angle = 0.0f;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float distanceFromPlayer = 0.0f;
	UPROPERTY()
		AIU_Player* player;
	UPROPERTY()
		AIU_Camera* camera;
	UPROPERTY()
		AB_Boss* boss;
	UPROPERTY()
		float radToDeg = (180.f / UE_PI);
	UPROPERTY()
		UCanvasPanelSlot* thisAsPanelSlot = nullptr;
	UPROPERTY()
		float maxSizeY = 1080.0f;
	UPROPERTY()
		float halfMaxSizeY = 1080.0f;
	UPROPERTY()
		float maxSizeX = 1920.0f;
	UPROPERTY()
		float halfMaxSizeX = 1080.0f;
public:
	UFUNCTION(BlueprintCallable) void Init(AIU_Player* _player,AIU_Camera* _camera,AB_Boss* _boss);
private:
	UFUNCTION(BlueprintCallable) void BossEnded();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION(BlueprintCallable) void SetWidgetPosFromAngle(const float& _angle);
};
