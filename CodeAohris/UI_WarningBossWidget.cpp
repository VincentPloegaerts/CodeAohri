#include "UI_WarningBossWidget.h"
#include "B_Boss.h"
#include "IU_Camera.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "STRUCT_CustomMath.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"

void UUI_WarningBossWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!boss || !player)return;
	FVector _camForward = camera->GetActorForwardVector();
	FVector _camRight = camera->GetActorRightVector();
	_camForward.Z = 0.0f;
	_camRight.Z = 0.0f;
	FVector _bossToPlayer =  boss->GetActorLocation() - player->GetActorLocation();
	_bossToPlayer.Z = 0.0f;
	distanceFromPlayer =  _bossToPlayer.Size();
	angle = FCustomMath::AngleBetweenVectors(_bossToPlayer,_camForward) * radToDeg;
	if (angle < 45.0f)
	{
		SetRenderOpacity(0.0f);
		return;
	}
	SetRenderOpacity(1.0f);
	if (FCustomMath::AngleBetweenVectors(_bossToPlayer,_camRight) * radToDeg < 90.0f)
		angle *=-1.0f;
	SetWidgetPosFromAngle(angle);
}

void UUI_WarningBossWidget::Init(AIU_Player* _player, AIU_Camera* _camera, AB_Boss* _boss)
{
	_boss->OnBossDead().AddUniqueDynamic(this,&UUI_WarningBossWidget::BossEnded);
	player = _player;
	camera = _camera;
	boss = _boss;
	thisAsPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	thisAsPanelSlot->SetSize(imageSize);
	maxSizeY -= imageSize.Y;
	maxSizeX -= imageSize.X;
	halfMaxSizeY = maxSizeY/2;
	halfMaxSizeX = maxSizeX/2;
}

void UUI_WarningBossWidget::BossEnded()
{
	RemoveFromParent();
}

void UUI_WarningBossWidget::SetWidgetPosFromAngle(const float& _angle)
{
	const float _angleAbs = FMath::Abs(_angle);
	const float _y = _angle > 135.0f || _angle < -135.0f ? maxSizeY :
					((((_angleAbs - 90.0f) / 45.0f ) * halfMaxSizeY) + halfMaxSizeY);
	const float _x = _angle > 0.0f && _angle < 135.0f ? 0.0f :
					 _angle < 0.0f && _angleAbs < 135.0f ? maxSizeX :
					 _angle > 0.0f && _angle >= 135.0f ? ((_angle - 135.0f) / 45.0f) * halfMaxSizeX :
					 _angle < 0.0f && _angleAbs >= 135.0f ? - (((_angleAbs - 135.0f) / 45.0f) * halfMaxSizeX) + maxSizeX :
					 0.0f;
	UWidgetLayoutLibrary::SlotAsCanvasSlot(this)->SetPosition(FVector2D(_x,_y));
	// const float _newAngle = _angle < 0.0 ? _angle +180.0f : _angle -180.0f;;
	// SetRenderTransformAngle( - _newAngle - (_newAngle/ 90.0f) * 20.0f);
}
