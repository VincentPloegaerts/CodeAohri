#include "B_Boss.h"

#include "IU_Macros.h"
#include "M_GameMode.h"

#include "M_ArenaManager.h"
#include "A_Arena.h"
#include "A_ArenaBlock.h"

#include "IU_HealthSystemComponent.h"

#include "B_AIBoss.h"
#include "B_BossAnim.h"
#include "B_BossCritPoint.h"
#include "P_PatternCaster.h"
#include "B_BossMovement.h"
#include "B_BossSpecialAnim.h"
#include "B_DT_CritPointDataTable.h"
#include "C_RespawnComponent.h"
#include "C_SlowMotionComponent.h"
#include "HUD_Main.h"

#include "IU_Player.h"
#include "IU_PlayerMovementComponent.h"
#include "M_BlackBoardManager.h"
#include "M_BossManager.h"
#include "M_CinematicManager.h"
#include "M_DifficultyManager.h"
#include "M_GameInstance.h"
#include "TriggerBoxLevelLoading.h"
#include "UC_PlayerAttackComponent.h"
#include "UI_BossWidget.h"
#include "UI_PlayerWidget.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"

AB_Boss::AB_Boss()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	capsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	capsuleCollider->SetupAttachment(RootComponent);
	
	skeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	skeletalMeshComponent->SetupAttachment(RootComponent);
	
	patternCaster = CreateDefaultSubobject<UP_PatternCaster>(TEXT("PatternCaster"));
	AddOwnedComponent(patternCaster);
	
	bossMovement = CreateDefaultSubobject<UB_BossMovement>(TEXT("BossMovement"));
	AddOwnedComponent(bossMovement);
	
	healthComponent = CreateDefaultSubobject<UIU_HealthSystemComponent>(TEXT("HealthSystemComponent"));
	AddOwnedComponent(healthComponent);

	bossSpecialAnim = CreateDefaultSubobject<UB_BossSpecialAnim>(TEXT("BossSpecialAnim"));
	AddOwnedComponent(bossSpecialAnim);
}

void AB_Boss::BeginPlay()
{
	Super::BeginPlay();

#if DEBUG_BOSS
	LOG_ERROR("%s -> BeginPlay", *GetName());
#endif
	
	world = GetWorld();
	timerManager = &world->GetTimerManager();
	healthComponent->OnDeath()->AddDynamic(this, &AB_Boss::BossDeath);
	gameMode = world->GetAuthGameMode<AM_GameMode>();
	aiBoss = GetController<AB_AIBoss>();

	InitializeCritPoint();
	
	ROG_ERROR(!gameMode || !aiBoss, "AB_Boss::BeginPlay -> Invalid Ai_Boss Controller or GameMode");
	
	blackBoardManager = gameMode->GetBlackBoardManager();
	ROG_ERROR(!blackBoardManager, "AB_Boss::BeginPlay -> Invalid black board manager");

	worldTimeManager = gameMode->GetWorldTimeManager();
	ROG_ERROR(!worldTimeManager, "AB_Boss::BeginPlay -> Invalid world time manager");

	Event_AddUnique(worldTimeManager->OnWorldSpeedChange(), AB_Boss::ChangeAnimWorldSpeed);
	
	skeletalMeshComponent->SetComponentTickEnabled(false);
	
	if (knockbackSettings.bActivateKnockBack)
	{
		Event_AddUnique(OnActorBeginOverlap, AB_Boss::StartKnockBackPlayer);
		Event_AddUnique(OnActorEndOverlap, AB_Boss::EndKnockBackPlayer);
	}

	Event_AddUnique(onBossStunned, AB_Boss::WhenBossIsStunStopMove);
	
	capsuleCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (AM_BossManager* _bossManager = gameMode->GetBossManager())
	{
		_bossManager->SetBoss(this, bossPhase);
	}
	cinematicManager = gameMode->GetCinematicManager();
	if (cinematicManager)
	{
		onBossDeadCinematic.AddUniqueDynamic(cinematicManager, &AM_CinematicManager::PlayBossEndCinematic);
	}

	if (const UM_GameInstance* _gameInstance = world->GetGameInstance<UM_GameInstance>())
	{
		difficultyManager = _gameInstance->GetDifficultyManager();
	}
}

