#include "M_CinematicManager.h"

#include "STRUCT_GameInput.h"

#include "M_GameInstance.h"
#include "M_GameMode.h"
#include "S_GameSave.h"

#include "HUD_Game.h"
#include "UI_CinematicWidget.h"

#include "IU_Player.h"
#include "UC_PlayerAttackComponent.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"

#include "IU_Macros.h"

AM_CinematicManager::AM_CinematicManager()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");
}

void AM_CinematicManager::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &AM_CinematicManager::InitializeCinematicManager, initTime);
}

void AM_CinematicManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (isHolding)
	{
		holdingTime += DeltaSeconds;
		if (widget) widget->UpdateProgressBar(holdingTime, holdTime);
		
		if (holdingTime > holdTime)
		{
			holdingTime = 0;	
			EndHold();
			OnSequenceFinished();
		}
	}
}

void AM_CinematicManager::InitializeCinematicManager()
{
	const UWorld* _w = GetWorld();
		
	if (const AM_GameMode* _gm = _w->GetAuthGameMode<AM_GameMode>())
	{
		player = _gm->GetPlayer();
		playerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		hud = playerController->GetHUD<AHUD_Game>();
		if (hud)
			widget = hud->GetWidget<UUI_CinematicWidget>(WIDGET_CINEMATIC);
	}

	if (playerController && playerController->InputComponent)
	{
		const FGameInput& _gameInput = FGameInput();
		playerController->InputComponent->BindAction(_gameInput.AnyKeyAction.ActionName, IE_Pressed, this, &AM_CinematicManager::BeginHold).bConsumeInput = false;
		playerController->InputComponent->BindAction(_gameInput.AnyKeyAction.ActionName, IE_Released, this, &AM_CinematicManager::EndHold).bConsumeInput = false;
	}
#if !WITH_EDITOR
	if (const UM_GameInstance* _gi = GetGameInstance<UM_GameInstance>())
	{
		if (_gi->GetCurrentGameSave() && _gi->GetCurrentGameSave()->GetProgressDone())
		{
			playerController->SetInputMode(FInputModeGameOnly());
			playerController->SetShowMouseCursor(false);
			return;
		}
	}
#endif
}

void AM_CinematicManager::PlayLevelIntroCinematic()
{
	if (PlayCinematic(levelIntroCinematic))
	{
		currentLevelSequenceActor->OnDestroyed.AddUniqueDynamic(this, &AM_CinematicManager::InitializeHUD);
		OnIntroCinematicPlayed();
	}
	else
	{
		InitializeHUD(nullptr);
	}
}

void AM_CinematicManager::PlayLevelTwoIntroCinematic()
{
	if (PlayCinematic(levelTwoIntroCinematic))
		OnIntroTwoCinematicPlayed();
}

void AM_CinematicManager::PlayLevelThreeIntroCinematic()
{
	if (PlayCinematic(levelThreeIntroCinematic))
		OnIntroThreeCinematicPlayed();	
}

void AM_CinematicManager::PlayTutoCinematic()
{
	if (PlayCinematic(tutoIntroCinematic))
		OnTutoIntroCinematicPlayed();
}
void AM_CinematicManager::PlayTutoOutroCinematic()
{
	if (PlayCinematic(tutoOutroCinematic))
		OnTutoOutroCinematicPlayed();
}

void AM_CinematicManager::PlayTempleDoorCinematic()
{
	if (PlayCinematic(templeDoorCinematic))
		OnTempleDoorCinematicPlayed();
}

void AM_CinematicManager::PlayBossIntroCinematic(const EBossPhase& _bossPhase)
{
	if (!bossIntroCinematic.Contains(_bossPhase)) return;

#if !WITH_EDITOR
	canPass = false;
#endif
	if (PlayCinematic(bossIntroCinematic[_bossPhase]))
		OnBossIntroCinematicPlayed(_bossPhase);
}
void AM_CinematicManager::PlayBossEndCinematic(const EBossPhase& _bossPhase)
{
	if (!bossEndCinematic.Contains(_bossPhase)) return;

#if !WITH_EDITOR
	canPass = false;
#endif
	if (PlayCinematic(bossEndCinematic[_bossPhase]))
		OnBossOutroCinematicPlayed(_bossPhase);

	if (_bossPhase == EBossPhase::TROIS && hud)
		onCinematicFinishedEvent.AddUniqueDynamic(hud, &AHUD_Game::ShowCredits);
}

