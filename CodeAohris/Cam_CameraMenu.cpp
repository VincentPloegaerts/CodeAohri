#include "Cam_CameraMenu.h"

#include "HUD_Main.h"
#include "Kismet/KismetMathLibrary.h"

#include "UI_Widget.h"
#include "IU_Macros.h"
#include "M_GameInstance.h"
#include "S_PlayerSave.h"
#include "UI_ConfirmWidget.h"

ACam_CameraMenu::ACam_CameraMenu()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");	
	
	camera = CreateDefaultSubobject<UCineCameraComponent>("Camera");
	camera->SetupAttachment(RootComponent);

	spline = CreateDefaultSubobject<USplineComponent>("Spline");
	spline->SetupAttachment(RootComponent);
}

void ACam_CameraMenu::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeCameraMenu();
}
void ACam_CameraMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	#if WITH_EDITOR
	if (debug)
	{
		DrawDebugCameraMenu();
		if (GetWorld() && !GetWorld()->IsPlayInEditor())
			return;
	}
	#endif
	
	CameraBehavior(DeltaTime);
}

void ACam_CameraMenu::SetPoint(const ECameraMenuPoint& _point)
{
	if (!IsValidPoint(_point)) return;
	
	previousPoint = currentPoint;
	currentPoint = _point;
	isAtPoint = false;
}

void ACam_CameraMenu::InitCameraMenu(const ECameraMenuPoint& _point)
{
	if (APlayerController* _pc = GetWorld()->GetFirstPlayerController())
		_pc->SetViewTargetWithBlend(this);
	
	SetPoint(_point);
	previousPoint = currentPoint;
}

bool ACam_CameraMenu::IsAtPoint()
{
	if (!IsValidPoint(currentPoint)) return false;
	
	const float _pointTime = spline->GetTimeAtDistanceAlongSpline(spline->GetDistanceAlongSplineAtSplinePoint(points[currentPoint].pointIndex));
	const FVector& _location = spline->GetLocationAtTime(_pointTime, ESplineCoordinateSpace::Local) + offset;
	
	return FVector::Dist(camera->GetRelativeLocation(), _location) < range;
}

void ACam_CameraMenu::InitializeCameraMenu()
{
	if (AHUD_Main* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>())
	{
		if (UUI_Widget* _newGameWidget = _hud->GetWidgetUI(EWidgetEnum::WIDGET_NEW_GAME))
		{
			_newGameWidget->OnShow().AddUniqueDynamic(this, &ACam_CameraMenu::MoveToSave);
		}
		if (UUI_Widget* _loadGameWidget = _hud->GetWidgetUI(EWidgetEnum::WIDGET_LOAD_GAME))
		{
			_loadGameWidget->OnShow().AddUniqueDynamic(this, &ACam_CameraMenu::MoveToSave);
		}
		if (UUI_Widget* _mainMenuWidget = _hud->GetWidgetUI(EWidgetEnum::WIDGET_MAIN_MENU))
		{
			_mainMenuWidget->OnShow().AddUniqueDynamic(this, &ACam_CameraMenu::MoveToTitle);
		}
	}

	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
		if (US_PlayerSave* _playerSave = _gi->GetPlayerSave())
		{
			_playerSave->OnPlayerSaveEdited().AddUniqueDynamic(this, &ACam_CameraMenu::UpdateCamera);
			UpdateCamera(_playerSave);
		}
}

void ACam_CameraMenu::UpdateCamera(const US_PlayerSave* _playerSave)
{
	const FGraphicsConfig& _config = _playerSave->GetGraphicConfig();
	camera->PostProcessSettings.bOverride_MotionBlurAmount = _config.GetMotionBlurEnable();
	camera->PostProcessSettings.MotionBlurAmount = _config.GetMotionBlurValue();
}

