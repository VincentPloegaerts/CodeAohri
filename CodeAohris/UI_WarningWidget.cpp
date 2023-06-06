#include "UI_WarningWidget.h"
#include "IU_Macros.h"
#include "GPE_Pattern.h"
#include "IU_Camera.h"
#include "IU_Player.h"
#include "STRUCT_CustomMath.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"

void UUI_WarningWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!pattern || !player)return;
	FVector _camForward = camera->GetActorForwardVector();
	FVector _camRight = camera->GetActorRightVector();
	_camForward.Z = 0.0f;
	_camRight.Z = 0.0f;
	FVector _gpeToPlayer =  pattern->GetActorLocation() - player->GetActorLocation();
	_gpeToPlayer.Z = 0.0f;
	distanceFromPlayer =  _gpeToPlayer.Size();
	// if (_size > 300.0f && _size < 1000.0f)
	// {
		angle = FCustomMath::AngleBetweenVectors(_gpeToPlayer,_camForward) * radToDeg;
		if (angle < 90.0f)
		{
			SetRenderOpacity(0.0f);
			return;
		}
		SetRenderOpacity(1.0f);
		if (FCustomMath::AngleBetweenVectors(_gpeToPlayer,_camRight) * radToDeg < 90.0f)
			angle *=-1.0f;
		SetWidgetPosFromAngle(angle);
	//}
}

void UUI_WarningWidget::SetWidgetPosFromAngle(const float& _angle)
{
	const float _angleAbs = FMath::Abs(_angle);
	const float _y = _angle > 135.0f || _angle < -135.0f ? maxSizeY : ((((_angleAbs - 90.0f) / 45.0f ) * halfMaxSizeY) + halfMaxSizeY);
	const float _x = _angle > 0.0f && _angle < 135.0f ? 0.0f :
					 _angle < 0.0f && _angleAbs < 135.0f ? maxSizeX :
					 _angle > 0.0f && _angle >= 135.0f ? ((_angle - 135.0f) / 45.0f) * halfMaxSizeX :
					 _angle < 0.0f && _angleAbs >= 135.0f ? - (((_angleAbs - 135.0f) / 45.0f) * halfMaxSizeX) + maxSizeX :
					 0.0f;
	
	UWidgetLayoutLibrary::SlotAsCanvasSlot(this)->SetPosition(FVector2D(_x,_y));
	const float _newAngle = _angle < 0.0 ? _angle +180.0f : _angle -180.0f;;
	SetRenderTransformAngle( - _newAngle - (_newAngle/ 90.0f) * 20.0f);
}

void UUI_WarningWidget::PatternEnded(AGPE_Pattern* _pattern)
{
	RemoveFromParent();
}

void UUI_WarningWidget::Init(AIU_Player* _player, AIU_Camera* _camera, AGPE_Pattern* _pattern)
{
	_pattern->OnCompleteBehavior().AddUniqueDynamic(this,&UUI_WarningWidget::PatternEnded);
	player = _player;
	camera = _camera;
	pattern = _pattern;
	thisAsPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	thisAsPanelSlot->SetSize(imageSize);
	maxSizeY -= thisAsPanelSlot->GetSize().Y;
	halfMaxSizeY = maxSizeY/2;
	halfMaxSizeX = maxSizeX/2;
}