void AB_Boss::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	onTickAnim.Broadcast(DeltaSeconds);
}

void AB_Boss::Init()
{
#if DEBUG_BOSS
	LOG_ERROR("%s -> Init", *GetName());
#endif
	
	if (skeletalMeshComponent)
	{
		if (UAnimInstance* _animInstance = skeletalMeshComponent->GetAnimInstance())
		{
			bossAnimInstance = Cast<UB_BossAnim>(_animInstance);
			ROG(!bossAnimInstance, "AB_Boss::Init -> Invalid boss anim instance");
		}
	}
	
	if (healthComponent)
	{
		healthComponent->OnLifeUpdatedWithMaxLife()->AddUniqueDynamic(this, &AB_Boss::CheckIsMidLife);
	}
	
	if (AHUD_Main* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>())
	{
		if (UUI_BossWidget* _bossWidget = _hud->GetWidget<UUI_BossWidget>(WIDGET_BOSS))
			_bossWidget->InitializeBossWidget(this);

		if (UUI_PlayerWidget* _playerWidget = _hud->GetWidget<UUI_PlayerWidget>(WIDGET_PLAYER))
			_playerWidget->InitializePlayerBossFight(this);
	}
	
	RegisterToArena();
}

void AB_Boss::RegisterToArena()
{
#if DEBUG_BOSS
	LOG_ERROR("%s -> RegisterToArena", *GetName());
#endif
	if (AM_ArenaManager* _arenaManager = gameMode->GetArenaManager())
	{
		if (const AA_Arena* _arena = _arenaManager->GetArena())
		{
			if (_arena->GetArenaBossPhase() == bossPhase)
			{
				onBossActivated.AddDynamic(_arena, &AA_Arena::CloseArena);
				onBossReset.AddDynamic(_arena, &AA_Arena::BeginOpenArena);
			
				if (AA_ArenaBlock* _block = _arena->GetArenaBlock())
					Event_AddUnique(_block->OnBlockTrigger(), AB_Boss::ActivateBoss);
				else
					LOG("AB_Boss::RegisterToArena -> Invalid ArenaBlock");
				return;
			}
		}
		
		_arenaManager->OnArenaSetted()->AddDynamic(this, &AB_Boss::PostRegisterToArena);
	}
	else
		LOG("AB_Boss::RegisterToArena -> Invalid ArenaManager");
}

void AB_Boss::PostRegisterToArena(AA_Arena* _arena)
{
#if DEBUG_BOSS
	LOG_ERROR("%s -> PostRegisterToArena", *GetName());
#endif
	if (AM_ArenaManager* _arenaManager = gameMode->GetArenaManager())
	{
		_arenaManager->OnArenaSetted()->RemoveDynamic(this, &AB_Boss::PostRegisterToArena);

		if (_arena->GetArenaBossPhase() == bossPhase)
		{
			onBossActivated.AddDynamic(_arena, &AA_Arena::CloseArena);
			onBossReset.AddDynamic(_arena, &AA_Arena::BeginOpenArena);
			
			if (AA_ArenaBlock* _block = _arena->GetArenaBlock())
				Event_AddUnique(_block->OnBlockTrigger(), AB_Boss::ActivateBoss);
			else
				LOG("AB_Boss::PostRegisterToArena -> Invalid ArenaBlock");
		}
		else
			LOG("AB_Boss::PostRegisterToArena -> Boss phase not corresponding between arena and boss");
	}
}

