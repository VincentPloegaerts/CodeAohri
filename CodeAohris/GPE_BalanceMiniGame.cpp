#include "GPE_BalanceMiniGame.h"

#include "HUD_Game.h"
#include "IU_CameraManager.h"
#include "IU_Macros.h"
#include "IU_Player.h"
#include "IU_PlayerArrow.h"
#include "IU_PlayerMovementComponent.h"
#include "M_GameInstance.h"
#include "M_GameMode.h"
#include "M_InputManager.h"
#include "UI_ElevatorWidget.h"
#include "Blueprint/UserWidget.h"

UGPE_BalanceMiniGame::UGPE_BalanceMiniGame()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGPE_BalanceMiniGame::BeginPlay()
{
	Super::BeginPlay();
	fActualInputSpeed = vPlayerSpeedRange.X;
	vAreaCurrentSizeRange = vAreaBaseSize;
	
	world = GetWorld();
	FTimerHandle _handle;
	world->GetTimerManager().SetTimer(_handle,this,&UGPE_BalanceMiniGame::Init,1.0f,false);
}

void UGPE_BalanceMiniGame::Init()
{
	hud = world->GetFirstPlayerController()->GetHUD<AHUD_Game>();
	hud->OnKeyboardKey().AddUniqueDynamic(this,&UGPE_BalanceMiniGame::SetUsingKeyBoardTrue);
	hud->OnGamepadKey().AddUniqueDynamic(this,&UGPE_BalanceMiniGame::SetUsingKeyBoardFalse);
	elevatorWidget = hud->GetWidget<UUI_ElevatorWidget>(WIDGET_ELEVATOR);
	const UM_InputManager* _im = world->GetGameInstance<UM_GameInstance>()->GetInputManager();
	mappings = _im->GetGameInput().ElevatorAxis.Mappings;
	inputInfoData = _im->GetInputInfoDataTable();
}

void UGPE_BalanceMiniGame::StartGame()
{
	if (!playerRef)
	{
		playerRef = Cast<AIU_Player>(world->GetFirstPlayerController()->GetPawn());
	}
#if WITH_EDITOR
	if (playerRef)
	{
#endif
		playerRef->InputComponent->BindAxis(gameInputName,this,&UGPE_BalanceMiniGame::GameInputAxis).bConsumeInput = false;
		playerRef->SetPlayerInputs(false);
		AIU_Camera* _cam = GetWorld()->GetAuthGameMode<AM_GameMode>()->GetCameraManager()->GetCamera();
		_cam->SetCinematicAngles(cameraPitch,cameraYaw);
		_cam->SetUseCinematicAngles(true);
		playerRef->MovementComponent()->SetCanSwitchState(false);
		const int& _max =mappings.Num();
		if (!usingKeyBoard)
		{
			for (int i = 0;i < _max;i++)
			{
				if (mappings[i].Key.IsGamepadKey())
					elevatorWidget->SetGamepadKeyImage(mappings[i].Key);
			}
		}
		else
		{
			for (int i = 0;i < _max;i++)
			{
				if (!mappings[i].Key.IsGamepadKey() && mappings[i].Scale < 0)
					elevatorWidget->SetLeftKeyImage(mappings[i].Key);
				else if (!mappings[i].Key.IsGamepadKey() && mappings[i].Scale > 0)
					elevatorWidget->SetRightKeyImage(mappings[i].Key);
			}
		}
		hud->ShowGameWidget(WIDGET_ELEVATOR);
#if WITH_EDITOR
	}
	else
		LOG("No player BalanceMiniGame::Start")
#endif
		
	
	bGameGoingRight = FMath::RandRange(0,1000)%2 == 0;
	fActualGameSpeed = FMath::RandRange(vGameSpeedRange.X,vGameSpeedRange.Y);
	
}

