#include "IU_Player.h"

#include "STRUCT_GameInput.h"

#include "M_GameInstance.h"
#include "M_GameMode.h"

#include "IU_CameraManager.h"

#include "C_PlayerAnimInstance.h"
#include "C_Bow.h"

#include "T_ProjectileDetectionSphere.h"

#include "IU_HealthSystemComponent.h"
#include "UC_PlayerAttackComponent.h"
#include "C_SlowMotionComponent.h"
#include "C_RespawnComponent.h"

#include "C_CollectibleComponent.h"
#include "IU_PlayerMovementComponent.h"
#include "IU_PlayerFSMBrain.h"

#include "M_BossManager.h"
#include "B_Boss.h"
#include "M_ArenaManager.h"
#include "A_Arena.h"
#include "A_ArenaBlock.h"

#include "HUD_Main.h"

#include "UI_HitPlayer.h"
#include "UI_PlayerWidget.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "M_SaveManager.h"

#include "DrawDebugHelpers.h"
#include "GPE_Chest.h"
#include "GPE_Elevator.h"
#include "GPE_ElevatorAnimInstance.h"

#include "IU_Macros.h"
#include "LevelLoadingManager.h"
#include "M_LevelStreamerDeluxeEdition.h"
#include "S_GameSave.h"
#include "TriggerBoxLevelLoading.h"
#include "UI_PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"

AIU_Player::AIU_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = capsuleComponent;

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	skeletalMesh->SetupAttachment(capsuleComponent);

	jumpIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JumpIndicatorMesh"));
	jumpIndicatorMesh->SetupAttachment(capsuleComponent);
	
	movementComponent = CreateDefaultSubobject<UIU_PlayerMovementComponent>(TEXT("MovementComponent"));
	AddOwnedComponent(movementComponent);

	movementFsm = CreateDefaultSubobject<UIU_PlayerFSMBrain>(TEXT("MovementFSM"));
	AddOwnedComponent(movementFsm);

	attackComp = CreateDefaultSubobject<UC_PlayerAttackComponent>("attackComp");
	AddOwnedComponent(attackComp);

	healthComponent = CreateDefaultSubobject<UIU_HealthSystemComponent>(TEXT("HealthComponent"));
	AddOwnedComponent(healthComponent);

	slowMoComponent = CreateDefaultSubobject<UC_SlowMotionComponent>(TEXT("SlowMoComponent"));
	AddOwnedComponent(slowMoComponent);

	respawnComp = CreateDefaultSubobject<UC_RespawnComponent>(TEXT("RespawnComp"));
	AddOwnedComponent(respawnComp);

	collectibleComponent = CreateDefaultSubobject<UC_CollectibleComponent>(TEXT("CollectibleComponent"));
	AddOwnedComponent(collectibleComponent);
}

void AIU_Player::BeginPlay()
{
	Super::BeginPlay();

	world = GetWorld();
	InitUI();
	Init();
	InitDifficulty();
	InitEvents();
	RegisterToGameMode();
	InitCameraByType(CAMERA_PLATEFORMING);
	SpawnBow();
}

void AIU_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	JumpMetrics();
	TravelledDistanceMetrics();
	TravelledDistanceTimer();
	msDistanceTimer();
	SetJumpIndicatorPosition();

	currentTimeSinceLastInput += GetWorld()->DeltaTimeSeconds;
	if(currentTimeSinceLastInput > timeBeforeAfkAnimation && !hasDoneAFKAnimation)
	{
		if(animInstance && !attackComp->GetIsOpeningChest())
			animInstance->UpdateOnAFK(true);
		hasDoneAFKAnimation = true;
	}
}

