#include "T_TutoLogic.h"

#include "M_GameMode.h"
#include "M_CinematicManager.h"
#include "C_RespawnComponent.h"
#include "IU_Player.h"
#include "T_BossTuto.h"
#include "T_BossTutoBlock.h"
#include "T_ProjectileDetectionSphere.h"
#include "T_ProjectileGuidedTuto.h"
#include "C_ThornPowerOrb.h"
#include "C_HarpoonPowerOrb.h"
#include "C_SlowMotionComponent.h"
#include "IU_PlayerMovementComponent.h"
#include "M_GameInstance.h"
#include "M_WorldTimeManager.h"
#include "TimerManager.h"
#include "UC_PlayerAttackComponent.h"
#include "IU_CameraManager.h"
#include "STRUCT_CustomMath.h"
#include "Kismet/KismetMathLibrary.h"
#include "M_ProjectileManager.h"

#include "UI_TutoWidget.h"
#include "UI_TutoElementWidget.h"

#if WITH_EDITOR
#include "IU_Macros.h"
#endif

AT_TutoLogic::AT_TutoLogic()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AT_TutoLogic::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle _handle;
	GetWorld()->GetTimerManager().SetTimer(_handle,this,&AT_TutoLogic::Init,0.5f,false);
}

void AT_TutoLogic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(canBeginBossActivationTimer)
		CheckForBossActivationTime();
	if (bStartedTuto)
		Behaviour(currentPhase,DeltaTime * fCurrentWorldSpeed);
}

void AT_TutoLogic::Init()
{
	gameMode = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
	if(!gameMode)return;
	gameMode->SetTutoLogic(this);
	gameMode->GetWorldTimeManager()->OnWorldSpeedChange().AddUniqueDynamic(this,&AT_TutoLogic::SetCurrentWorldSpeed);
	refPlayer = gameMode->GetPlayer();
	cinematicManager = gameMode->GetCinematicManager();
	hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Game>();

	if (hud)
		tutoWidget = hud->GetWidget<UUI_TutoWidget>(WIDGET_TUTO);
}

void AT_TutoLogic::CheckForBossActivationTime()
{
	currentTimerForBossActivation += GetWorld()->DeltaTimeSeconds;
	if(currentTimerForBossActivation > timeBeforeFirstBossActivation)
	{
		currentTimerForBossActivation = 0;
		canBeginBossActivationTimer = false;
		//SwitchPhase(BossShootingDodge);
		
	}
}