#pragma region CritPoint
void AB_Boss::InitializeCritPoint()
{
	if (!critPointDataTable) return;
	
	const TArray<FCritPointData>& _areasData = critPointDataTable->GetCritPointData();
	const int& _max = _areasData.Num();
	
	if (!skeletalMeshComponent) return;
	
	for (int i = 0; i < _max; ++i)
	{
		const FCritPointData& _current = _areasData[i];
		const FName& _socketName = _current.GetSocketName();
		const TSubclassOf<AB_BossCritPoint>& _type = _current.GetCritPointType();
		if (!_type.Get()) continue;
		
		if (AB_BossCritPoint* _spawned = GetWorld()->SpawnActor<AB_BossCritPoint>(_type, GetActorLocation(), GetActorRotation()))
		{
			if (USkeletalMeshSocket const* _mesh = skeletalMeshComponent->GetSocketByName(_socketName))
			{
				_mesh->AttachActor(_spawned, skeletalMeshComponent);
				_spawned->SetCritPointSettings(_current.GetCritPointSettings());
				_spawned->SetBoss(this);

				critPoints.Add(_spawned);
			}
		}	
	}

	UnactivateCritPointCollision();
}

void AB_Boss::ActivateCritPointCollision()
{
	const uint& _max = critPoints.Num();
	for (uint i = 0; i < _max; ++i)
	{
		if (AB_BossCritPoint* _critPoint = critPoints[i])
		{
			_critPoint->SetActorEnableCollision(true);
		}
	}
}

void AB_Boss::UnactivateCritPointCollision()
{
	const uint& _max = critPoints.Num();
	for (uint i = 0; i < _max; ++i)
	{
		if (AB_BossCritPoint* _critPoint = critPoints[i])
		{
			_critPoint->SetActorEnableCollision(false);
		}
	}
}
#pragma endregion CritPoint

void AB_Boss::RegisterToPlayerDeath()
{
#if DEBUG_BOSS
	LOG_ERROR("%s -> RegisterToPlayerDeath", *GetName());
#endif
	if (!player)
	{
		player = gameMode->GetPlayer();
		ROG(!player, "AB_Boss::RegisterToPlayerDeath -> Invalid Player");
	}

	if (!playerRespawnComponent)
		playerRespawnComponent = player->RespawnComponent();

	if (playerRespawnComponent)
		Event_AddUnique(playerRespawnComponent->OnRespawnStart(), AB_Boss::ResetBoss);
}

