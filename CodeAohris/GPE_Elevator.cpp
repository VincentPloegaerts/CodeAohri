#include "GPE_Elevator.h"

#include "GPE_ElevatorAnimInstance.h"
#include "HUD_Main.h"
#include "IU_CameraManager.h"
#include "IU_Player.h"
#include "IU_PlayerMovementComponent.h"
#include "JsonObjectConverter.h"
#include "M_GameMode.h"
#include "UI_PlayerWidget.h"

#if WITH_EDITOR
	#include "IU_Macros.h"
#endif

AGPE_Elevator::AGPE_Elevator()
{
	splinePath = CreateDefaultSubobject<USplineComponent>("Spline Path");
	splinePath->SetupAttachment(RootComponent);
	
	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	skeletalMesh->SetupAttachment(staticMesh);
	
	animLocation = CreateDefaultSubobject<USceneComponent>("AnimTPLocation");
	animLocation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AGPE_Elevator::BeginPlay()
{
	Super::BeginPlay();
#if WITH_EDITOR
	if (!staticMesh)
		LOG("No static Mesh in %s",*GetName());
#endif
	stockedSplineLenght = splinePath->GetSplineLength();
	fActualMoveSpeed = fMoveSpeed;
	animInstance = Cast<UGPE_ElevatorAnimInstance>(skeletalMesh->GetAnimInstance());
	animInstance->InitAnimInstance(fCustomPlayRate);
	
	if (AHUD_Main* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>())
		playerWidget = _hud->GetWidget<UUI_PlayerWidget>(WIDGET_PLAYER);
	GetWorld()->GetTimerManager().SetTimer(timerDistance, this, &AGPE_Elevator::CheckPlayerInteractionRange, 0.2f, true, FMath::RandRange(0.1f, 0.3f));
}

void AGPE_Elevator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsMoving)
		MoveMeshAlongSpline(fActualMoveSpeed * DeltaSeconds * (bReverse ? - 1.0f : 1.0f));
	
}

void AGPE_Elevator::SetAnimationState(bool _status) const
{
	if(animInstance)
		animInstance->UpdateIsActivated(_status);
	
}

void AGPE_Elevator::CheckPlayerInteractionRange()
{
	if (!player || !playerWidget) return;
	
	const float _distance = FVector::Distance(player->GetActorLocation(), skeletalMesh->GetComponentLocation());
	if (_distance < player->GetInteractionRange()/* + additionalRange*/)
	{
		if (isWidgetHidden)
		{
			isWidgetHidden = false;
			playerWidget->ShowInteractionWidget();
		}
	}
	else if (!isWidgetHidden)
	{
		isWidgetHidden = true;
		playerWidget->HideInteractionWidget();
	}
}

void AGPE_Elevator::InstantActivateOnLoad()
{
	staticMesh->SetWorldLocation(splinePath->GetLocationAtDistanceAlongSpline(stockedSplineLenght, ESplineCoordinateSpace::World));
	bCanBeActivated = false;
}

void AGPE_Elevator::FailedMiniGame(const float& _speed,const float& _duration)
{
	fActualMoveSpeed = _speed;
	bReverse = !bReverse;
	FTimerHandle _handle;
	GetWorld()->GetTimerManager().SetTimer(_handle,this,&AGPE_Elevator::StopFail,_duration,false);
}

void AGPE_Elevator::StopFail()
{
	fActualMoveSpeed = fMoveSpeed;
	bReverse = !bReverse;
}

void AGPE_Elevator::InitializeGPE(const FString& _jsonStr)
{
	FGPE_ElevatorJson _json;
	FJsonObjectConverter::JsonObjectStringToUStruct(_jsonStr, &_json);
	SetEnableGPE(_json.enable);
	fProgress = _json.fSavedProgress;
	staticMesh->SetWorldLocation(splinePath->GetLocationAtDistanceAlongSpline(fProgress,ESplineCoordinateSpace::World));
	selfSaveJson = _jsonStr;
}

FString AGPE_Elevator::GetGPEJson() const
{
	FGPE_ElevatorJson _json;
	GetGPEBaseJson(_json);
	_json.fSavedProgress = fProgress;
	FString _jsonStr = "";
	FJsonObjectConverter::UStructToJsonObjectString(_json, _jsonStr);
	return _jsonStr;
}

void AGPE_Elevator::MoveMeshAlongSpline(const float& _progressToAdd)
{
	fProgress += _progressToAdd;
	if (fProgress <0.0f || fProgress > stockedSplineLenght)
	{
		onArrived.Broadcast();
		DeactivateElevator();
	}
	fProgress = fProgress > stockedSplineLenght ? stockedSplineLenght :
				fProgress < 0.0f ? 0.0f :
				fProgress;
	staticMesh->SetWorldLocation(splinePath->GetLocationAtDistanceAlongSpline(fProgress,ESplineCoordinateSpace::World));
}

void AGPE_Elevator::ActivateElevator(const bool& _reset)
{
	if (!bCanBeActivated)return;
	SetAnimationState(true);
	player->SetPlayerInputs(false);
	player->MovementComponent()->SetCanSwitchState(false);
	AIU_Camera* _cam = GetWorld()->GetAuthGameMode<AM_GameMode>()->GetCameraManager()->GetCamera();
	_cam->SetCinematicAngles(cameraPitch,cameraYaw);
	_cam->SetUseCinematicAngles(true);
	if (_reset)
	{
		staticMesh->SetWorldLocation(splinePath->GetLocationAtDistanceAlongSpline(0.0f,ESplineCoordinateSpace::World));
		fProgress = 0.0f;
		bReverse = false;
	}
	else if (fProgress <= 0.0f || fProgress >= stockedSplineLenght)
	{
		bReverse = !bReverse;
	}
	onStartMoving.Broadcast();
	bIsMoving = true;
	
	if(playerWidget)
	{
		GetWorld()->GetTimerManager().ClearTimer(timerDistance);
		isWidgetHidden = true;
		playerWidget->HideInteractionWidget();
	}

}

void AGPE_Elevator::DeactivateElevator()
{
	if (!bCanBeActivated)return;
	bIsMoving = false;
	player->SetPlayerInputs(true);
	player->MovementComponent()->SetCanSwitchState(true);
	AIU_Camera* _cam = GetWorld()->GetAuthGameMode<AM_GameMode>()->GetCameraManager()->GetCamera();
	_cam->SetUseCinematicAngles(false);
	SetAnimationState(false);

	GetWorld()->GetTimerManager().SetTimer(timerDistance, this, &AGPE_Elevator::CheckPlayerInteractionRange, 0.2f, true);
}