void AT_TutoLogic::SwitchPhase(EPhaseTuto _newPhase)
{
	currentPhase = _newPhase;
	FTimerHandle _handle;
	switch (currentPhase)
	{
		//PHASE 1
	case NotStarted :
		break;
	case CinematicEnter :
		refPlayer->DisableInput(GetWorld()->GetFirstPlayerController());
		cinematicManager->OnCinematicFinishedEvent().AddUniqueDynamic(this,&AT_TutoLogic::EndStartCinematic);
		if (cinematicManager->CanPlayCinematic())
			cinematicManager->PlayTutoCinematic();
		else
		{

			SwitchPhase(Ended);
			EndTuto();
		}
		break;
	case PlayerThreeShoot :	
		currentPhaseNumber = 1;
		refBoss->HealthSystemComponent()->SetCanTakeDamages(true);
		//refPlayer->HealthComponent()->OnDeath()->AddUniqueDynamic(this, &AT_TutoLogic::ResetPhaseIfTutoDeath);
		refPlayer->RespawnComponent()->OnRespawn().AddUniqueDynamic(this, &AT_TutoLogic::ResetPhaseIfTutoDeath);
		tutoWidget->ClearTutoWidget();
		currentCountElementWidget = Cast<UUI_TutoCountElementWidget>(tutoWidget->InitializeTutoWidget(iterationThreeShootCountWidget));
		//iterationThreeShootCountWidget->GetDefaultObject<UUI_TutoCountElementWidget>()->UpdateCurrentCount(refPlayer->GetCharacterAttackComponent()->GetCurrentTutoIterationNumber());
		refPlayer->EnableInput(GetWorld()->GetFirstPlayerController());
		
		break;
	case HarpoonOnGround :

		tutoWidget->ClearTutoWidget();
		tutoWidget->InitializeTutoWidget(harpoonWidget);

		currentCountElementWidget->ResetCurrentCount();
		if(harpoonPowerOrb)
		{
			FVector _spawnLocation = refPlayer->GetActorLocation() + (refPlayer->GetActorForwardVector() * orbForwardSpawnOffset);
			_spawnLocation.Z = refBoss->GetActorLocation().Z + 80.0f;
			AC_HarpoonPowerOrb* _myHarpoonOrb = GetWorld()->SpawnActor<AC_HarpoonPowerOrb>(harpoonPowerOrb,
																							_spawnLocation,
																							refPlayer->GetActorRotation());
			
			if(!_myHarpoonOrb)return;
			_myHarpoonOrb->SetHaveALifeSpan(false);
			refPlayer->GetCharacterAttackComponent()->FOnPowerOrbCollected().AddUniqueDynamic(this,&AT_TutoLogic::CollectedHarpoonOrb);
		}
		break;
	case ShootUntilHarpoonStun :
		tutoWidget->ClearTutoWidget();
		currentCountElementWidget = Cast<UUI_TutoCountElementWidget>(tutoWidget->InitializeTutoWidget(iterationStunCountWidget));
		currentCountElementWidget->ResetCurrentCount();
		refBoss->HealthSystemComponent()->OnTakeDamage()->AddUniqueDynamic(this,&AT_TutoLogic::HitHarpoonStun);
		break;
	case DashCancelShoot :
		tutoWidget->ClearTutoWidget();
		currentCountElementWidget = Cast<UUI_TutoCountElementWidget>(tutoWidget->InitializeTutoWidget(iterationDashCancelShootCountWidget));
		currentCountElementWidget->ResetCurrentCount();
		refPlayer->GetCharacterAttackComponent()->ResetShootState();

		break;
	case SelfGuidedOnGround :
		tutoWidget->ClearTutoWidget();
		tutoWidget->InitializeTutoWidget(doubleGuidedWidget);
		if(doubleGuidedowerOrb)
		{
			FVector _spawnLocation = refPlayer->GetActorLocation() + (refPlayer->GetActorForwardVector() * orbForwardSpawnOffset);
			_spawnLocation.Z = refBoss->GetActorLocation().Z + 80.0f;
			AC_ThornPowerOrb* _myDoubleGuidedOrb = GetWorld()->SpawnActor<AC_ThornPowerOrb>(doubleGuidedowerOrb,
																							_spawnLocation,
																							refPlayer->GetActorRotation());
			_myDoubleGuidedOrb->SetHaveALifeSpan(false);
			refPlayer->GetCharacterAttackComponent()->FOnPowerOrbCollected().AddUniqueDynamic(this,&AT_TutoLogic::CollectedGuidedOrb);
		}
		break;
	case PlayerShootSelfGuided :
		tutoWidget->ClearTutoWidget();
		currentCountElementWidget = Cast<UUI_TutoCountElementWidget>(tutoWidget->InitializeTutoWidget(iterationDoubleGuidedHitCountWidget));
		currentCountElementWidget->ResetCurrentCount();
		refBoss->HealthSystemComponent()->OnTakeDamage()->AddUniqueDynamic(this,&AT_TutoLogic::HitGuidedTrial);
		refPlayer->MovementComponent()->SetHasDashed(false);
		break;

		//PHASE 2
	case DashThroughProjectileWall :
		currentPhaseNumber = 2;
		tutoWidget->ClearTutoWidget();
		refPlayer->GetCharacterAttackComponent()->ResetShootState();
		currentCountElementWidget = Cast<UUI_TutoCountElementWidget>(tutoWidget->InitializeTutoWidget(iterationDashThroughWallCountWidget));
		currentCountElementWidget->ResetCurrentCount();
		//refBoss->SetBossShootDelay(5.0f);
		
		break;
	case SlowMoDodgePattern :
	/*	tutoWidget->ClearTutoWidget();
		currentCountElementWidget = Cast<UUI_TutoCountElementWidget>(tutoWidget->InitializeTutoWidget(iterationSlowMoCountWidget));
		currentCountElementWidget->ResetCurrentCount();*/
		//DestroyTutoBossProjectiles();
		refPlayer->GetCharacterAttackComponent()->SetSlowMoCountToMax();
		refPlayer->GetSlowMotionComponent()->SetOrbConsumptionPerSecond(0);
		
		
		break;

		//PHASE 3
	case BossFight :
		currentPhaseNumber = 3;
		refPlayer->GetCharacterAttackComponent()->SetSlowMoAtQuarter();
		refPlayer->GetSlowMotionComponent()->ResetOrbConsumptionPerSecond();
		refBoss->HealthSystemComponent()->OnDeath()->AddUniqueDynamic(this,&AT_TutoLogic::SwitchPhaseToEndTuto);
		refBoss->HealthSystemComponent()->SetMaxHealth(tutoBossLife);
		refBoss->HealthSystemComponent()->ResetHealth();
		//refBoss->ResetShootDelay();
		
		if (UUI_TutoBossLifeWidget* _widget = Cast<UUI_TutoBossLifeWidget>(tutoWidget->InitializeTutoWidget(shotOnBossWidget)))
		{
			_widget->InitializeTutoBossLifeWidget(refBoss->HealthSystemComponent());
		}
		
		break;
	case Ended :
		tutoWidget->ClearTutoWidget();
		//refPlayer->HealthComponent()->OnDeath()->RemoveDynamic(this, &AT_TutoLogic::ResetPhaseIfTutoDeath);
		refPlayer->RespawnComponent()->OnRespawn().RemoveDynamic(this, &AT_TutoLogic::ResetPhaseIfTutoDeath);
		refPlayer->GetCharacterAttackComponent()->ResetShootState();
		tutoWidget->InitializeTutoWidget(endTutoWidget);
		refPlayer->GetCharacterAttackComponent()->SetSlowMoCountToZero();
		
		DestroyTutoBossProjectiles();

		refPlayer->DisableInput(GetWorld()->GetFirstPlayerController());
		cinematicManager->OnCinematicFinishedEvent().AddUniqueDynamic(this, &AT_TutoLogic::EndTuto);
		cinematicManager->PlayTutoOutroCinematic();
		refPlayer->HealthComponent()->SetCanTakeDamages(false);
		
		refBlock->DestroyObstacle();

		//refPlayer->SetPlayerInputs(false);
		//refPlayer->MovementComponent()->SetCanSwitchState(false);
		//refBlock->SetCanOpen(true);											//To open the door

		//SetCam();		

		//GetWorld()->GetTimerManager().SetTimer(_handle,this,&AT_TutoLogic::EndTuto,timeBeforeTutoEnd,false);
		
		break;
	default:
#if WITH_EDITOR
		LOG("T_TutoLogic::SwitchPhase failed to asses current phase enum")
#endif
	return;

		
		/*
	case WaitDelayFirstShoot :
		refPlayer->EnableInput(GetWorld()->GetFirstPlayerController());
		canBeginBossActivationTimer = true;
		break;
	case BossShootingDodge :
		refBoss->Shoot();
		tutoWidget->ClearTutoWidget();
		break;		
	case DashDodge :
		{
			tutoWidget->ClearTutoWidget();
			tutoWidget->InitializeTutoWidget(dashWidget);
			refPlayer->MovementComponent()->OnDashed().AddUniqueDynamic(this,&AT_TutoLogic::DashedToDodge);
			refPlayer->GetDetectionSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		}
	case PlayerFirstShoot :
		tutoWidget->ClearTutoWidget();
		tutoWidget->InitializeTutoWidget(shotOnBossWidget);
		refBoss->HealthSystemComponent()->OnTakeDamage()->AddUniqueDynamic(this,&AT_TutoLogic::HitFirstShot);
		break;		
	case ShowingSlowMow :
		tutoWidget->ClearTutoWidget();
		FuckLesSwitch(WIDGET_TUTO,showSlowMowDuration,BossShootingSlowMow);
		break;

	case BossShootingSlowMow :
		tutoWidget->ClearTutoWidget();
		FuckLesSwitch(WIDGET_TUTO,refBoss->GetBossShootDelay(),SlowMoDodge);
		//refPlayer->GetDetectionSphere()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		//refPlayer->GetDetectionSphere()->ModifyRadius(newRadiusForDetectionSphere);
		
		break;
		
	case SlowMoDodge:
		refBoss->Shoot();
		tutoWidget->InitializeTutoWidget(slowMowWidget);
		//refPlayer->GetDetectionSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case PlayerShootLoadHarpoon :
		tutoWidget->ClearTutoWidget();
		tutoWidget->InitializeTutoWidget(shotOnBossWidget);
		refBoss->HealthSystemComponent()->OnTakeDamage()->AddUniqueDynamic(this,&AT_TutoLogic::HitHarpoonSpawn);
		break;
	case HarpoonOnGround :
		tutoWidget->ClearTutoWidget();
		tutoWidget->InitializeTutoWidget(harpoonWidget);
		if(harpoonPowerOrb)
		{
			FVector _spawnLocation = refPlayer->GetActorLocation() + (refPlayer->GetActorForwardVector() * orbForwardSpawnOffset);
			_spawnLocation.Z = refBoss->GetActorLocation().Z + 80.0f;
			AC_HarpoonPowerOrb* _myHarpoonOrb = GetWorld()->SpawnActor<AC_HarpoonPowerOrb>(harpoonPowerOrb,
																							_spawnLocation,
																							refPlayer->GetActorRotation());
			
			if(!_myHarpoonOrb)return;
			_myHarpoonOrb->SetHaveALifeSpan(false);
			refPlayer->GetCharacterAttackComponent()->FOnPowerOrbCollected().AddUniqueDynamic(this,&AT_TutoLogic::CollectedHarpoonOrb);
		}
		break;		
	case ShootUntilHarpoonStun :
		tutoWidget->ClearTutoWidget();
		tutoWidget->InitializeTutoWidget(shotOnBossHarpoonWidget);
		refBoss->HealthSystemComponent()->OnTakeDamage()->AddUniqueDynamic(this,&AT_TutoLogic::HitHarpoonStun);
		break;		
	case PlayerShootLoadSelfGuided :
		tutoWidget->ClearTutoWidget();
		refPlayer->GetCharacterAttackComponent()->ResetShootState();
		tutoWidget->InitializeTutoWidget(shotOnBossWidget);
		refBoss->HealthSystemComponent()->OnTakeDamage()->AddUniqueDynamic(this,&AT_TutoLogic::HitGuidedSpawn);
		break;		
	case SelfGuidedOnGround :
		tutoWidget->ClearTutoWidget();
		tutoWidget->InitializeTutoWidget(doubleGuidedWidget);
		if(doubleGuidedowerOrb)
		{
			FVector _spawnLocation = refPlayer->GetActorLocation() + (refPlayer->GetActorForwardVector() * orbForwardSpawnOffset);
			_spawnLocation.Z = refBoss->GetActorLocation().Z + 80.0f;
			AC_ThornPowerOrb* _myDoubleGuidedOrb = GetWorld()->SpawnActor<AC_ThornPowerOrb>(doubleGuidedowerOrb,
																							_spawnLocation,
																							refPlayer->GetActorRotation());
			_myDoubleGuidedOrb->SetHaveALifeSpan(false);
			refPlayer->GetCharacterAttackComponent()->FOnPowerOrbCollected().AddUniqueDynamic(this,&AT_TutoLogic::CollectedGuidedOrb);
		 }
		break;
	case PlayerShootSelfGuided :
		tutoWidget->ClearTutoWidget();
		tutoWidget->InitializeTutoWidget(shotOnBossDoubleGuidedWidget);
		refBoss->HealthSystemComponent()->OnTakeDamage()->AddUniqueDynamic(this,&AT_TutoLogic::HitGuidedTrial);
		break;
		*/
	
	}
}