void AB_Boss::BossDeath()
{
#if DEBUG_BOSS
	LOG_ERROR("%s -> BossDeath", *GetName());
#endif
	if (healthComponent)
	{
		healthComponent->OnDeath()->RemoveDynamic(this, &AB_Boss::BossDeath);
		healthComponent->OnLifeUpdatedWithMaxLife()->RemoveDynamic(this, &AB_Boss::CheckIsMidLife);
	}
	
	if (playerRespawnComponent)
		Event_Remove(playerRespawnComponent->OnRespawnStart(), AB_Boss::ResetBoss);

	if (player)
	{
		if (UC_PlayerAttackComponent* _playerAttackComponent = player->GetCharacterAttackComponent())
		{
			_playerAttackComponent->GetValidSlowMoOrbCount(_playerAttackComponent->GetCurrentSlowMoOrbCount());
			_playerAttackComponent->ResetShootState();
		}
	}

	if (bossAnimInstance)
	{
		UB_BossAnim::FOnAnimationEnd& _animCallback = bossAnimInstance->OnAnimationEnd();
		Event_Remove(_animCallback, AB_Boss::Internal_SetInGround);
		Event_Remove(_animCallback, AB_Boss::Internal_SetOutGround);
		Event_Remove(bossAnimInstance->OnAnimSpecialEnd(), AB_Boss::StunBossAnimationEnd);
	}
	
	blackBoardManager->STOP_DEBUG();
	timerManager->ClearTimer(animOutGroundTimer);
	timerManager->ClearTimer(animInGroundTimer);

	Event_Remove(onBossStunnedEnd, AB_Boss::StartMidLifeAnim);

	timerManager->SetTimer(deadTimer, this, &AB_Boss::PlayDeathAnimation, animDeathDelay);
	capsuleCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UnactivateCritPointCollision();
	knockbackSettings.bActivateKnockBack = false;
	onBossDead.Broadcast();

	timerManager->SetTimer(cinematicTimer, this, &AB_Boss::BroadcastCinematic, cinematicDeathDelay);
	
	if (cameraDelayDeath > 0.0f)
	{
		FTimerHandle _handle;
		timerManager->SetTimer(_handle, this, &AB_Boss::ChangeCam, cameraDelayDeath);
	}
	else
	{
		ChangeCam();
	}
	
	if (const UM_GameInstance* _gameInstance = GetGameInstance<UM_GameInstance>())
	{
		if (UAPI_SteamIntegration* _steamIntegration = _gameInstance->GetSteamIntegration())
		{
			const UM_DifficultyManager* _difficultyManager = _gameInstance->GetDifficultyManager();
			ROG_ERROR(!_difficultyManager, "AB_Boss::BossDeath -> Invalid Difficulty manager");
			const EDifficultyType& _currentDifficulty = _difficultyManager->GetCurrentDifficulty();
			
			if (bossPhase == EBossPhase::UN)
			{
				_steamIntegration->ValidateSuccess("ACH_INITIATED_1");
				
				if (_currentDifficulty == EDifficultyType::DIFFICULTY_BORN)
				{
					_steamIntegration->ValidateSuccess("ACH_BORN_1");
				}
				else if (_currentDifficulty == EDifficultyType::DIFFICULTY_ELITE)
				{
					_steamIntegration->ValidateSuccess("ACH_BORN_1");
					_steamIntegration->ValidateSuccess("ACH_ELITE_1");
				}
			}
			else if (bossPhase == EBossPhase::DEUX)
			{
				_steamIntegration->ValidateSuccess("ACH_INITIATED_2");
				
				if (_currentDifficulty == EDifficultyType::DIFFICULTY_BORN)
				{
					_steamIntegration->ValidateSuccess("ACH_BORN_2");
				}
				else if (_currentDifficulty == EDifficultyType::DIFFICULTY_ELITE)
				{
					_steamIntegration->ValidateSuccess("ACH_BORN_2");
					_steamIntegration->ValidateSuccess("ACH_ELITE_2");
				}
			}
			else //bossPhase == EBossPhase::TROIS
			{
				_steamIntegration->ValidateSuccess("ACH_INITIATED_3");
				_steamIntegration->ValidateSuccess("ACH_INITIATED_ALL");
				
				if (_currentDifficulty == EDifficultyType::DIFFICULTY_BORN)
				{
					_steamIntegration->ValidateSuccess("ACH_BORN_3");
					_steamIntegration->ValidateSuccess("ACH_BORN_ALL");
				}
				else if (_currentDifficulty == EDifficultyType::DIFFICULTY_ELITE)
				{
					_steamIntegration->ValidateSuccess("ACH_BORN_3");
					_steamIntegration->ValidateSuccess("ACH_BORN_ALL");
					_steamIntegration->ValidateSuccess("ACH_ELITE_3");
					_steamIntegration->ValidateSuccess("ACH_ELITE_ALL");
				}
			}
		}
	}
}

void AB_Boss::BroadcastCinematic()
{
	LoadLevelsAfterDeath();
	onBossDeadCinematic.Broadcast(bossPhase);
}

void AB_Boss::ChangeCam()
{
	if (player)
		player->InitCameraByType(ECameraType::CAMERA_PLATEFORMING);
}

void AB_Boss::MakeBossVisible()
{
	blackBoardManager->UPDATE_DEBUG();
	skeletalMeshComponent->SetVisibility(true);
	capsuleCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	knockbackSettings.bActivateKnockBack = true;
	ActivateCritPointCollision();
}

void AB_Boss::ActivateBoss()
{
#if DEBUG_BOSS
	LOG_ERROR("%s -> ActivateBoss", *GetName());
#endif

	if (healthComponent && difficultyManager)
	{
		FDifficultyData _data;
		if (difficultyManager->GetCurrentDifficultyData(_data))
		{
			if (_data.bossesLife.Contains(bossPhase))
			{
				healthComponent->SetMaxHealth(_data.bossesLife[bossPhase]);
				healthComponent->SetCanRegen(true);
				healthComponent->ResetHealth();
				healthComponent->SetCanRegen(false);
			}
		}
	}

	blackBoardManager->SetBossIsActivated();
	
	MakeBossVisible();
	RegisterToPlayerDeath();

	Event_AddUnique(onTickAnim, AB_Boss::UpdateSkeletalAnim);

	onBossActivated.Broadcast();
}

