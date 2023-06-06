#include "T_BossTuto.h"

#include "C_SlowMotionComponent.h"
#include "IU_Macros.h"
#include "IU_PlayerMovementComponent.h"
#include "M_GameMode.h"
#include "P_PatternShoot.h"
#include "P_PoolingTriggerSphere.h"
#include "T_TutoLogic.h"
#include "UC_PlayerAttackComponent.h"
#include "Kismet/KismetMathLibrary.h"

class AP_PatternShoot;

AT_BossTuto::AT_BossTuto()
{
	PrimaryActorTick.bCanEverTick = true;

	billboardShootPoint = CreateDefaultSubobject<UBillboardComponent>("Billboard ShootPoint");
	billboardShootPoint->AttachToComponent(staticMesh, FAttachmentTransformRules::KeepRelativeTransform);

	shootPoint = CreateDefaultSubobject<USceneComponent>("Shoot Point");
	shootPoint->AttachToComponent(billboardShootPoint, FAttachmentTransformRules::KeepRelativeTransform);

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	skeletalMesh->SetupAttachment(RootComponent);
}

void AT_BossTuto::BeginPlay()
{
	Super::BeginPlay();
	SpawnPoolingTrigger();

	FTimerHandle _initTimer;
	world = GetWorld();
	world->GetTimerManager().SetTimer(_initTimer, this, &AT_BossTuto::Init, 1.0f, false);
}

void AT_BossTuto::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	RotateToPlayer();
}

void AT_BossTuto::SpawnPoolingTrigger()
{
	if (poolingTriggerType.Get())
	{
		poolingTrigger = GetWorld()->SpawnActor<AP_PoolingTriggerSphere>(
			poolingTriggerType, GetActorLocation(), GetActorRotation());
		poolingTrigger->SetTriggerSphereRadius(rangePulling);
	}
	else
		LOG("AT_BossTuto::SpawnPoolingTrigger => Pooling Trigger is not valid.")
}

void AT_BossTuto::Shoot()
{
	if (!patternDataTable)return;
	if (poolingTrigger) poolingTrigger->EnablePoolingTrigger();
	onShoot.Broadcast();
	CastPattern(patternDataTable);
	wait = true;
	currentPattern->OnComplete().AddDynamic(this, &AT_BossTuto::ChangeWait);
}

void AT_BossTuto::LoadShoot(const float& _deltaTime)
{
	if (wait)
		return;
	shootingTime += _deltaTime;
	if (shootingTime >= shootDelay)
	{
		Shoot();
		shootingTime -= shootDelay;
	}
}

void AT_BossTuto::LoadWallShoot(const float& _deltaTime)
{
	if (wait)
		return;
	shootingTime += _deltaTime;
	if (shootingTime >= shootDelay && canShoot)
	{

		 
		ShootSpecificPattern(patternWallDataTable);
		bossTutoAnimInstance->UpdateIsShootingWall(true);

		currentPlayerLife = tutoLogic->SendRefPlayerToTutoBlock()->HealthComponent()->GetHealth();
		
		UNiagaraComponent* _niagComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
			wallShootNiagaraSys,GetActorLocation() + FVector(1000,0,250), currentPattern->GetActorRotation());


		//currentPattern->GetActorLocation() + FVector(200, 0, 400)

		FTimerHandle _checkTimer;
		FTimerDelegate _RespawnDelegate = FTimerDelegate::CreateUObject( this, &AT_BossTuto::StopFX, _niagComp );
		GetWorld()->GetTimerManager().SetTimer(_checkTimer,_RespawnDelegate, 2.f,false);
	
		
		FTimerHandle _animTimer;
		GetWorld()->GetTimerManager().SetTimer(_animTimer, bossTutoAnimInstance, &UT_BossTutoAnimInstance::ResetIsShootingWall, 1.0f);
		
		shootingTime = 0;
		
	}
}