void AT_TutoLogic::Behaviour(EPhaseTuto _current,const float& _deltaTime)
{
	switch (_current)
	{
		//PHASE 1
		case NotStarted :
			break;
		case CinematicEnter :		
			break;
		case PlayerThreeShoot :
			currentCountElementWidget = Cast<UUI_TutoCountElementWidget>(tutoWidget->InitializeTutoWidget(iterationThreeShootCountWidget));
			if (currentCountElementWidget)
				currentCountElementWidget->UpdateCurrentCount(refPlayer->GetCharacterAttackComponent()->GetCurrentTutoIterationNumber());
			if(refPlayer->GetCharacterAttackComponent()->GetIsTutoConditionValid())
			{
				refPlayer->GetCharacterAttackComponent()->ResetTutoCondition();
				SwitchPhase(HarpoonOnGround);
			}
			break;
		case HarpoonOnGround :
			//iterationCountWidget->GetDefaultObject<UUI_TutoCountElementWidget>()->ResetCurrentCount();
			break;
		case ShootUntilHarpoonStun :
			break;
		case DashCancelShoot :
			//iterationCountWidget->GetDefaultObject<UUI_TutoCountElementWidget>()->UpdateCurrentCount(refPlayer->GetCharacterAttackComponent()->GetCurrentTutoIterationNumber());
			//tutoWidget->InitializeTutoWidget(iterationCountWidget);
			if(refPlayer->GetCharacterAttackComponent()->GetIsTutoConditionValid())
			{
				SwitchPhase(SelfGuidedOnGround);
				refPlayer->GetCharacterAttackComponent()->ResetTutoCondition();
			}
			break;
		case SelfGuidedOnGround :
			//iterationCountWidget->GetDefaultObject<UUI_TutoCountElementWidget>()->ResetCurrentCount();
			break;
		case PlayerShootSelfGuided :
			break;
			
		//PHASE 2
		case DashThroughProjectileWall :
			if(!boolBetWeenWallDashAndSlowMo && !refPlayer->GetCharacterAttackComponent()->GetIsTutoConditionValid())
				refBoss->LoadWallShoot(_deltaTime);
			//iterationCountWidget->GetDefaultObject<UUI_TutoCountElementWidget>()->UpdateCurrentCount(refPlayer->GetCharacterAttackComponent()->GetCurrentTutoIterationNumber());
			//tutoWidget->InitializeTutoWidget(iterationCountWidget);	
			if(refPlayer->GetCharacterAttackComponent()->GetIsTutoConditionValid())
			{
				boolBetWeenWallDashAndSlowMo = true;
				tutoWidget->ClearTutoWidget();
				refPlayer->GetCharacterAttackComponent()->ResetTutoCondition();

				currentCountElementWidget = Cast<UUI_TutoCountElementWidget>(tutoWidget->InitializeTutoWidget(iterationSlowMoCountWidget));
				currentCountElementWidget->ResetCurrentCount();
							
				FTimerHandle _checkTimer;
				FTimerDelegate _RespawnDelegate = FTimerDelegate::CreateUObject(this, &AT_TutoLogic::SwitchPhase, SlowMoDodgePattern);

				GetWorld()->GetTimerManager().SetTimer(_checkTimer, _RespawnDelegate, timeBetweenWallDashAndSlowMo, false);
				
				//SwitchPhase(SlowMoDodgePattern);
			}

			break;
		case SlowMoDodgePattern :
			//iterationCountWidget->GetDefaultObject<UUI_TutoCountElementWidget>()->ResetCurrentCount();
			boolBetWeenWallDashAndSlowMo = false;
			refBoss->LoadNormalShoot(_deltaTime);
			//iterationCountWidget->GetDefaultObject<UUI_TutoCountElementWidget>()->UpdateCurrentCount(refPlayer->GetCharacterAttackComponent()->GetCurrentTutoIterationNumber());
			//tutoWidget->InitializeTutoWidget(iterationCountWidget);
				if(refPlayer->GetCharacterAttackComponent()->GetIsTutoConditionValid())
				{
					refPlayer->GetCharacterAttackComponent()->ResetTutoCondition();
					SwitchPhase(BossFight);
				}
			break;

		//PHASE 3
		case BossFight :
			//iterationCountWidget->GetDefaultObject<UUI_TutoCountElementWidget>()->ResetCurrentCount();
			refBoss->LoadRandomShootPattern(_deltaTime);
			break;
		case Ended :
			break;
	default:
#if WITH_EDITOR
		LOG("T_TutoLogic::Behaviour failed to asses current phase enum")
#endif
	return;
		
		/*case WaitDelayFirstShoot :
			break;
		case BossShootingDodge :
			refBoss->LoadShoot(_deltaTime);
			break;
		case DashDodge :
			break;
		case PlayerFirstShoot :
			break;
		case ShowingSlowMow :
			break;
		case BossShootingSlowMow :
			
			break;
		case SlowMoDodge:
			refBoss->LoadShoot(_deltaTime);
			
			if (refPlayer->GetCharacterAttackComponent()->GetCurrentSlowMoOrbCount() < 3)
				SwitchPhase(PlayerShootLoadHarpoon);
			break;
		case PlayerShootLoadHarpoon :
			break;
		case HarpoonOnGround :		
			break;
		case ShootUntilHarpoonStun :
			refBoss->LoadShoot(_deltaTime);		
			break;
		case PlayerShootLoadSelfGuided :
			break;
		case SelfGuidedOnGround :
			break;
		case PlayerShootSelfGuided :
			break;
			*/

	}
}