void AB_Boss::ResetBoss()
{
#if DEBUG_BOSS
	LOG_ERROR("%s -> ResetBoss", *GetName());
#endif
	if (healthComponent)
	{
		if (bCanResetHealth)
			healthComponent->ResetHealth();
		SetInvincibleTimeStart(2.0f);
	}

	healthComponent->OnLifeUpdatedWithMaxLife()->AddUniqueDynamic(this, &AB_Boss::CheckIsMidLife);
	bIsMidLife = false;
	
	if (playerRespawnComponent)
		Event_Remove(playerRespawnComponent->OnRespawnStart(), AB_Boss::ResetBoss);

	blackBoardManager->STOP_DEBUG();
	timerManager->ClearTimer(animOutGroundTimer);
	timerManager->ClearTimer(animInGroundTimer);
	
	skeletalMeshComponent->SetVisibility(true);
	capsuleCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UnactivateCritPointCollision();
	
	if (bossAnimInstance)
	{
		UB_BossAnim::FOnAnimationEnd& _animCallback = bossAnimInstance->OnAnimationEnd();
		Event_Remove(_animCallback, AB_Boss::Internal_SetInGround);
		Event_Remove(_animCallback, AB_Boss::Internal_SetOutGround);
		Event_Remove(bossAnimInstance->OnAnimSpecialEnd(), AB_Boss::StunBossAnimationEnd);
	}
	
	Event_Remove(onBossStunnedEnd, AB_Boss::StartMidLifeAnim);
	
	onBossReset.Broadcast();
	blackBoardManager->Broadcast_Boss_OnBossReset();
}

void AB_Boss::LoadLevelsAfterDeath() const
{
	if (saveTriggerRefEndBoss.Get())
	{
		saveTriggerRefEndBoss.Get()->ActivateSave();
		TArray<TSoftObjectPtr<ATriggerBoxLevelLoading>> _array = saveTriggerRefEndBoss.Get()->CorrespondingLevelStreamer();
		for (int i = 0;i< _array.Num();i++)
		{
			if (!_array.IsValidIndex(i) || !_array[i].Get())continue;
			_array[i].Get()->AddLevels();
		}
	}
}

#pragma region KnockBack
void AB_Boss::StartKnockBackPlayer(AActor* _mine, AActor* _other)
{
	if (_other != player)return;
	if (knockbackSettings.bActivateKnockBack)
	{
		timerManager->SetTimer(knockBackTimer, this, &AB_Boss::KnockBackPlayer, 0.1f, true);
		
		if (UIU_PlayerMovementComponent* _playerMovement = player->MovementComponent())
		{
			FVector _direction(player->GetActorLocation() - GetActorLocation());
			_direction.Z = 0.f;
			_direction = _direction.GetSafeNormal(UE_SMALL_NUMBER, FVector::ForwardVector);
			_direction *= knockbackSettings.fKnockBackStrenght;
			_playerMovement->SetMomentum(FVector(0, 0, -50));
			_playerMovement->StartKnockBack(_direction, knockbackSettings.fKnockBackDuration);
		}
	}
}

void AB_Boss::KnockBackPlayer()
{
	if (knockbackSettings.bActivateKnockBack)
	{
		if (UIU_PlayerMovementComponent* _playerMovement = player->MovementComponent())
		{
			FVector _direction(player->GetActorLocation() - GetActorLocation());
			_direction.Z = 0.f;
			_direction = _direction.GetSafeNormal(UE_SMALL_NUMBER, FVector::ForwardVector);
			_direction *= knockbackSettings.fKnockBackStrenght;
			_playerMovement->SetMomentum(FVector(0, 0, -50));
			_playerMovement->StartKnockBack(_direction, knockbackSettings.fKnockBackDuration);
		}
	}
}

void AB_Boss::EndKnockBackPlayer(AActor* _mine, AActor* _other)
{
	if (_other != player)return;
	timerManager->ClearTimer(knockBackTimer);
}
#pragma endregion KnockBack