void UGPE_BalanceMiniGame::EndGame()
{
#if WITH_EDITOR
	if (playerRef)
	{
#endif
		playerRef->InputComponent->RemoveAxisBinding(gameInputName);
		playerRef->SetPlayerInputs(true);
		elevatorWidget->HideLeftKeyImage();
		elevatorWidget->HideRightKeyImage();
		elevatorWidget->HideGamepadKeyImage();
		hud->HideGameWidget(WIDGET_ELEVATOR);
		playerRef->MovementComponent()->SetCanSwitchState(true);
		AIU_Camera* _cam = GetWorld()->GetAuthGameMode<AM_GameMode>()->GetCameraManager()->GetCamera();
		_cam->SetUseCinematicAngles(false);
#if WITH_EDITOR
	}
#endif
}

void UGPE_BalanceMiniGame::GameInputAxis(float _value)
{
	if (!bIsStarted && _value  !=0.0f)
	{
		checkInputQuantity = 1.0f;
		ChangeSpeed();
	}
	if (bFailed)return;
	const float _deltaTime = world->DeltaTimeSeconds;
	if (_value != 0)
		checkInputQuantity += _deltaTime / vGameChangeSpeedRange.X;
	if ((_value > 0 && bPlayerGoingRight) || (_value <0 && !bPlayerGoingRight))
	{
		fActualInputSpeed += fInputAcceleration * _deltaTime * fActualInputSpeed;
		fActualInputSpeed = fActualInputSpeed > vPlayerSpeedRange.Y ? vPlayerSpeedRange.Y : fActualInputSpeed;
	}
	else
	{
		fActualInputSpeed = vPlayerSpeedRange.X;
		bPlayerGoingRight = _value >0;
	}
	
	fCursorPosition += fActualInputSpeed * _deltaTime * _value +
						fActualGameSpeed * _deltaTime * (bGameGoingRight ? 1.0f : -1.0f);
	if (fCursorPosition < vAreaCurrentSizeRange.X || fCursorPosition > 100.0f - vAreaCurrentSizeRange.Y)
	{
		Fail();
	}
}

void UGPE_BalanceMiniGame::Fail()
{
	vAreaCurrentSizeRange.X -= fAreasRegression;
	vAreaCurrentSizeRange.X = vAreaCurrentSizeRange.X < vAreaMinimumSize.X ? vAreaMinimumSize.X : vAreaCurrentSizeRange.X;
	
	vAreaCurrentSizeRange.Y -= fAreasRegression;
	vAreaCurrentSizeRange.Y = vAreaCurrentSizeRange.Y < vAreaMinimumSize.X ? vAreaMinimumSize.Y : vAreaCurrentSizeRange.Y;
	fCursorPosition = 50.0f;
	ChangeFailedStatus();
	if (fDurationFail >0.0f)
	{
		FTimerHandle _handle;
		world->GetTimerManager().SetTimer(_handle,this,&UGPE_BalanceMiniGame::ChangeFailedStatus,  fDurationFail,false);
	}
	onFail.Broadcast(fSpeedFail,fDurationFail);
}

void UGPE_BalanceMiniGame::ChangeSpeed()
{
	if (checkInputQuantity < 0.5f)
	{
		world->GetTimerManager().SetTimer(handle,this,&UGPE_BalanceMiniGame::ChangeSpeed,FMath::RandRange(vGameChangeSpeedRange.X,vGameChangeSpeedRange.Y),false);
		return;
	}
	bIsStarted = true;
	fActualGameSpeed = FMath::RandRange(vGameSpeedRange.X,vGameSpeedRange.Y);
	bGameGoingRight = !bGameGoingRight;
	fActualInputSpeed = vPlayerSpeedRange.X;
	world->GetTimerManager().SetTimer(handle,this,&UGPE_BalanceMiniGame::ChangeSpeed,FMath::RandRange(vGameChangeSpeedRange.X,vGameChangeSpeedRange.Y),false);
	checkInputQuantity = 0.0f;
}