void AT_TutoLogic::EndStartCinematic()
{
	cinematicManager->OnCinematicFinishedEvent().RemoveDynamic(this,&AT_TutoLogic::EndStartCinematic);
	SwitchPhase(PlayerThreeShoot);
}

void AT_TutoLogic::DashedToDodge()
{
	refPlayer->MovementComponent()->OnDashed().RemoveDynamic(this,&AT_TutoLogic::AT_TutoLogic::DashedToDodge);
	
	AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
	if(!_gm)return;
	_gm->GetWorldTimeManager()->SetWorldDefaultSpeed();
	
}

void AT_TutoLogic::CollectedHarpoonOrb()
{
	SwitchPhase(ShootUntilHarpoonStun);
	refPlayer->GetCharacterAttackComponent()->FOnPowerOrbCollected().RemoveDynamic(this,&AT_TutoLogic::CollectedHarpoonOrb);
}

void AT_TutoLogic::CollectedGuidedOrb()
{
	SwitchPhase(PlayerShootSelfGuided);	
	refPlayer->GetCharacterAttackComponent()->FOnPowerOrbCollected().RemoveDynamic(this,&AT_TutoLogic::CollectedGuidedOrb);
}

void AT_TutoLogic::HitFirstShot(float _dmg)
{
	refPlayer->GetCharacterAttackComponent()->IncreaseCurrentSlowMoOrbCount(slowMowCount);
	refPlayer->GetCharacterAttackComponent()->OnOrbCountChange().Broadcast(refPlayer->GetCharacterAttackComponent()->GetCurrentSlowMoOrbCount(),refPlayer->GetCharacterAttackComponent()->GetMaxOrbCount(),1);
	//SwitchPhase(ShowingSlowMow);
}