void AT_BossTuto::LoadNormalShoot(const float& _deltaTime)
{
	if (wait)
		return;
	shootingTime += _deltaTime;
	if (shootingTime >= shootDelay && canShoot)
	{
		ShootSpecificPattern(patternDataTable);
		bossTutoAnimInstance->UpdateIsShootingNormal(true);

		UNiagaraComponent* _niagComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				normalShootNiagaraSys, GetRootComponent(), NAME_None, FVector(0.f), FRotator(0.f),
				EAttachLocation::Type::KeepRelativeOffset, true);
	
		FTimerHandle _checkTimer;
		FTimerDelegate _RespawnDelegate = FTimerDelegate::CreateUObject( this, &AT_BossTuto::StopFX, _niagComp );
		GetWorld()->GetTimerManager().SetTimer(_checkTimer,_RespawnDelegate, 2.f,false);
		
		FTimerHandle _animTimer;
		GetWorld()->GetTimerManager().SetTimer(_animTimer, bossTutoAnimInstance, &UT_BossTutoAnimInstance::ResetIsShootingNormal, 1.0f);
		
		shootingTime = 0;


		if (player->GetSlowMotionComponent()->GetHasUsedSlowMo())
		{

			if (tutoLogic->GetCurrentPhase() == SlowMoDodgePattern && !tutoLogic->GetBoolBetWeenWallDashAndSlowMo())
			{
				player->GetSlowMotionComponent()->ResetHasUsedSlowMo();
			}
		}
	}
}

void AT_BossTuto::ShootSpecificPattern(UP_DT_BaseDataTable* patternToShoot)
{
	if (!patternDataTable)return;
	if (poolingTrigger) poolingTrigger->EnablePoolingTrigger();
	onShoot.Broadcast();
	CastPattern(patternToShoot);
	wait = true;
	currentPattern->OnComplete().AddDynamic(this, &AT_BossTuto::ChangeWait);
}

void AT_BossTuto::LoadShootSpecificPattern(const float& _deltaTime, UP_DT_BaseDataTable* patternToShoot)
{
	if (wait)
		return;
	shootingTime += _deltaTime;
	if (shootingTime >= shootDelay)
	{
		ShootSpecificPattern(patternToShoot);
		shootingTime -= shootDelay;
	}
}

void AT_BossTuto::LoadRandomShootPattern(const float& _deltaTime)
{
	if (wait)
		return;
	shootingTime += _deltaTime;
	if (shootingTime >= shootDelay)
	{
		int _random = UKismetMathLibrary::RandomIntegerInRange(0,1);
		
		if(_random == 0)
			ShootSpecificPattern(patternDataTable);
		if(_random == 1)
			ShootSpecificPattern(patternWallDataTable);
		shootingTime -= shootDelay;
	}
}

void AT_BossTuto::StopFX(UNiagaraComponent* niagaraComp)
{
	ROG_ERROR(!FNiagaraWorldManager::Get(GetWorld()), "AT_BossTuto::StopFX -> niagara world manager is nullptr");
	if(IsValid(niagaraComp))
		niagaraComp->DestroyInstance();
}

void AT_BossTuto::ChangeWait(AP_PatternBase* _pattern)
{
	_pattern->OnComplete().RemoveDynamic(this, &AT_BossTuto::ChangeWait);
	wait = !wait;
}

void AT_BossTuto::Init()
{

	HealthSystemComponent()->SetCanTakeDamages(false);

	if (!target)
		target = GetWorld()->GetAuthGameMode<AM_GameMode>()->GetPlayer();

	if (const AM_GameMode* _gameMode = world->GetAuthGameMode<AM_GameMode>())
	{
		_gameMode->GetTutoLogic()->SetTutoBoss(this);
		patternsTickManager = _gameMode->GetPatternsTickManager();
		feedbackManager = _gameMode->GetFeedbackManager();
		tutoLogic = _gameMode->GetTutoLogic();
	}

	if (skeletalMesh)
	{
		bossTutoAnimInstance = Cast<UT_BossTutoAnimInstance>(skeletalMesh->GetAnimInstance());
	}
	


#if WITH_EDITOR
	else
	{
		LOG("AT_BossTuto::Init -> Invalid gamemode");
	}
#endif
}