void AIU_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const FGameInput& _input = FGameInput();
	// Simple movement
	InputComponent->BindAxis(_input.VerticalMovementAxis.AxisName, movementComponent, &UIU_PlayerMovementComponent::MoveForward);
	InputComponent->BindAxis(_input.HorizontalMovementAxis.AxisName, movementComponent, &UIU_PlayerMovementComponent::MoveRight);
	
	// First shoot
	InputComponent->BindAction(_input.PrimaryShotAction.ActionName, IE_Pressed, attackComp, &UC_PlayerAttackComponent::StartShooting);
	InputComponent->BindAction(_input.PrimaryShotAction.ActionName, IE_Released, attackComp, &UC_PlayerAttackComponent::CheckHoldingTime);

	// Secondary shoot
	InputComponent->BindAction(_input.SlowMotionAction.ActionName, IE_Pressed, slowMoComponent ,&UC_SlowMotionComponent::ApplySlowMotion);
	//InputComponent->BindAction(_input.SlowMotionAction.ActionName, IE_Released, slowMoComponent, &UC_SlowMotionComponent::StopSlowMotion);

	// Interraction
	InputComponent->BindAction(_input.InteractionAction.ActionName, IE_Pressed, this, &AIU_Player::BroadcastInteract);

	// Bind specials inputs
	InputComponent->BindAction(_input.CheatGodModeAction.ActionName, IE_Pressed, healthComponent, &UIU_HealthSystemComponent::ToggleGodMode);
	InputComponent->BindAction(_input.CheatSkipTutorialAction.ActionName, IE_Pressed, this, &AIU_Player::ToggleFreePass);
	InputComponent->BindAction(_input.CheatBossStartAction.ActionName, IE_Pressed, this, &AIU_Player::StartBoss);
	InputComponent->BindAction(_input.CheatBossKillAction.ActionName, IE_Pressed, this, &AIU_Player::KillBoss);
	
	DECLARE_DELEGATE_OneParam(FOnTeleportToArena, const int);
	InputComponent->BindAction<FOnTeleportToArena>(_input.CheatTPArena1Action.ActionName, IE_Pressed, this, &AIU_Player::TeleportInFrontOfArena, 1);
	InputComponent->BindAction<FOnTeleportToArena>(_input.CheatTPArena2Action.ActionName, IE_Pressed, this, &AIU_Player::TeleportInFrontOfArena, 2);
	InputComponent->BindAction<FOnTeleportToArena>(_input.CheatTPArena3Action.ActionName, IE_Pressed, this, &AIU_Player::TeleportInFrontOfArena, 3);

	//Animation
	InputComponent->BindAction(_input.AnyKeyAction.ActionName,IE_Pressed,this ,&AIU_Player::ResetAfkTimeForAnimation);
	InputComponent->BindAction(_input.AnyKeyAction.ActionName,IE_Repeat,this ,&AIU_Player::ResetAfkTimeForAnimation);
}

void AIU_Player::Hit(float _null)
{
	iHitAmount++;
	iHitAmount = iHitAmount > iMaxHitAmount ? iMaxHitAmount : iHitAmount;
	OnPlayerHit(iHitAmount);

	if (TimerManager)
		TimerManager->SetTimer(TimerHit, this, &AIU_Player::ResetHit, fHitDecayTime);
}
void AIU_Player::ResetHit()
{
	iHitAmount = 0;
}

void AIU_Player::ForceResetHit()
{
	ResetHit();
	OnPlayerHit(iHitAmount);
}

#pragma region Init

void AIU_Player::Init()
{
	UM_SaveManager::Load(playerDatas, Save_Player, GetName());
	
	gameInstance = GetGameInstance<UM_GameInstance>();
	gameSave = gameInstance->GetCurrentGameSave();
	
	if (world)
	{
		gameMode = world->GetAuthGameMode<AM_GameMode>();
		TimerManager = &world->GetTimerManager();
	}
	
	if (skeletalMesh)
	{
		animInstance = Cast<UC_PlayerAnimInstance>(skeletalMesh->GetAnimInstance());
	}
	
	rangeMeshIndicator = jumpIndicatorMesh->GetRelativeLocation();

	if (gameSave)
	{
		healthComponent->InitializeHealth(gameSave->GetExtraLife());
		attackComp->AddSlowMowMaxCount(gameSave->GetExtraSlowMo());	
	}
	
	if (!gameSave || !gameSave->GetProgressDone())
	{
		gameMode->GetLevelLoadingManager()->AddLevels(gameMode->GetLevelLoadingManager()->LevelsToLoadOnStart(),{});
		return;
	}
	LoadLevelOnReloadSave();	
}