void AT_TutoLogic::HitHarpoonSpawn(float _dmg)
{
	hitsCount++;
	if (hitsCount == harpoonNeededHits)
	{
		hitsCount = 0;
		SwitchPhase(HarpoonOnGround);
		refBoss->HealthSystemComponent()->OnTakeDamage()->RemoveDynamic(this,&AT_TutoLogic::HitHarpoonSpawn);
	}
}

void AT_TutoLogic::HitShotBeforeHarpoon(float _dmg)
{
	refBoss->HealthSystemComponent()->OnTakeDamage()->RemoveDynamic(this,&AT_TutoLogic::HitShotBeforeHarpoon);
	SwitchPhase(HarpoonOnGround);
}

void AT_TutoLogic::HitHarpoonStun(float _dmg)
{

	hitsCount++;
	currentCountElementWidget->UpdateCurrentCount(hitsCount);

	if (hitsCount == harpoonStunNeededHits)
	{
		hitsCount = 0;
		SwitchPhase(DashCancelShoot);
		currentCountElementWidget->ResetCurrentCount();
		refBoss->GetTutoBossAnimInstance()->UpdateIsStunned(true);
		refBoss->OnTutoBossStun().Broadcast();

		if (refBoss->GetStunNiagaraSystem())
		{
			UNiagaraComponent* _niagComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				refBoss->GetStunNiagaraSystem(), refBoss->GetRootComponent(), NAME_None, FVector(0.f), FRotator(0.f),
				EAttachLocation::Type::KeepRelativeOffset, true);
			
			FTimerHandle _checkTimer;
			FTimerHandle _animTimer;
			FTimerDelegate _RespawnDelegate = FTimerDelegate::CreateUObject( this, &AT_TutoLogic::StopFX, _niagComp );
			GetWorld()->GetTimerManager().SetTimer(_checkTimer, refBoss->GetTutoBossAnimInstance(), &UT_BossTutoAnimInstance::ResetIsStunned, 2.0f);
			GetWorld()->GetTimerManager().SetTimer(_animTimer,_RespawnDelegate, 2.0f,false);
		}
		
		
		refBoss->HealthSystemComponent()->OnTakeDamage()->RemoveDynamic(this,&AT_TutoLogic::HitHarpoonStun);
	}
}