void AT_BossTuto::CastPattern(UP_DT_BaseDataTable* _dataTable)
{
	canShoot = false;

	if (!_dataTable)
	{
		LOG("AT_BossTuto::CastPattern -> Invalid dataTable");
		return;
	}
	const TSubclassOf<AP_PatternBase>& _patternType = _dataTable->GetPatternType();
	if (!_patternType)
	{
		LOG("AT_BossTuto::CastPattern -> Invalid pattern type");
		return;
	}


	if (tutoLogic->GetCurrentPhase() == DashThroughProjectileWall && !tutoLogic->GetBoolBetWeenWallDashAndSlowMo())
	{
		
		FTimerHandle _timer;
		GetWorld()->GetTimerManager().SetTimer(_timer, this, &AT_BossTuto::CheckPlayerLife, tutoLogic->GetTimerForDashThroughWallCheck());
	}

	if (tutoLogic->GetCurrentPhase() == SlowMoDodgePattern && !tutoLogic->GetBoolBetWeenWallDashAndSlowMo())
	{
		FTimerHandle _timerSlowMo;
		GetWorld()->GetTimerManager().SetTimer(_timerSlowMo, this, &AT_BossTuto::CheckForSlowMoUse, tutoLogic->GetTimerForSlowMoDodgeCheck());
	}



	FVector _location = FVector::Zero();

	if(_dataTable == patternDataTable)
		_location = GetActorLocation() + FVector(0, 0, fSelfGuidedOffset);
	else
		_location = GetActorLocation();

	const FRotator& _rotation = GetActorRotation();
	if (AP_PatternBase* _pattern = world->SpawnActor<AP_PatternBase>(_patternType, _location, _rotation))
	{
		_pattern->AttachToComponent(staticMesh, FAttachmentTransformRules::KeepWorldTransform);
		_pattern->SetPatternsTickManager(patternsTickManager);
		_pattern->SetFeedbackManager(feedbackManager);
		_pattern->OnComplete().AddDynamic(this, &AT_BossTuto::PatternBehaviorCompleted);
		_pattern->DelayedInitializePattern(_dataTable, nullptr);
		currentPattern = _pattern;

		const bool& _center = _dataTable->GetCenterDuplication();

		FTripleDeluxeFeedBack _feedback = FTripleDeluxeFeedBack();
		if (_center)
			DuplicatePatternCenter(_pattern, _location, _dataTable, FSpawnSettings(), &_feedback);
		else
			DuplicatePattern(_pattern, _location, _dataTable, FSpawnSettings(), &_feedback);
		
	}

	 
}

void AT_BossTuto::PatternBehaviorCompleted(AP_PatternBase* _pattern)
{
	_pattern->OnComplete().RemoveDynamic(this, &AT_BossTuto::PatternBehaviorCompleted);
	_pattern->Destroy();
	currentPattern = nullptr;

	GetWorld()->GetTimerManager().ClearTimer(timerClear);
	GetWorld()->GetTimerManager().SetTimer(timerClear, this, &AT_BossTuto::ProjectileClear, fWaitProjectileClear);
}

void AT_BossTuto::ProjectileClear()
{
	if (currentPattern) return;
	onProjectileClearTuto.Broadcast();
}

void AT_BossTuto::CheckPlayerLife()
{


	canShoot = true;

	if (player->HealthComponent()->GetHealth() < currentPlayerLife) return;


	if(player->MovementComponent()->GetHasDashed())
	{
		player->MovementComponent()->SetHasDashed(false);
		
		if (tutoLogic->GetCurrentPhase() == DashThroughProjectileWall  && !tutoLogic->GetBoolBetWeenWallDashAndSlowMo())
		{
			fCurrentConditionIteration++;
			tutoLogic->GetCurrentCountElementWidget()->UpdateCurrentCount(fCurrentConditionIteration);
			
			if (fCurrentConditionIteration >= 3)
			{
				player->GetCharacterAttackComponent()->SetIsTutoConditionValid(true);
				tutoLogic->GetCurrentCountElementWidget()->ResetCurrentCount();
				fCurrentConditionIteration = 0;
			
			}
		}
		
	}

}

void AT_BossTuto::CheckForSlowMoUse()
{
	canShoot = true;

	if (player->GetSlowMotionComponent()->GetHasUsedSlowMo())
	{

		if (tutoLogic->GetCurrentPhase() == SlowMoDodgePattern && !tutoLogic->GetBoolBetWeenWallDashAndSlowMo())
		{

			fCurrentConditionIteration++;
			tutoLogic->GetCurrentCountElementWidget()->UpdateCurrentCount(fCurrentConditionIteration);
			if (fCurrentConditionIteration >= 3)
			{
				tutoLogic->SwitchPhase(BossFight);
				tutoLogic->GetCurrentCountElementWidget()->ResetCurrentCount();
				fCurrentConditionIteration = 0;
				
			}
		}
	}

}