#pragma region Stun
void AB_Boss::SetHarpoonData(int _iStackNeededToStun, float _fHarpoonStunDuration)
{
	iStackNeededToStun = _iStackNeededToStun;
	fHarpoonStunDuration = _fHarpoonStunDuration;
}

void AB_Boss::CheckIfStunned()
{
	blackBoardManager->UPDATE_DEBUG();
	if (bIsStunned)return;
	++iCurrentHarpoonStack;
	if(iCurrentHarpoonStack >= iStackNeededToStun)
		StunBoss(fHarpoonStunDuration);
}

void AB_Boss::StunBoss(const float& _stunDuration)
{
	if (bIsMidLifeAnimPlaying)
	{
#if WITH_LOG_STUN_BOSS
		LOG_ERROR("Boss : Stun impossible cause isMidLife anim is playing");
#endif
		return;
	}

	if (bIsStunned || !bCanBeStun)return;

#if WITH_LOG_STUN_BOSS
	LOG_ERROR("Boss : Stun start for %f", _stunDuration);
#endif
	
	timerManager->SetTimer(stunTimer, this, &AB_Boss::StunBossEnd, _stunDuration);
	patternCaster->ClearTimer();
	patternCaster->DestroyEveryPattern();
	bIsStunned = true;
	iCurrentHarpoonStack = 0;
	
	if (bossAnimInstance)
		bossAnimInstance->PlaySpecialAnim(EBossSpecialAnimState::STUN_START);
	
	onBossStunned.Broadcast();
	blackBoardManager->Broadcast_Boss_OnBossStunned();
}

void AB_Boss::StunBossEnd()
{
	blackBoardManager->UPDATE_DEBUG();

#if WITH_LOG_STUN_BOSS
	LOG_ERROR("Boss : Start Stun end Animation");
#endif
	
	if (bossAnimInstance)
	{
		bossAnimInstance->PlaySpecialAnim(EBossSpecialAnimState::STUN_END);
		Event_AddUnique(bossAnimInstance->OnAnimSpecialEnd(), AB_Boss::StunBossAnimationEnd);
	}
}

void AB_Boss::StunBossAnimationEnd(const TEnumAsByte<EBossSpecialAnimState>& _specialAnim)
{
#if WITH_LOG_STUN_BOSS
	LOG_ERROR("Boss : End Stun end with anim %s", *UEnum::GetValueAsString(_specialAnim));
#endif
	
	if (_specialAnim != EBossSpecialAnimState::STUN_END)return;
	
	blackBoardManager->UPDATE_DEBUG();
	
	if (bossAnimInstance)
	{
		Event_Remove(bossAnimInstance->OnAnimSpecialEnd(), AB_Boss::StunBossAnimationEnd);
		bossAnimInstance->PlaySpecialAnim(EBossSpecialAnimState::NONE__);
	}
	
	bIsStunned = false;

	if (bIsMidLifeAnimPlaying)
		StartMidLifeAnim();
	else
	{
		onBossStunnedEnd.Broadcast();
		blackBoardManager->Broadcast_Boss_OnBossStunnedEnd();
	}
}

void AB_Boss::WhenBossIsStunStopMove()
{
	UB_BossAnim::FOnAnimationEnd& _onAnimationEnd = bossAnimInstance->OnAnimationEnd();
	Event_Remove(_onAnimationEnd, AB_Boss::Internal_SetInGround);
	Event_Remove(_onAnimationEnd, AB_Boss::Internal_SetOutGround);
	Event_Remove(_onAnimationEnd, AB_Boss::Internal_SetStartDash);
	Event_Remove(_onAnimationEnd, AB_Boss::Internal_SetEndDash);

	blackBoardManager->UPDATE_DEBUG();
	
	skeletalMeshComponent->SetVisibility(true);
	capsuleCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	knockbackSettings.bActivateKnockBack = true;
	ActivateCritPointCollision();

}

void AB_Boss::SetInvincibleStart()
{
	if (healthComponent)
		healthComponent->SetCanTakeDamages(false);
}

void AB_Boss::SetInvincibleTimeStart(const float& _time)
{
	SetInvincibleStart();
	timerManager->SetTimer(invincibleTimer, this, &AB_Boss::SetInvincibleEnd, _time);
}

