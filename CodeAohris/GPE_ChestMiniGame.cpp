#include "GPE_ChestMiniGame.h"
#include "IU_HealthSystemComponent.h"
#if WITH_EDITOR
#include "IU_Macros.h"
#endif
#include "GPE_Chest.h"
#include "GPE_ChestAnimInstance.h"
#include "IU_CameraManager.h"
#include "IU_Player.h"
#include "M_GameMode.h"
#include "STRUCT_GameInput.h"
#include "UC_PlayerAttackComponent.h"
#include "UI_ChestGameQTEWidget.h"
#include "API_SteamAchievementsManager.h"
#include "M_GameInstance.h"
#include "M_InputManager.h"
#include "GameFramework/InputSettings.h"
#include "S_GameSave.h"

UGPE_ChestMiniGame::UGPE_ChestMiniGame()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGPE_ChestMiniGame::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle _handle;
	GetWorld()->GetTimerManager().SetTimer(_handle,this,&UGPE_ChestMiniGame::Init,0.05f,false);
}

void UGPE_ChestMiniGame::Init()
{
	const APlayerController* _playerController = GetWorld()->GetFirstPlayerController();
	if (UWorld* _world = GetWorld())
	{
		if (AM_GameMode* _gm = _world->GetAuthGameMode<AM_GameMode>())
		{
			playerRef = _gm->GetPlayer();
		}
	}
	if (!_playerController)return;
	inputComponentRef = _playerController->InputComponent;

	if (UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		inputManager = _gi->GetInputManager();
		if (inputManager)
		{
			inputManager->OnInputAxisRebind().AddUniqueDynamic(this, &UGPE_ChestMiniGame::OnAxisRebind);
			inputManager->OnInputReset().AddUniqueDynamic(this, &UGPE_ChestMiniGame::OnInputReset);
		}
		InitKeys();
	}
	
	hud = _playerController->GetHUD<AHUD_Game>();
	if (hud)
	{
		hud->OnKeyboardKey().AddDynamic(this,&UGPE_ChestMiniGame::SetUsingKeyBoardTrue);
		hud->OnGamepadKey().AddDynamic(this,&UGPE_ChestMiniGame::SetUsingKeyBoardFalse);
		mashWidget = hud->GetWidget<UUI_ChestGameMashWidget>(WIDGET_CHEST_SPAM);
		QTEWidget = hud->GetWidget<UUI_ChestGameQTEWidget>(WIDGET_CHEST_QTE);
	}


}

void UGPE_ChestMiniGame::InitKeys()
{
	if (inputManager)
	{
		const FGameInput& _gameInput = inputManager->GetGameInput();

		gamepadQTEKeys.Empty();
		const FInputActionData& _gamepadQTE = _gameInput.QTEGamepadAction;
		for (const auto& _mapping : _gamepadQTE.Mappings)
			gamepadQTEKeys.Add(_mapping.Key);
			
		keyboardQTEKeys.Empty();
		const FInputActionData& _keyboardQTE = _gameInput.QTEKeyboardAction;
		for (const auto& _mapping : _keyboardQTE.Mappings)
			keyboardQTEKeys.Add(_mapping.Key);
	}
}

void UGPE_ChestMiniGame::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	countedInputDuringFrame = false;
}

void UGPE_ChestMiniGame::StartMiniGame()
{
	playerRef->SetPlayerInputs(false);
	playerRef->ShowArrow();
	playerRef->GetCharacterAttackComponent()->SetIsOpeningChest(true);
	const FGameInput& _gameInput = FGameInput();
	inputComponentRef->BindAction(_gameInput.MashChestAction.ActionName,IE_Pressed,this,&UGPE_ChestMiniGame::Mashing);
	hud->ShowGameWidget(WIDGET_CHEST_SPAM);
	mashWidget->SetProgressMash(0);
	mashWidget->SetTime(timeToMash);
	AIU_Camera* _cam = GetWorld()->GetAuthGameMode<AM_GameMode>()->GetCameraManager()->GetCamera();
	_cam->SetCinematicAngles(cameraPitch,cameraYaw);
	_cam->SetUseCinematicAngles(true);
	GetWorld()->GetTimerManager().SetTimer(handle,this,&UGPE_ChestMiniGame::TimeExpired,timeToMash,false);
}