void AIU_Player::InitUI()
{
	hud = world->GetFirstPlayerController()->GetHUD<AHUD_Game>();
	if (!hud) return;
	
	pauseWidget = hud->GetWidget<UUI_PauseMenuWidget>(WIDGET_PAUSE);
	playerWidget = hud->GetWidget<UUI_PlayerWidget>(WIDGET_PLAYER);

	if (playerWidget)
	{
		if (attackComp)
			playerWidget->InitializePlayerAttackUI(attackComp);
	
		if (healthComponent)
			playerWidget->InitializePlayerLifeUI(healthComponent);

		if (collectibleComponent)
			playerWidget->InitializePlayerCollectibleUI(collectibleComponent);
	}
}

void AIU_Player::InitEvents()
{
	if (gameInstance)
	{
		// gameInstance->OnLevelWillChange()->AddDynamic(this, &AIU_Player::SaveDatas);
	}
	
	if (movementComponent)
	{
		movementComponent->OnJump().AddDynamic(this, &AIU_Player::ChangeIndicatorState);
		movementComponent->OnHitGround().AddDynamic(this, &AIU_Player::ChangeIndicatorState);
		movementComponent->OnLeaveGround().AddDynamic(this, &AIU_Player::ChangeIndicatorState);
	}

	if (healthComponent)
	{
		healthComponent->OnTakeDamage()->AddUniqueDynamic(this, &AIU_Player::Hit);
		healthComponent->OnDeath()->AddUniqueDynamic(this, &AIU_Player::ForceResetHit);
		
		healthComponent->OnTakeDamage()->AddDynamic(this, &AIU_Player::NiagaraCharaHit);
		healthComponent->OnDeath()->AddDynamic(respawnComp, &UC_RespawnComponent::ToDeath);
	}

	//if (respawnComp)
	//{
	//	respawnComp->OnRespawn().AddUniqueDynamic(animInstance, &UC_PlayerAnimInstance::ResetDeadStatus);
	//}
}

void AIU_Player::RegisterToGameMode()
{
	if (gameMode)
	{
		if (AM_GameLogic* _gameLogic = gameMode->GetGameLogic())
		{
			_gameLogic->SetPlayer(this);
			_gameLogic->OnBossRegistered()->AddDynamic(this, &AIU_Player::InitBoss);
		}
		
		else
		{
			LOG("AIU_Player::RegisterToGameMode -> GameLogic not valid");
		}
	}
}

void AIU_Player::InitBoss(AB_Boss* _boss)
{
	_boss->OnBossActivated().AddUniqueDynamic(this, &AIU_Player::SetIsIntoArena);
	_boss->OnBossDead().AddUniqueDynamic(this, &AIU_Player::SetIsOutFromArena);
	_boss->OnBossReset().AddUniqueDynamic(this, &AIU_Player::SetIsOutFromArena);

	InitBossDeath(_boss);
}

void AIU_Player::InitBossDeath(AB_Boss* _boss)
{
	if (!_boss || !healthComponent) return;
	
	if (UIU_HealthSystemComponent* _bossHealth = _boss->GetHealthComponent())
	{
		_bossHealth->OnDeath()->AddDynamic(healthComponent, &UIU_HealthSystemComponent::ResetHealth);
	}
}

void AIU_Player::SpawnBow()
{
	AC_Bow* _bow = GetWorld()->SpawnActor<AC_Bow>(bowToSpawn,GetActorLocation(),GetActorRotation());
	if (!_bow) return;

	const FName& bowSocket = "bowSocket";
	_bow->GetBowSkeletalMesh()->AttachToComponent(skeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, bowSocket);

	currentBow = _bow;
	
}

#pragma endregion

#pragma region Debugs

void AIU_Player::JumpMetrics()
{
	if(!useJumpMetricsDebug)return;
	
	const FVector& _playerLocation = GetActorLocation();
	
	if(isPersistent)
		DrawDebugLine(world, _playerLocation, _playerLocation, FColor::Blue, true, 5, 0, 5);
	else
		DrawDebugLine(world, _playerLocation, _playerLocation, FColor::Blue, false, debugStayTime, 0, 5);


	FHitResult _outHit;
	if(!movementComponent->CheckIsGrounded(_outHit))
	{
		if(!isStartVectorSaved)
		{
			startDistanceVector = _playerLocation;
			startDistanceVector.Z += jumpDistanceOffset;
			isStartVectorSaved = true;
		}
		const FVector& _currentHeight = _playerLocation;

		if(_currentHeight.Z < maxHeightVector.Z)
		{
			ShowMaxHeight(maxHeightVector);
			isMaxHeight = true;
		}
		maxHeightVector = _currentHeight;
		
	}

	if(movementComponent->CheckIsGrounded(_outHit))
	{
		if(!isEndvectorSaved && isStartVectorSaved)
		{
			endDistanceVector = _playerLocation;
			endDistanceVector.Z += + jumpDistanceOffset;
			isEndvectorSaved = true;
		}
		isMaxHeight = false;
	}

	if(isEndvectorSaved && isStartVectorSaved)
	{
		ShowMaxLength();
		isStartVectorSaved = false;
		isEndvectorSaved = false;
		maxHeightVector = FVector::ZeroVector;
	}
}