void AT_TutoLogic::HitGuidedSpawn(float _dmg)
{
	hitsCount++;
	//refBoss->GetTutoBossAnimInstance()->UpdateIsStunned(false);
	//currentCountElementWidget->UpdateCurrentCount(hitsCount);
	if (hitsCount == guidedSpawnNeededHits)
	{
		hitsCount = 0;
		SwitchPhase(SelfGuidedOnGround);
		currentCountElementWidget->ResetCurrentCount();
		refBoss->HealthSystemComponent()->OnTakeDamage()->RemoveDynamic(this,&AT_TutoLogic::HitGuidedSpawn);
	}
}

void AT_TutoLogic::HitGuidedTrial(float _dmg)
{

	if (refPlayer->GetCharacterAttackComponent()->GetIsAimingAtEnemy())
	{

		currentCountElementWidget->ResetCurrentCount();
		hitsCount = 0;
		return;
	}

	if(!refPlayer->GetCharacterAttackComponent()->GetIsAimingAtEnemy())
	{
		
		hitsCount++;
		currentCountElementWidget->UpdateCurrentCount(hitsCount);
	
	}
	
	if (hitsCount == guidedTrialNeededHits)
	{
		hitsCount = 0;
		SwitchPhase(DashThroughProjectileWall);
		currentCountElementWidget->ResetCurrentCount();
		refBoss->HealthSystemComponent()->OnTakeDamage()->RemoveDynamic(this,&AT_TutoLogic::HitGuidedTrial);
	}
}