void UGPE_ChestMiniGame::Mashing()
{
	if (countedInputDuringFrame)
		return;
	inputCount++;
	
	playerRef->GetPlayerAnimInstance()->UpdateIsInChestFirstPhase(true);
	
	
	countedInputDuringFrame = true;
	mashWidget->SetProgressMash(static_cast<float>(inputCount)/inputQuantityNeeded);
	
	if (inputCount==inputQuantityNeeded)
	{
		const FGameInput& _gameInput = FGameInput();
		inputComponentRef->RemoveActionBinding(_gameInput.MashChestAction.ActionName,IE_Pressed);
		
		hud->HideGameWidget(WIDGET_CHEST_SPAM);
		
			playerRef->GetPlayerAnimInstance()->UpdateIsInChestFirstPhase(false);
		
		
		StartQTE();
		inputCount = 0;
	}
}

void UGPE_ChestMiniGame::StartQTE()
{
#if WITH_EDITOR
	if (gamepadQTEKeys.Num() <1 && keyboardQTEKeys.Num() < 1)
	{
		LOG("Chest Minigame missing inputs in InputManager");
		return;
	}
#endif
	GetWorld()->GetTimerManager().ClearTimer(handle);
	GetWorld()->GetTimerManager().SetTimer(handle,this,&UGPE_ChestMiniGame::TimeExpired,timeForQTE,false);
	playerRef->HideArrow();

	// if(chestOwner)
	// 	playerRef->SetActorRotation(chestOwner->GetAnimTPLocation()->GetComponentRotation());
	
	const FGameInput& _gameInput = FGameInput();
	inputComponentRef->BindAction(_gameInput.QTEKeyboardAction.ActionName, IE_Pressed,this,&UGPE_ChestMiniGame::PressedGameKeyKeyBoard).bConsumeInput = false;
	inputComponentRef->BindAction(_gameInput.QTEGamepadAction.ActionName,IE_Pressed,this,&UGPE_ChestMiniGame::PressedGameKeyGamePad).bConsumeInput = false;
	
	playerRef->GetPlayerAnimInstance()->UpdateIsInChestSecondPhase(true);
	
	
	int _rand = -1;
	FKey _key = FKey();
	for (int i = 0;i<inputCountQTE;i++)
	{
		_rand = usingKeyBoard ? FMath::RandRange(0,keyboardQTEKeys.Num() -1) : FMath::RandRange(0,gamepadQTEKeys.Num() -1);
		_key = usingKeyBoard ? keyboardQTEKeys[_rand] : gamepadQTEKeys[_rand];

		if (i >1)
		{
			if (keyArrayQTE[i-1] == _key && keyArrayQTE[i-1] == keyArrayQTE[i-2])	//manipulate random a bit so it doesn't do the same input 3 times in a row
			{
				i--;
				continue;
			}
		}
		
		keyArrayQTE.Add(_key);
	}
	
	QTEWidget->FillHorizontalBox(keyArrayQTE);
	hud->ShowGameWidget(WIDGET_CHEST_QTE);
	QTEWidget->SetTime(timeForQTE);
	QTEWidget->UpdateCurrentQTEKey(currentAskedInput);

#if WITH_EDITOR
	for (int i= 0;i<keyArrayQTE.Num();i++)
	{
		LOG("%s",*keyArrayQTE[i].ToString());
	}
#endif	
}