void AIU_Player::TravelledDistanceMetrics()
{
	if(!calculateTravelledDistance)return;

	const FVector& _playerLocation = GetActorLocation();
	const FVector& _momentum = movementComponent->Momentum();
	
	if(startTravelledDistanceVector == FVector::ZeroVector)
		startTravelledDistanceVector = _playerLocation;

	if(_momentum != FVector::ZeroVector)
	{
		endTravelledDistanceVector = _playerLocation;
		totalTravelled += FVector::Dist(startTravelledDistanceVector,endTravelledDistanceVector);

		if(isPersistent)
			DrawDebugLine(world, startTravelledDistanceVector, endTravelledDistanceVector, FColor::Yellow,true,5,0,5);
		else if(!isPersistent)
			DrawDebugLine(world, startTravelledDistanceVector, endTravelledDistanceVector, FColor::Yellow,false,debugStayTime,0,5);
		
		startTravelledDistanceVector = endTravelledDistanceVector;
	}

	if(_momentum.Size() < 0.1f)
	{
		endTravelledDistanceVector = _playerLocation;
		totalTravelled += FVector::Dist(startTravelledDistanceVector,endTravelledDistanceVector);

		if(isPersistent)
			DrawDebugLine(world, startTravelledDistanceVector, endTravelledDistanceVector, FColor::Blue,true,5,0,5);
		else if(!isPersistent)
			DrawDebugLine(world, startTravelledDistanceVector, endTravelledDistanceVector, FColor::Blue,false,debugStayTime,0,5);

		if(totalTravelled > 1.0f)
		{
			UE_LOG(LogTemp,Warning,TEXT("Distance Travelled : %f in %f seconds"),totalTravelled,msTimer);
			UE_LOG(LogTemp,Warning,TEXT(" -->  %f m/s"),totalTravelled/msTimer);
			
			totalTravelled = 0.0f;
			msTimer = 0.0f;
		}
	}
}

void AIU_Player::ShowMaxHeight(const FVector& maxHeight)
{
	if (isMaxHeight)return;
	const FVector& _start = FVector(maxHeight.X, maxHeight.Y, 0.0f);
	
	if (isPersistent)
	{
		DrawDebugLine(world, _start,maxHeight, FColor::Red,true,5,0,5);
	}
	
	else
	{
		DrawDebugLine(world, _start,maxHeight, FColor::Red,false,debugStayTime,0,5);
	}
	    
	UE_LOG(LogTemp,Warning,TEXT("Max Height Reached : %f"),(maxHeight.Z - startDistanceVector.Z));
}

void AIU_Player::ShowMaxLength()
{
	if (isPersistent)
	{
		DrawDebugLine(world, startDistanceVector, endDistanceVector,FColor::Red,true,5,0,5);
	}
	else
	{
		DrawDebugLine(world, startDistanceVector, endDistanceVector,FColor::Red,false,debugStayTime,0,5);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Jump Distance : %f"), FVector::Dist(startDistanceVector,endDistanceVector));
}

void AIU_Player::TravelledDistanceTimer()
{
	if (!calculateTravelledDistance)return;
	
	if (world->DeltaTimeSeconds > distancePrecision)
	{
		TravelledDistanceMetrics();
	}
}

void AIU_Player::msDistanceTimer()
{
	if (!calculateTravelledDistance)return;
	
	if (movementComponent->Momentum().Size() < 0.1f) return;
	
	msTimer += world->DeltaTimeSeconds;	
}

void AIU_Player::ClearDebugs()
{
	FlushPersistentDebugLines(GetWorld());
}

void AIU_Player::StartBoss()
{
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if (const AM_BossManager* _bossManager = _gameMode->GetBossManager())
		{
			if (const AA_Arena* _arena = _bossManager->GetCurrentArena())
			{
				if (AA_ArenaBlock* _arenaBlock = _arena->GetArenaBlock())
				{
					InitCameraByType(ECameraType::CAMERA_BOSS);
					_arenaBlock->BroadcastArenaBlockTrigger();
				}
			}
		}
	}
}