void AT_BossTuto::RotateToPlayer()
{
	if (!tutoLogic)return;

	if (tutoLogic->SendRefPlayerToTutoBlock())
	{
		FRotator _rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), tutoLogic->SendRefPlayerToTutoBlock()->GetActorLocation());

		FRotator _goodRot = FRotator(0, _rot.Yaw, 0);

		SetActorRotation(_goodRot);

	}

}

void AT_BossTuto::DuplicatePattern(const AP_PatternBase* _initialPattern, const FVector& _initialLocation, UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack)
{
	const int& _duplication = _dataTable->GetDuplication();
	if (_duplication <= 0) return;

	const TSubclassOf<AP_PatternBase>& _patternType = _dataTable->GetPatternType();
	const FVector& _fwd = _initialPattern->GetActorForwardVector();
	const FVector& _right = _initialPattern->GetActorRightVector();
	const FVector& _up = _initialPattern->GetActorUpVector();

	const FVector& _duplicateOffset = _dataTable->GetDuplicationOffset();
	const FVector& _offset = _fwd * _duplicateOffset.X + _right * _duplicateOffset.Y + _up * _duplicateOffset.Z;
	
	for (int i = 0; i < _duplication; i++)
	{
		const FVector& _offsetLocation = _offset * (i + 1);
		const FVector& _duplicationLocation = _initialLocation + _offsetLocation;

		const FRotator& _duplicationRotation = _initialPattern->GetActorRotation();
		SpawnPatternDuplicata(_patternType, _duplicationLocation, _duplicationRotation, _dataTable, _overridedFeedBack);
	}
}

void AT_BossTuto::DuplicatePatternCenter(AP_PatternBase* _initialPattern, const FVector& _initialLocation, UP_DT_BaseDataTable* _dataTable, const FSpawnSettings& _spawnSettings, const FTripleDeluxeFeedBack* _overridedFeedBack)
{
	const int& _duplication = _dataTable->GetDuplication();
	if (_duplication <= 0) return;
	
	const TSubclassOf<AP_PatternBase>& _patternType = _dataTable->GetPatternType();
	const FVector& _fwd = _initialPattern->GetActorForwardVector();
	const FVector& _right = _initialPattern->GetActorRightVector();
	const FVector& _up = _initialPattern->GetActorUpVector();

	const FVector& _duplicateOffset = _dataTable->GetDuplicationOffset();
	const FVector& _offset = _fwd * _duplicateOffset.X + _right * _duplicateOffset.Y + _up * _duplicateOffset.Z;
	
	for (int i = 0; i < _duplication / 2; i++)
	{
		const FVector& _offsetLocation = _offset * (i + 1);

		for (int inverse = 0; inverse <= 2; inverse++)
		{
			const FVector& _duplicationLocation = _initialLocation + _offsetLocation * (inverse ? 1 : -1);
			const FRotator& _duplicationRotation = _initialPattern->GetActorRotation();

			SpawnPatternDuplicata(_patternType, _duplicationLocation, _duplicationRotation, _dataTable, _overridedFeedBack);
		}
	}
}

AActor* AT_BossTuto::SpawnPatternDuplicata(const TSubclassOf<AP_PatternBase>& _patternType, const FVector& _location, const FRotator& _rotation, UP_DT_BaseDataTable* _dataTable, const FTripleDeluxeFeedBack* _overridedFeedBack)
{
	if (AP_PatternBase* _duplicatePattern = world->SpawnActor<AP_PatternBase>(_patternType, _location, _rotation))
	{
		_duplicatePattern->SetPatternsTickManager(patternsTickManager);
		_duplicatePattern->SetFeedbackManager(feedbackManager);
		//_duplicatePattern->OnComplete().AddDynamic(this, &UP_PatternCaster::DuplicatePatternCompleted);
		_duplicatePattern->DelayedInitializePattern(_dataTable, _overridedFeedBack, true, _dataTable->GetDuplicationFX());

		return _duplicatePattern;
	}
	return nullptr;
}