void AT_TutoLogic::FuckLesSwitch(TEnumAsByte<EWidgetEnum> _widget,float _delay,TEnumAsByte<EPhaseTuto> _phase)
{
	tutoWidget->ClearTutoWidget();
	hud->ShowGameWidget(_widget);
	if (_delay == 0.0f)
	{
		SwitchPhase(_phase);
		return;
	}
	FTimerHandle _handleSlowMow;
	FTimerDelegate _delegateSlowMow;
	_delegateSlowMow.BindUFunction(this,"SwitchPhase",_phase);
	GetWorld()->GetTimerManager().SetTimer(_handleSlowMow,_delegateSlowMow,_delay,false);
}

void AT_TutoLogic::EndTuto()
{
	AIU_Camera* _cam = GetWorld()->GetAuthGameMode<AM_GameMode>()->GetCameraManager()->GetCamera();

	//_cam->SetUseCinematicAngles(false);
	currentPhaseNumber = 0;
	tutoWidget->ClearTutoWidget();
	refPlayer->GetCharacterAttackComponent()->SetIsInTuto(false);
	if(refBlock)
		refBlock->DestroyObstacle();
	hud->HideGameWidget(WIDGET_TUTO);

	refPlayer->HealthComponent()->SetCanTakeDamages(true);
	//refPlayer->SetPlayerInputs(true);
	//refPlayer->MovementComponent()->SetCanSwitchState(true);

	refPlayer->EnableInput(GetWorld()->GetFirstPlayerController());

	if(refBoss)
	refBoss->Destroy();
}