void AB_Boss::SetInvincibleEnd()
{
	if (healthComponent)
		healthComponent->SetCanTakeDamages(true);
}

void AB_Boss::UnStunLockAfterTime()
{
	bIsStunned = false;
}
#pragma endregion Stun

#pragma region Movement
void AB_Boss::SetInGround()
{
	bCanBeStun = false;
	Event_AddUnique(bossAnimInstance->OnAnimationEnd(), AB_Boss::Internal_SetInGround);
#if WITH_LOG_MOVEMENT_BOSS
	LOG("AB_Boss::SetInGround -> Start Anim In Ground");
#endif

	timerManager->SetTimer(animInGroundTimer, this, &AB_Boss::Internal_SetInGround, 1.f);
	
	MakeBossVisible();
}

void AB_Boss::SetOutGround()
{
	Event_AddUnique(bossAnimInstance->OnAnimationEnd(), AB_Boss::Internal_SetOutGround);
	
	MakeBossVisible();

	timerManager->SetTimer(animOutGroundTimer, this, &AB_Boss::Internal_SetOutGround, 1.f);

#if WITH_LOG_MOVEMENT_BOSS
	LOG("AB_Boss::SetOutGround -> Start Anim Out Ground");
#endif
}

void AB_Boss::SetStartDash()
{
	Event_AddUnique(bossAnimInstance->OnAnimationEnd(), AB_Boss::Internal_SetStartDash);
	
	bCanBeStun = false;
	
	MakeBossVisible();
	
#if WITH_LOG_MOVEMENT_BOSS
	LOG_ERROR("Boss -> Set Start Dash");
#endif
}

void AB_Boss::SetEndDash()
{
	Event_AddUnique(bossAnimInstance->OnAnimationEnd(), AB_Boss::Internal_SetEndDash);

	skeletalMeshComponent->SetVisibility(true);
	capsuleCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	knockbackSettings.bActivateKnockBack = true;
	ActivateCritPointCollision();
	
	blackBoardManager->UPDATE_DEBUG();
	
#if WITH_LOG_MOVEMENT_BOSS
	LOG_ERROR("Boss -> Set End Dash");
#endif
}

void AB_Boss::Internal_SetInGround()
{
	Event_Remove(bossAnimInstance->OnAnimationEnd(), AB_Boss::Internal_SetInGround);
	timerManager->ClearTimer(animInGroundTimer);
	timerManager->ClearTimer(animOutGroundTimer);
	
	blackBoardManager->UPDATE_DEBUG();

	if (!bIsStunned && !bIsMidLifeAnimPlaying)
	{
		skeletalMeshComponent->SetVisibility(false);
		capsuleCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		knockbackSettings.bActivateKnockBack = false;
		UnactivateCritPointCollision();
	}

#if WITH_LOG_MOVEMENT_BOSS
	LOG("AB_Boss::Internal_SetInGround -> Make boss invisible and untargetable");
#endif
	onInGround.Broadcast();
}

void AB_Boss::Internal_SetOutGround()
{
	bCanBeStun = true;
	
	Event_Remove(bossAnimInstance->OnAnimationEnd(), AB_Boss::Internal_SetOutGround);
	timerManager->ClearTimer(animOutGroundTimer);
	timerManager->ClearTimer(animInGroundTimer);
	
	blackBoardManager->UPDATE_DEBUG();

#if WITH_LOG_MOVEMENT_BOSS
	LOG("AB_Boss::Internal_SetOutGround -> Make boss visible and targetable");
#endif
	onOutGround.Broadcast();
}

void AB_Boss::Internal_SetStartDash()
{
#if WITH_LOG_MOVEMENT_BOSS
	LOG_ERROR("Boss -> Internal Set Start Dash");
#endif
	
	MakeBossVisible();
	
	Event_Remove(bossAnimInstance->OnAnimationEnd(), AB_Boss::Internal_SetStartDash);
	onStartDash.Broadcast();
}