#pragma region Behavior
void ACam_CameraMenu::CameraBehavior(const float _deltaTime)
{
	if (!IsValidPoint(currentPoint) || !IsValidPoint(previousPoint)) return;

	MoveToSplinePoint(points[currentPoint].pointIndex, _deltaTime);
	LookAt(points[currentPoint].pointView, _deltaTime);

	UpdateSpeed(points[currentPoint].pointIndex, _deltaTime);
}
void ACam_CameraMenu::MoveToSplinePoint(const int32& _index, const float _deltaTime)
{
	if (!IsValidIndexPoint(_index)) return;
		
	const float _pointTime = spline->GetTimeAtDistanceAlongSpline(spline->GetDistanceAlongSplineAtSplinePoint(_index));
	time = FMath::FInterpConstantTo(time, _pointTime, _deltaTime, currentMoveSpeed);
	const FVector& _location = spline->GetLocationAtTime(time, ESplineCoordinateSpace::Local) + offset;
	
	camera->SetRelativeLocation(_location);

	if (!isAtPoint && IsAtPoint())
	{
		isAtPoint = true;
		onArrivedAtPoint.Broadcast();
	}
}
void ACam_CameraMenu::LookAt(const AActor* _view, const float _deltaTime) const
{
	if (!IsValid(_view)) return;
	
	const FRotator& _rotationTarget = UKismetMathLibrary::FindLookAtRotation(camera->GetComponentLocation(), _view->GetActorLocation());
	const FRotator& _rotation = FMath::RInterpConstantTo(camera->GetComponentRotation(), _rotationTarget, _deltaTime, rotateSpeed);
	
	camera->SetWorldRotation(_rotation);
}
void ACam_CameraMenu::UpdateSpeed(const int32& _index, const float _deltaTime)
{
	currentMoveSpeed = globalMoveSpeed;
	
	const float _targetPointTime = spline->GetTimeAtDistanceAlongSpline(spline->GetDistanceAlongSplineAtSplinePoint(points[currentPoint].pointIndex));
	const float _previousPointTime = spline->GetTimeAtDistanceAlongSpline(spline->GetDistanceAlongSplineAtSplinePoint(points[previousPoint].pointIndex));

	const bool _goBack = _previousPointTime > _targetPointTime;

	float _begin = 1;
	float _end = 1;
	
	if (_goBack)
	{
		//LOG("GO BACK")
		//BEGIN
		{
			const float _min = _previousPointTime - accelerationTime;
			const float _max = _previousPointTime;
		
			const float _beginTimeProgress = ClampTime(_min, _max);
			//LOG("Begin Time	Min	= %f", _min)
			//LOG("Begin Time	Max	= %f", _max)

			_begin = NormalizeValue(_beginTimeProgress, _max, _min);
		}
		//END
		{
			const float _min = _targetPointTime;
			const float _max = _targetPointTime + decelerationTime;
			const float _endTimeProgress = ClampTime(_min, _max);
			//LOG("End Time Min		= %f", _min)
			//LOG("End Time Max		= %f", _max)

			_end = NormalizeValue(_endTimeProgress, _max, _min);
		}
	}
	else
	{
		//LOG("GO FORWARD")
		//BEGIN
		{
			const float _min = _previousPointTime;
			const float _max = _previousPointTime + accelerationTime;
		
			const float _beginTimeProgress = ClampTime(_min, _max);
			//LOG("Begin Time	Min	= %f", _min)
			//LOG("Begin Time	Max	= %f", _max)

			_begin = NormalizeValue(_beginTimeProgress, _min, _max);
		}
		//END
		{
			const float _min = _targetPointTime - decelerationTime;
			const float _max = _targetPointTime;
			const float _endTimeProgress = ClampTime(_min, _max);
			//LOG("End Time Min		= %f", _min)
			//LOG("End Time Max		= %f", _max)

			_end = NormalizeValue(_endTimeProgress, _min, _max);
		}
	}
	
	//LOG("Begin Progress	= %f", _begin)
	//LOG("End Progress	= %f", 1 - _end)
	
	currentMoveSpeed = globalMoveSpeed + (maxAcceleration * _begin) + (maxDeceleration * (1 - _end));
}
#pragma endregion

#pragma region Navigation
void ACam_CameraMenu::MoveToTitle()
{
	SetPoint(ECameraMenuPoint::POINT_TITLE);
}
void ACam_CameraMenu::MoveToSave()
{
	SetPoint(ECameraMenuPoint::POINT_SAVE);
}
#pragma endregion

float ACam_CameraMenu::NormalizeValue(const float _current, const float _min, const float _max) const
{
	return (_current - _min) / (_max - _min);
}
float ACam_CameraMenu::ClampTime(const float _min, const float _max) const
{
	return FMath::Clamp(time, _min, _max);
}

#if WITH_EDITOR
void ACam_CameraMenu::SetDebugPoint()
{
	SetPoint(debugPoint);	
}

void ACam_CameraMenu::DrawDebugCameraMenu()
{
	if (UWorld* _w = GetWorld())
	{
		for (auto _current : points)
		{
			const int32& _index = _current.Value.pointIndex;
			const AActor* _view = _current.Value.pointView;

			const bool _indexValid = spline->GetNumberOfSplinePoints() > _index; 
			const bool _viewValid = IsValid(_view); 
			const FColor& _color = _indexValid && _viewValid ? FColor::Green : FColor::Red;
			
			if (_viewValid)
			{
				FVector _origin;
				FVector _extent;
				_view->GetActorBounds(false, _origin, _extent);
				DrawDebugBox(_w, _view->GetActorLocation(), FVector::OneVector * _extent + FVector(5), _color, false, -1, 0, 1);
			}
			if (_indexValid)
				DrawDebugSphere(_w, spline->GetLocationAtSplinePoint(_index, ESplineCoordinateSpace::World) + offset, 15, 10, _color, false, -1, 0, 1);
			if (_viewValid && _indexValid) 
				DrawDebugLine(_w, spline->GetLocationAtSplinePoint(_index, ESplineCoordinateSpace::World) + offset, _view->GetActorLocation(), _color, false, -1, 0, 1);
		}

		DrawDebugSphere(_w, camera->GetComponentLocation(), 10, 10, FColor::Blue, false, -1, 0, 1);
		DrawDebugDirectionalArrow(_w, camera->GetComponentLocation(), camera->GetComponentLocation() + camera->GetForwardVector() * 50, 10, FColor::Blue, false, -1, 0, 1);
	}
}
#endif