void UGPE_ChestMiniGame::PressedGameKeyKeyBoard(FKey _key)
{
	if (!usingKeyBoard)return;
	if (keyArrayQTE[currentAskedInput] == _key)
	{
		SuccessQTE();
	}
	else
		FailQTE();
}

void UGPE_ChestMiniGame::PressedGameKeyGamePad(FKey _key)
{
	if (usingKeyBoard)return;
	if (keyArrayQTE[currentAskedInput] == _key)
	{
		SuccessQTE();		
	}
	else
		FailQTE();
}

void UGPE_ChestMiniGame::SuccessQTE()
{
	currentAskedInput++;
	QTEWidget->UpdateCurrentQTEKey(currentAskedInput);
	
	if (currentAskedInput == keyArrayQTE.Num())
	{
		hud->HideGameWidget(WIDGET_CHEST_QTE);
		QTEWidget->EmptyBox();
		
			playerRef->GetPlayerAnimInstance()->UpdateIsInChestSecondPhase(false);
	
		onGameEnded.Broadcast(true);

		UWorld* _world = GEngine->GameViewport->GetWorld();


		ExitGPE();		
	}
}

void UGPE_ChestMiniGame::FailQTE()
{
	hud->HideGameWidget(WIDGET_CHEST_QTE);
	QTEWidget->EmptyBox();
	playerRef->HealthComponent()->TakeDamage(failDamage);

	playerRef->GetPlayerAnimInstance()->UpdateIsInChestSecondPhase(false);

	onGameEnded.Broadcast(false);
	ExitGPE();
}

void UGPE_ChestMiniGame::ExitGPE()
{	
	//playerRef->SetPlayerInputs(true);
	AIU_Camera* _cam = GetWorld()->GetAuthGameMode<AM_GameMode>()->GetCameraManager()->GetCamera();
	_cam->SetUseCinematicAngles(false);
	currentAskedInput = 0;

	const FGameInput& _gameInput = FGameInput();
	inputComponentRef->RemoveActionBinding(_gameInput.QTEKeyboardAction.ActionName,IE_Pressed);
	inputComponentRef->RemoveActionBinding(_gameInput.QTEGamepadAction.ActionName,IE_Pressed);
	keyArrayQTE.Empty();
	GetWorld()->GetTimerManager().ClearTimer(handle);

		playerRef->GetPlayerAnimInstance()->UpdateIsOpeningChest(false);
		playerRef->GetPlayerAnimInstance()->UpdateIsInChestSecondPhase(false);
		playerRef->HideArrow();
		playerRef->ShowBow();
		playerRef->GetCharacterAttackComponent()->SetIsOpeningChest(false);
}

void UGPE_ChestMiniGame::TimeExpired()
{
	const FGameInput& _gameInput = FGameInput();
	inputComponentRef->RemoveActionBinding(_gameInput.MashChestAction.ActionName,IE_Pressed);
	inputCount = 0; 
	hud->HideGameWidget(WIDGET_CHEST_SPAM);
	hud->HideGameWidget(WIDGET_CHEST_QTE);
	QTEWidget->EmptyBox();
	playerRef->HealthComponent()->TakeDamage(failDamage);
	onGameEnded.Broadcast(false);
	ExitGPE();	
}

void UGPE_ChestMiniGame::OnInputReset(UM_InputManager* _inputManager)
{
	InitKeys();
}
void UGPE_ChestMiniGame::OnAxisRebind(const FInputAxisData& _input, FInputAxisKeyMapping _oldMapping, FInputAxisKeyMapping _newMapping)
{
	if (inputManager)
	{
		const FGameInput& _gameInput = inputManager->GetGameInput();
		if (_input.AxisName == _gameInput.VerticalMovementAxis.AxisName || _input.AxisName == _gameInput.HorizontalMovementAxis.AxisName)
		{
			inputManager->ForceRebindAction(_gameInput.QTEKeyboardAction.ActionName, _oldMapping.Key, _newMapping.Key);
		}
		InitKeys();
	}
}
