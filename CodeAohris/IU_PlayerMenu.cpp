#include "IU_PlayerMenu.h"

#include "STRUCT_GameInput.h"

#include "HUD_Main.h"
#include "UI_Widget.h"
#include "UI_SaveMenuWidget.h"

#include "Cam_CameraMenu.h"

#include "S_SaveActor.h"

#include "Kismet/KismetSystemLibrary.h"

#include "IU_Macros.h"

AIU_PlayerMenu::AIU_PlayerMenu()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");
}

void AIU_PlayerMenu::BeginPlay()
{
	Super::BeginPlay();

	TimerManager = &GetWorld()->GetTimerManager();

	FTimerHandle _initTimer;
	TimerManager->SetTimer(_initTimer, this, &AIU_PlayerMenu::InitializePlayerMenu, 0.25f, false);
}

void AIU_PlayerMenu::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const FGameInput& _gameInput = FGameInput();
			
	FInputActionBinding& _menuLeftBinding = PlayerInputComponent->BindAction(_gameInput.MenuSavePreviousAction.ActionName, IE_Pressed, this, &AIU_PlayerMenu::NextSave);
	_menuLeftBinding.bConsumeInput = false;
	FInputActionBinding& _menuRightBinding = PlayerInputComponent->BindAction(_gameInput.MenuSaveNextAction.ActionName, IE_Pressed, this, &AIU_PlayerMenu::PreviousSave);
	_menuRightBinding.bConsumeInput = false;
	FInputActionBinding& _menuSelect = PlayerInputComponent->BindAction(_gameInput.MenuSaveSelectAction.ActionName, IE_Pressed, this, &AIU_PlayerMenu::SelectSave);
	_menuSelect.bConsumeInput = false;
	FInputActionBinding& _menuDelete = PlayerInputComponent->BindAction(_gameInput.MenuSaveDeleteAction.ActionName, IE_Pressed, this, &AIU_PlayerMenu::DeleteHoveredSave);
	_menuDelete.bConsumeInput = false;
}

void AIU_PlayerMenu::InitializePlayerMenu()
{
	playerController = GetWorld()->GetFirstPlayerController();
	if (!playerController) return;
	
	hud = playerController->GetHUD<AHUD_Main>();
	if (!IsPlayerMenuValid()) return;
	hud->OnKeyboardKey().AddUniqueDynamic(this, &AIU_PlayerMenu::EnableCursorTrace);
	hud->OnGamepadKey().AddUniqueDynamic(this, &AIU_PlayerMenu::DisableCursorTrace);
	
	playerController->Possess(this);
	camera->InitCameraMenu(ECameraMenuPoint::POINT_TITLE);

	if (UUI_Widget* _newGameWidget = hud->GetWidgetUI(EWidgetEnum::WIDGET_NEW_GAME))
	{
		_newGameWidget->OnShow().AddUniqueDynamic(this, &AIU_PlayerMenu::BeginDelayedSaveSelection);
		_newGameWidget->OnHide().AddUniqueDynamic(this, &AIU_PlayerMenu::TerminateSaveSelection);
	}
	if (UUI_Widget* _loadGameWidget = hud->GetWidgetUI(EWidgetEnum::WIDGET_LOAD_GAME))
	{
		_loadGameWidget->OnShow().AddUniqueDynamic(this, &AIU_PlayerMenu::BeginDelayedSaveSelection);
		_loadGameWidget->OnHide().AddUniqueDynamic(this, &AIU_PlayerMenu::TerminateSaveSelection);
	}
	
	const int _max = saveActors.Num();
	for (int i = 0; i < _max; i++)
	{
		if (AS_SaveActor* _saveActor = saveActors[i])
			_saveActor->InitializeSaveActor(i);
	}
}

void AIU_PlayerMenu::EnableCursorTrace()
{
	if (!saveSelection || hud->IsGamepadVisual() || TimerManager->TimerExists(checkTimer)) return;
	TimerManager->SetTimer(checkTimer, this, &AIU_PlayerMenu::CheckCursor, checkRate, true);
}
void AIU_PlayerMenu::DisableCursorTrace()
{
	if (TimerManager->TimerExists(checkTimer))
		TimerManager->ClearTimer(checkTimer);
}