void AIU_Player::KillBoss()
{
	if (!bIsInArena) return;
	
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if (const AM_BossManager* _bossManager = _gameMode->GetBossManager())
		{
			if (const AB_Boss* _boss = _bossManager->GetCurrentBoss())
			{
				_boss->GetHealthComponent()->TakeDamageWithImpactLocation(1000000000000, true, _boss->GetActorLocation());
			}
		}
	}
}

#pragma endregion 

#pragma region Niagara

void AIU_Player::NiagaraCharaHit(float _damage)
{
	if (UNiagaraSystem* _niagaraSystem = niagraComps.niagaraCharaHit)
	{
		SpawnNiagaraSystem(_niagaraSystem);
	}
}

void AIU_Player::NiagaraDeathFall(float _damage, float _actualHealth)
{
	if (UNiagaraSystem* _niagaraSystem = niagraComps.niagaraDeathFall)
	{
		SpawnNiagaraSystem(_niagaraSystem);
	}
}

#pragma endregion

#pragma region Jump

void AIU_Player::SetJumpIndicatorPosition()
{
	const FVector& _location = GetActorLocation();
	const ECollisionChannel& _traceChannel = capsuleComponent->GetCollisionObjectType();
	FHitResult _result;
	const FVector& _offset = GetActorForwardVector() * rangeMeshIndicator.X + GetActorRightVector() * rangeMeshIndicator.Y + GetActorUpVector() * rangeMeshIndicator.Z;
	world->LineTraceSingleByChannel(_result,
									_location,
									_location + _offset,
									_traceChannel);
	if (_result.bBlockingHit)
	{
		jumpIndicatorMesh->SetWorldLocation(_result.ImpactPoint + FVector::UpVector * meshIndicatorOffset);
		jumpIndicatorMesh->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(_result.ImpactNormal));
		
	}
	
	else
	{
		jumpIndicatorMesh->SetWorldLocation(FVector::DownVector * 100000.0f);
	}
}

void AIU_Player::ChangeIndicatorState()
{
	if (!jumpIndicatorMesh) return;
	
	if (movementFsm->CurrentState()->StateType() != Moving)
	{
		jumpIndicatorMesh->SetVisibility(false);
	}
	else
	{
		jumpIndicatorMesh->SetVisibility(true);
	}
}

#pragma endregion

#pragma region Difficulty

void AIU_Player::InitDifficulty()
{
	if (gameInstance)
	{
		if (UM_DifficultyManager* _difficultyManager = gameInstance->GetDifficultyManager())
		{
			_difficultyManager->OnDifficultyChanged().AddDynamic(this, &AIU_Player::UpdateOnDifficultyChanged);
			currentDifficulty = _difficultyManager->GetCurrentDifficulty();
			
			FDifficultyData _data;
			if (_difficultyManager->GetCurrentDifficultyData(_data))
				UpdateOnDifficultyChanged(currentDifficulty, _data);
		}
	}
}

void AIU_Player::UpdateOnDifficultyChanged(EDifficultyType _difficulty, const FDifficultyData& _difficultyData)
{
	currentDifficulty = _difficulty;
	if (playerWidget)
	{
		if (currentDifficulty == EDifficultyType::DIFFICULTY_ELITE)
			playerWidget->HideMaxLifeBar();
		else
			playerWidget->ShowMaxLifeBar();
	}
			
	if (healthComponent)
	{
		healthComponent->SetCanRegen(_difficultyData.bCanRegen);
		healthComponent->SetRegenDatas(_difficultyData.fRegenCooldown, _difficultyData.fRegenValue, _difficultyData.fMaxLifeToRemove);
		healthComponent->SetDamageFromProjectile(_difficultyData.fProjectileExtraDamage);
		healthComponent->SetDamageFromZone(_difficultyData.fZoneExtraDamage);
		healthComponent->SetFallDamage(_difficultyData.fFallDamage);

		HealthComponent()->ResetHealthForced();
	}
	//Doesn't check is in 'Level UI' (player should NEVER be in the 'Level UI'
	if (bAlreadyInit && respawnComp)
	{
		respawnComp->Death(true);
		return;
	}
	
	bAlreadyInit = true;
}