void AB_Boss::Internal_SetEndDash()
{
#if WITH_LOG_MOVEMENT_BOSS
	LOG_ERROR("Boss -> Internal Set End Dash");
#endif
	bCanBeStun = true;
	
	MakeBossVisible();
	
	Event_Remove(bossAnimInstance->OnAnimationEnd(), AB_Boss::Internal_SetEndDash);
	onEndDash.Broadcast();
}
#pragma endregion Movement

#pragma region Anim
void AB_Boss::CheckIsMidLife(float _actualLife, float _maxLife)
{
	const float& _currentPercent = _actualLife / _maxLife;
	if (_currentPercent <= 0.01f)return;
	if (_currentPercent <= 0.5f)
	{
#if WITH_LOG_STUN_BOSS
		LOG_ERROR("Boss : Stun state = %s", bIsStunned ? *FString("True") : *FString("False"));
#endif
		
		healthComponent->OnLifeUpdatedWithMaxLife()->RemoveDynamic(this, &AB_Boss::CheckIsMidLife);
		bIsMidLife = true;
		bIsMidLifeAnimPlaying = true;
		
		MakeBossVisible();
		
		if (bIsStunned)
		{
			//Event_AddUnique(onBossStunnedEnd, AB_Boss::StartMidLifeAnim);
			return;
		}

		StartMidLifeAnim();
	}
}

void AB_Boss::StartMidLifeAnim()
{
#if WITH_LOG_STUN_BOSS
	LOG_ERROR("Boss : Start MidLifeAnim");
#endif

	Event_Remove(onBossStunnedEnd, AB_Boss::StartMidLifeAnim);
	Event_AddUnique(bossAnimInstance->OnAnimSpecialEnd(), AB_Boss::EndMidLifeAnim);

	MakeBossVisible();
	
	patternCaster->ClearTimer();
	patternCaster->DestroyEveryPattern();
	timerManager->ClearTimer(stunTimer);

	bossAnimInstance->PlaySpecialAnim(EBossSpecialAnimState::ANGRY_MIDLIFE);

	onBossMidLifeAnimStart.Broadcast();
	blackBoardManager->Broadcast_Boss_OnBossMidLifeAnimStart();
}

void AB_Boss::EndMidLifeAnim(const TEnumAsByte<EBossSpecialAnimState>& _specialAnim)
{
	if (_specialAnim != EBossSpecialAnimState::ANGRY_MIDLIFE)return;

#if WITH_LOG_STUN_BOSS
	LOG_ERROR("Boss : MidLifeAnim Finished");
#endif

	ROG_ERROR(!bossAnimInstance, "AB_Boss::EndMidLifeAnim -> Invalid Anim instance");
	
	Event_Remove(bossAnimInstance->OnAnimSpecialEnd(), AB_Boss::EndMidLifeAnim);

	
	MakeBossVisible();
	
	bossAnimInstance->PlaySpecialAnim(EBossSpecialAnimState::ANGRY_MIDLIFE_END);
	//bossAnimInstance->PlaySpecialAnim(EBossSpecialAnimState::NONE__);

	bIsStunned = true;
	FTimerHandle _timer;
	timerManager->SetTimer(_timer, this, &AB_Boss::UnStunLockAfterTime, 3.0f);
	bIsMidLifeAnimPlaying = false;
	
	blackBoardManager->Broadcast_Boss_OnBossMidLifeAnimEnd();
}

void AB_Boss::PlayDeathAnimation()
{
	ROG_ERROR(!bossAnimInstance, "AB_Boss::PlayDeathAnimation -> Invalid boss Anim instance");
	bossAnimInstance->PlaySpecialAnim(EBossSpecialAnimState::ANGRY_DEAD);
}

void AB_Boss::ChangeAnimWorldSpeed(const float& _worldSpeed)
{
	fWorldSpeed = _worldSpeed;
}

void AB_Boss::UpdateSkeletalAnim(const float& _deltaTime)
{
	skeletalMeshComponent->TickComponent(_deltaTime * fWorldSpeed, ELevelTick::LEVELTICK_All, nullptr);
	if (fWorldSpeed < 1.f)
		blackBoardManager->UPDATE_DEBUG();
}
#pragma endregion Anim