void AT_TutoLogic::SwitchPhaseToEndTuto()
{
	SwitchPhase(Ended);
}

void AT_TutoLogic::DestroyTutoBossProjectiles()
{
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if (AM_ProjectileManager* _projectileManager = _gameMode->GetProjectileManager())
		{
			_projectileManager->ReturnAllProjectile();
		}
	}


}

void AT_TutoLogic::ResetPhaseIfTutoDeath()
{
	
	refPlayer->GetCharacterAttackComponent()->ResetConditionIfTutoDeath();
	currentCountElementWidget->ResetCurrentCount();
	refBoss->ResetCurrentTutoIteration();

	if (currentPhaseNumber == 1)
		SwitchPhase(PlayerThreeShoot);
	if (currentPhaseNumber == 2)
		SwitchPhase(DashThroughProjectileWall);
	if (currentPhaseNumber == 3)
		SwitchPhase(BossFight);
}

void AT_TutoLogic::StopFX(UNiagaraComponent* niagaraComp)
{
	if(niagaraComp)
		niagaraComp->Deactivate();
}

void AT_TutoLogic::SetCam()
{
	const FVector& _exitLocation = refBlock->GetExitLocation();
	const FVector& _playerLocation = refPlayer->GetActorLocation();
	const float _angle = FCustomMath::AngleBetweenVectors(FVector::ForwardVector,_exitLocation - _playerLocation);
	if (!cameraRef)cameraRef = GetWorld()->GetAuthGameMode<AM_GameMode>()->GetCameraManager()->GetCamera();
	if (_exitLocation.Y > _playerLocation.Y)
		cameraRef->SetCinematicAngles(20.0f, 180.0f + FMath::RadiansToDegrees(_angle) );
	else
		cameraRef->SetCinematicAngles(20.0f, 180.0f - FMath::RadiansToDegrees(_angle) );
	cameraRef->SetUseCinematicAngles(true);
}

void AT_TutoLogic::StartTuto()
{
	hud->ShowGameWidget(WIDGET_TUTO);
	bStartedTuto = true;
	SwitchPhase(CinematicEnter);
}

void AT_TutoLogic::RegisterSphere(UT_ProjectileDetectionSphere* _sphere)
{
	_sphere->OnComponentBeginOverlap.AddDynamic(this,&AT_TutoLogic::SphereOverlap);
}

void AT_TutoLogic::SphereOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep,
	const FHitResult& _sweepResult)
{
	if (AP_Projectile* _projectile = Cast<AP_Projectile>(_otherActor))
	{
#if WITH_EDITOR
		if (!hud)
		{
			LOG("AT_TutoLogic::SphereOverlap HUD invalid");
			return;
		}
#endif

		// if(currentPhase == BossShootingDodge)
		// {
		// 	SwitchPhase(DashDodge);
		// 	gameMode->GetWorldTimeManager()->SetWorldInSlowMotion();
		// 	
		// }
		// if(currentPhase == BossShootingSlowMow)
		// {
		// 	SwitchPhase(SlowMoDodge);
		// 	
		// }
		
	}
}