#pragma endregion

#pragma region Tutorial

void AIU_Player::ApplySlow(float _slowFactor) const
{
	if (movementComponent)
	{
		movementComponent->SetSlowFactor(_slowFactor);
	}
}

#pragma endregion

#pragma region Status

void AIU_Player::SetPlayerStatus(bool _status, bool _isDead)
{
	SetPlayerInputs(_status);

	if (attackComp)
	{		
		attackComp->SetIsDead(!_status);
	}
	
	if (gameMode)
	{
		const float& _capsuleHeight = capsuleComponent->GetScaledCapsuleHalfHeight();
		const FVector& _location = gameMode->GetRespawnLocation(_isDead);
		const FRotator& _rotation = gameMode->GetRespawnRotation(_isDead);
		gameMode->SetRespawnPoint(false, _location, _rotation);

		if (_status)
		{
			// Set new location and rotation
			SetActorLocationAndRotation(_location + FVector::UpVector * _capsuleHeight, _rotation);
			if (const AIU_CameraManager* _cameraManager = gameMode->GetCameraManager())
			{
				if (AIU_Camera* _camera = _cameraManager->GetCamera())
				{				
					if (_status)
					{
						_camera->ResetCamera(_location, _rotation.Euler().Z);
					}
				}
			}			
			ResetPlayerStatus(_rotation.Vector().GetClampedToMaxSize(1.0f));
		}
	}
	if (healthComponent)
	{
		healthComponent->SetCanTakeDamages(_status);
	}
}

void AIU_Player::SetPlayerInputs(bool _status) const
{
	if (movementComponent)
	{
		movementComponent->SetCanMove(_status);
	}
	if (movementFsm)
	{
		movementFsm->JumpingState()->SetCanJumpStatus(_status);
		movementFsm->DashingState()->SetCanDash(_status);
	}

	if (attackComp)
	{
		attackComp->SetShootInput(_status);
	}
	if (const AIU_CameraManager* _cameraManager = gameMode->GetCameraManager())
	{
		if (AIU_Camera* _camera = _cameraManager->GetCamera())
		{
			_camera->SetCanMove(_status);
		}
	}
}

void AIU_Player::ResetPlayerStatus(const FVector& _direction) const
{
	if (movementComponent)
	{
		movementComponent->SetMomentum(FVector(_direction));
	}

	if (animInstance)
	{
		animInstance->ResetDeadStatus();
	}
	
	if (UNiagaraSystem* _niagaraSystem = niagraComps.niagaraDeath)
	{
		SpawnNiagaraSystem(_niagaraSystem);
	}
}

#pragma endregion

#pragma region Publics
void AIU_Player::ShowArrow()
{
	if(currentBow)
	{
		currentBow->SetActorHiddenInGame(true);
		currentBow->SetActorEnableCollision(false);
	}
	
	if(!currentArrow)
	{
	AActor* _arrow = GetWorld()->SpawnActor<AActor>(arrowToSpawn,GetActorLocation(),GetActorRotation());
	if (!_arrow) return;

	const FName& bowSocket = "arrowSocket";
	_arrow->AttachToComponent(skeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, bowSocket);

	currentArrow = _arrow;
	}

	else if(currentArrow)
	{
		currentArrow->SetActorHiddenInGame(false);
		currentArrow->SetActorEnableCollision(true);
	}
}

void AIU_Player::HideArrow()
{
	if(currentArrow)
	{
		currentArrow->SetActorHiddenInGame(true);
		currentArrow->SetActorEnableCollision(false);
	}	
}

void AIU_Player::ShowBow()
{
	if(currentBow)
	{
		currentBow->SetActorHiddenInGame(false);
		currentBow->SetActorEnableCollision(true);
	}
}

void AIU_Player::HideBow()
{
	if (currentBow)
	{
		currentBow->SetActorHiddenInGame(true);
		currentBow->SetActorEnableCollision(false);
	}
}