bool AM_CinematicManager::PlayCinematic(const FCinematicData& _cinematicData, const bool _hidePlayer)
{
	if (!CanPlayCinematic()) return false;

	bool _cinematic = false;
	if (_cinematicData.IsSequenceValid())
	{
		_cinematic = PlaySequence(_cinematicData.cinematicSequence);
	}
	if (_cinematicData.IsMediaValid())
	{
		_cinematic = PlayMedia(_cinematicData.cinematicMediaPlayer, _cinematicData.cinematicSource, _cinematicData.cinematicMaterial);
	}

	if (_cinematic)
	{
		LockPlayer(_hidePlayer);
		ShowCinematicWidget();
	}

	return _cinematic;
}

bool AM_CinematicManager::PlaySequence(ULevelSequence* _sequence, const bool hidePlayer)
{
	if (ULevelSequencePlayer* _sequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), _sequence, FMovieSceneSequencePlaybackSettings(), currentLevelSequenceActor))
	{
		_sequencePlayer->Play();
		_sequencePlayer->OnFinished.AddUniqueDynamic(this, &AM_CinematicManager::OnSequenceFinished);
		return true;
	}
	return false;
}

bool AM_CinematicManager::PlayMedia(UMediaPlayer* _mediaPlayer, UMediaSource* _mediaSource, UMaterialInstance* _mediaMaterial)
{
	if (widget && _mediaPlayer)
	{
		currentMediaPlayer = _mediaPlayer;
		currentMediaPlayer->OnEndReached.AddUniqueDynamic(this, &AM_CinematicManager::OnSequenceFinished);
		currentMediaPlayer->OpenSource(_mediaSource);		
		widget->SetDisplayImage(_mediaMaterial);
		widget->ShowDisplayImage();
		return true;
	}
	return false;
}

void AM_CinematicManager::LockPlayer(const bool& _hide)
{
	player->DisableInput(playerController);
	player->GetCharacterAttackComponent()->CheckHoldingTime();
	playerController->FlushPressedKeys();
	player->SetActorHiddenInGame(_hide);
	player->HideBow();
}
void AM_CinematicManager::UnlockPlayer()
{
	player->EnableInput(playerController);
	player->SetActorHiddenInGame(false);
	player->ShowBow();
}

void AM_CinematicManager::ShowCinematicWidget()
{
	if (hud)
		hud->ShowCinematicWidget();
	if (widget)
		widget->SetPassContentVisibility(canPass ? ESlateVisibility::Visible : ESlateVisibility::Hidden);	
}
void AM_CinematicManager::HideCinematicWidget()
{
	if (hud)
		hud->HideCinematicWidget();
	if (widget)
		widget->HideDisplayImage();	
}

void AM_CinematicManager::BeginHold()
{
	if (!currentLevelSequenceActor && !currentMediaPlayer || isHolding || !canPass) return;		//Avoid Holding if there's no cinematic
	
	holdingTime = 0;
	isHolding = true;
	SetActorTickEnabled(true);
	if (widget)
		widget->ResetProgressbar();
}
void AM_CinematicManager::EndHold()
{
	isHolding = false;
	SetActorTickEnabled(false);
	if (widget)
		widget->ResetProgressbar();
}

void AM_CinematicManager::OnSequenceFinished()
{
	if (!IsCinematicManagerValid() || !currentLevelSequenceActor && !currentMediaPlayer) return;

	canPass = true;
	if (currentLevelSequenceActor)
	{
		currentLevelSequenceActor->Destroy();
		currentLevelSequenceActor = nullptr;		
	}

	if (currentMediaPlayer)
	{
		currentMediaPlayer->Close();
		currentMediaPlayer->OnEndReached.RemoveDynamic(this, &AM_CinematicManager::OnSequenceFinished);
		currentMediaPlayer = nullptr;
	}
	
	HideCinematicWidget();
	UnlockPlayer();
	
	onCinematicFinishedEvent.Broadcast();
	OnCinematicFinished();
}

void AM_CinematicManager::InitializeHUD(AActor* _levelSequenceActor)
{
	if (_levelSequenceActor)
		_levelSequenceActor->OnDestroyed.RemoveDynamic(this, &AM_CinematicManager::InitializeHUD);

	if (hud)
		hud->ShowInitialWidget();
}