void AIU_PlayerMenu::BeginDelayedSaveSelection()
{
	if (!camera) return;

	if (camera->IsAtPoint())
		InitializeSaveSelection();
	else
		camera->OnArrivedAtPoint().AddUniqueDynamic(this, &AIU_PlayerMenu::InitializeSaveSelection);
}

void AIU_PlayerMenu::InitializeSaveSelection()
{
	if (!hud || !camera) return;

	camera->OnArrivedAtPoint().RemoveDynamic(this, &AIU_PlayerMenu::InitializeSaveSelection);
	
	saveMenuWidget = Cast<UUI_SaveMenuWidget>(hud->GetCurrentMenuWidget());
	saveSelection = true;
	EnableCursorTrace();
	
	saveIndex = 0;
	if (saveActors.IsEmpty())
	{
		saveIndex = -1;
		return;
	}

	for (AS_SaveActor* _current : saveActors)
		_current->UnhoverSave();
	
	saveActors[saveIndex]->HoverSave();
}

void AIU_PlayerMenu::TerminateSaveSelection()
{
	if (!hud || !camera) return;

	camera->OnArrivedAtPoint().RemoveDynamic(this, &AIU_PlayerMenu::InitializeSaveSelection);
	
	saveMenuWidget = nullptr;
	saveSelection = false;
	DisableCursorTrace();

	if (saveIndex < 0) return;
	
	for (AS_SaveActor* _current : saveActors)
	{
		_current->UnhoverSave();
		_current->ClearSaveAppearance();
	}
	
	saveIndex = -1;
}

void AIU_PlayerMenu::NextSave()
{
	if (!saveSelection || saveIndex < 0) return;

	saveActors[saveIndex]->UnhoverSave();
	saveIndex++;
	saveIndex = saveIndex >= saveActors.Num() ? saveActors.Num() - 1 : saveIndex;
	saveActors[saveIndex]->HoverSave();
}
void AIU_PlayerMenu::PreviousSave()
{
	if (!saveSelection || saveIndex < 0) return;

	saveActors[saveIndex]->UnhoverSave();
	saveIndex--;
	saveIndex = saveIndex < 0 ? 0 : saveIndex;
	saveActors[saveIndex]->HoverSave();
}
void AIU_PlayerMenu::SelectSave()
{
	if (!saveSelection || !saveMenuWidget || saveIndex < 0) return;
	saveActors[saveIndex]->SelectSave();
	saveMenuWidget->SelectGame(saveIndex);
}

void AIU_PlayerMenu::CheckCursor()
{
	if (!playerController || !saveSelection) return;
	
	FVector _location = FVector::ZeroVector;
	FVector _direction = FVector::ZeroVector;
	if (playerController->DeprojectMousePositionToWorld(_location, _direction))
	{		
		const FVector& _dir = _location - camera->GetCamera()->GetComponentLocation();

		//DrawDebugLine(GetWorld(), _location, _location + _dir * 150, FColor::Red, false, checkRate, 0, 2);
		FHitResult _result;
		if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _location, _location + _dir * 500, { saveActorLayer }, false, { }, EDrawDebugTrace::None, _result, true))
		{
			HoverSaveActor(Cast<AS_SaveActor>(_result.GetActor()));
		}
	}
}

void AIU_PlayerMenu::HoverSaveActor(AS_SaveActor* _saveActor)
{
	if (!_saveActor) return;
	const int _index = _saveActor->GetSaveIndex();

	if (!saveSelection || !saveMenuWidget || saveIndex < 0 || saveIndex >= saveActors.Num()) return;
	saveActors[saveIndex]->UnhoverSave();
	saveIndex = _index;
	saveActors[saveIndex]->HoverSave();
}

void AIU_PlayerMenu::DeleteHoveredSave()
{
	if (!saveSelection || !saveMenuWidget || saveIndex < 0 || saveIndex >= saveActors.Num()) return;
	saveActors[saveIndex]->DeleteSave();
}