void AIU_Player::AddExtraLife(float _extraLife)
{
	if (gameSave) gameSave->SetExtraLife(gameSave->GetExtraLife() + _extraLife);
	healthComponent->UpdateMaxHealth(_extraLife);
	healthComponent->ResetHealthForced();
}
void AIU_Player::AddSlowMotion(int _extraSlowMo)
{
	if (gameSave) gameSave->SetExtraSlowMo(gameSave->GetExtraSlowMo() + _extraSlowMo);
	attackComp->AddSlowMowMaxCount(_extraSlowMo);	
}

void AIU_Player::SetSimulatePhysics(bool _status) const
{
	if (capsuleComponent)
	{
		FBodyInstance* _bodyInstance = capsuleComponent->GetBodyInstance();
		if (!_bodyInstance->bSimulatePhysics && _status)
		{
			_bodyInstance->bSimulatePhysics = _status;
			capsuleComponent->SetPhysicsLinearVelocity(FVector(0));
		}
		else
			_bodyInstance->bSimulatePhysics = _status;
	}
}

void AIU_Player::InitCameraByType(const TEnumAsByte<ECameraType>& _cameraType)
{
	if (!gameMode) return;
	
	if (AIU_CameraManager* _cameraManager = gameMode->GetCameraManager())
	{
		const FGameInput& _input = FGameInput();
		
		if (AIU_Camera* _currentCamera = _cameraManager->GetCamera())
		{
			InputComponent->RemoveAxisBinding(_input.CameraYawKeyboardAxis.AxisName);
			InputComponent->RemoveAxisBinding(_input.CameraPitchKeyboardAxis.AxisName);
			InputComponent->RemoveAxisBinding(_input.CameraYawGamepadAxis.AxisName);
			InputComponent->RemoveAxisBinding(_input.CameraPitchGamepadAxis.AxisName);
			_currentCamera->SetActorTickEnabled(false);
		}

		AIU_Camera* _newCam = _cameraManager->GetOrCreateCameraByType(_cameraType);
		if (!_newCam || !movementComponent || !attackComp) return;
	
		_newCam->SetActorTickEnabled(true);
		movementComponent->SetPlayerCamera(_newCam);
		attackComp->SetPlayerCamera(_newCam);
		_newCam->SetTarget(this);
		_newCam->SetActorLocation(GetActorLocation());
	
		InputComponent->BindAxis(_input.CameraYawKeyboardAxis.AxisName, _newCam, &AIU_Camera::MoveYaw);
		InputComponent->BindAxis(_input.CameraPitchKeyboardAxis.AxisName, _newCam, &AIU_Camera::MovePitch);
		InputComponent->BindAxis(_input.CameraYawGamepadAxis.AxisName, _newCam, &AIU_Camera::MoveYawController);
		InputComponent->BindAxis(_input.CameraPitchGamepadAxis.AxisName, _newCam, &AIU_Camera::MovePitchController);
		if (camera)
		{
			_newCam->SetPitch(camera->GetPitch());
			_newCam->SetYaw(camera->GetYaw());			
		}
		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(_newCam);
		camera = _newCam;
		
	}
}
#pragma endregion 

void AIU_Player::ResetAfkTimeForAnimation()
{
	
	currentTimeSinceLastInput = 0;
	hasDoneAFKAnimation = false;
	if(animInstance)
		animInstance->UpdateOnAFK(false);
}

void AIU_Player::TryInteract()
{
	if (bInteracting || movementFsm->CurrentState()->StateType() != Moving) return;
	
	const FVector& _actorLoc = GetActorLocation();
	FHitResult _outHit;

	ROG_ERROR (interactionLayer.Num() < 1, "AIU_Player::TryInteract -> There is no layer set in player interaction");
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(world, _actorLoc, _actorLoc, interactionRange, interactionLayer, false, { }, EDrawDebugTrace::None, _outHit, true))
	{		
		if (AGPE_Chest* _chest = Cast<AGPE_Chest>(_outHit.GetActor()))
		{

			const FVector& _targetLocation = _outHit.GetComponent()->GetComponentLocation();
			const FVector& _location = GetActorLocation();
			const FVector& _locationZ = FVector(_targetLocation.X, _targetLocation.Y, _location.Z);
			const FRotator& _rotation = Get_LookRotation(_locationZ - _location);
			SetActorRotation(_rotation);

			attackComp->CheckHoldingTime();

			if(!_chest->GetCanInteract())return;
			StartInteraction();
			_chest->SetCanInteract(false);
			_chest->StartChestMiniGame();
			animInstance->UpdateIsOpeningChest(true);
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(1,0,3,FColor::Black);
		
			attackComp->CheckHoldingTime();
			movementFsm->SwitchCurrentState(EStateType::Falling);
			
			return;
		}
		
		//Elevator
		if(AGPE_Elevator* _elevator = Cast<AGPE_Elevator>(_outHit.GetActor()))
		{

			if (!_elevator->GetCanBeUsed()) return;


			StartInteraction();
			_elevator->OnInteracted().Broadcast();

			const FVector& _targetLocation = _outHit.GetComponent()->GetComponentLocation();
			const FVector& _location = GetActorLocation();
			const FVector& _locationZ = FVector(_targetLocation.X, _targetLocation.Y, _location.Z);
			const FRotator& _rotation = Get_LookRotation(_locationZ - _location);
			SetActorRotation(_rotation);
		
			attackComp->CheckHoldingTime();

			if (_elevator->GetUseCustomPlayRate())
				fNewAnimPlayRate = _elevator->GetCustomPlayRate();
			else
				fNewAnimPlayRate = 1.0f;

			const FRotator& _Rot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(),_elevator->GetAnimLocation()->GetComponentLocation());
			const FRotator& _goodRot = FRotator(GetActorRotation().Pitch,_Rot.Yaw,_Rot.Roll);
			SetActorRotation(_goodRot);
			_elevator->OnArrived().AddUniqueDynamic(animInstance,&UC_PlayerAnimInstance::StopIsActivatingEscalator);
			_elevator->OnArrived().AddUniqueDynamic(this, &AIU_Player::StopInteraction);
			animInstance->SetIsActivatingElevator();

			
			movementFsm->SwitchCurrentState(EStateType::Falling);
			
			return;
		}

	}
}
void AIU_Player::StartInteraction()
{
	SetInteractingState(true);
	if (pauseWidget) pauseWidget->LockButtons();
}
void AIU_Player::StopInteraction()
{
	SetInteractingState(false);
	if (pauseWidget) pauseWidget->UnlockButtons();
}

void AIU_Player::BroadcastInteract()
{
	onInterract.Broadcast();
	TryInteract();
}

void AIU_Player::LoadLevelOnReloadSave()
{
	if (!gameSave || !gameSave->GetSaveTrigger() || !gameMode || !gameMode->GetLevelLoadingManager() || !hud)return;
	TArray<TSoftObjectPtr<ATriggerBoxLevelLoading>> _lvlStreamers = gameSave->GetSaveTrigger()->CorrespondingLevelStreamer();
	const int& _maxStreamers = _lvlStreamers.Num();
	gameMode->GetLevelLoadingManager()->OnFinishedLoading().AddUniqueDynamic(this,&AIU_Player::SetPlayerAfterLoad);
	hud->ShowLoadScreen();
	for (int i = 0;i< _maxStreamers;i++)
	{
		if (!_lvlStreamers[i].IsValid())continue;
		_lvlStreamers[i].Get()->AddLevels();
	}
}

void AIU_Player::SetPlayerAfterLoad()
{
	if (!gameSave)return;
	gameMode->GetLevelLoadingManager()->OnFinishedLoading().RemoveDynamic(this,&AIU_Player::SetPlayerAfterLoad);
	const FVector& _pos = gameSave->GetPlayerStartingTransform().GetLocation();
	const FRotator& _rot = gameSave->GetPlayerStartingTransform().GetRotation().Rotator();

	if (const AGPE_SaveTrigger* _saveTrigger = gameSave->GetSaveTrigger())
	{
		if (AM_LevelThreeTriggerCinematic* _trigger = _saveTrigger->GetCinematicTrigger())
		{
			_trigger->RotateDoor();
		}
		if (AGPE_Elevator* _elevator = _saveTrigger->GetElevatorToActivate())
		{
			_elevator->InstantActivateOnLoad();
		}
	}
	
	SetActorLocationAndRotation(_pos,_rot);
	if (gameMode)
		gameMode->SetRespawnPoint(true,_pos,_rot);
	hud->HideLoadScreen();
}
