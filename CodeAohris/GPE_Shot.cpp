#include "GPE_Shot.h"

#include "Components/BillboardComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "P_PoolingTriggerSphere.h"
#include "M_GameMode.h"
#include "IU_Player.h"

#include "IU_Macros.h"
#include "M_GameInstance.h"
#include "API_SteamAchievementsManager.h"
#include "P_PatternShoot.h"

#include "S_GameSave.h"

AGPE_Shot::AGPE_Shot()
{
	patternCaster = CreateDefaultSubobject<UGPE_PatternCasterComponent>("Pattern Caster");
	AddOwnedComponent(patternCaster);
	
	directionArrow = CreateDefaultSubobject<UArrowComponent>("Direction Arrow");
	directionArrow->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	
	billBoard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	billBoard->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	
	targetPoint = CreateDefaultSubobject<USceneComponent>("TargetPoint");
	targetPoint->AttachToComponent(billBoard,FAttachmentTransformRules::KeepRelativeTransform);
	
	niagaraAimLaser = CreateDefaultSubobject<UNiagaraComponent>("Aim Niagara");
	niagaraAimLaser->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	skeletalMesh->AttachToComponent(staticMesh,FAttachmentTransformRules::KeepRelativeTransform);

	billboardShootPoint = CreateDefaultSubobject<UBillboardComponent>("Billboard ShootPoint");
	billboardShootPoint->AttachToComponent(skeletalMesh,FAttachmentTransformRules::KeepRelativeTransform);
	
	shootPoint = CreateDefaultSubobject<USceneComponent>("Shoot Point");
	shootPoint->AttachToComponent(billboardShootPoint,FAttachmentTransformRules::KeepRelativeTransform);
}

void AGPE_Shot::EnableGPE()
{
	if (difficultyManager && difficultyManager->GetCurrentDifficulty() != EDifficultyType::DIFFICULTY_INITIATED)
	{
		Super::EnableGPE();

		if (UM_GameInstance* _gameInstance = GetWorld()->GetGameInstance<UM_GameInstance>())
		{
			if (!_gameInstance->GetCurrentGameSave())return;

			if(_gameInstance->GetCurrentGameSave()->GetGPEShotData().Contains(this->GetName()))
				_gameInstance->GetCurrentGameSave()->GetGPEShotData().Emplace(this->GetName(),false);
		}
	}
}
void AGPE_Shot::DisableGPE()
{
	if (AP_PatternBase* _currentPattern = patternCaster->CurrentPattern())
	{
		_currentPattern->OnComplete().RemoveDynamic(this,&AGPE_Shot::ChangeWait);
		patternCaster->PatternBehaviorCompleted(_currentPattern);
		wait = !wait;
	}
	if (poolingTrigger)
		poolingTrigger->BeginDisablePoolingTrigger();
	Super::DisableGPE();
}

void AGPE_Shot::BeginPlay()
{
	Super::BeginPlay();
	SpawnPoolingTrigger();

	FTimerHandle _initTimer;
	GetWorld()->GetTimerManager().SetTimer(_initTimer, this, &AGPE_Shot::Init, FMath::RandRange(0.5f, 1.0f), false);
	
	toIgnoreActors.Add(this);
	healthSystemComponent->OnDeath()->AddDynamic(this,&AGPE_Shot::TurretDeath);

	if (const UM_GameInstance* _gm = GetWorld()->GetGameInstance<UM_GameInstance>())
		difficultyManager = _gm->GetDifficultyManager();

	if (patternDataTable)
		patternDataTable->GetTripleDeluxeFeedBack().bIsGPEShotFeedBack = true;
}
void AGPE_Shot::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
#if WITH_EDITOR
	if (showDebug && !useBox)
		DrawDebugSphere(GetWorld(),ShootStartPosition(),range,32,FColor::Yellow,false,-1,0,5);
	else if(showDebug && useBox)
	{
		DrawDebugBox(GetWorld(),ShootStartPosition(),boxRange,GetActorRotation().Quaternion(),FColor::Yellow,false,-1,0,5.0f);		
	}
#endif
	
	if (IsPlayerInRange(target))
	{
		const FVector& _zOffset = FVector::UpVector *  zOffsetShoot;
		AimAt(shootAtPoint ? targetPoint->GetComponentLocation() + _zOffset : target->GetActorLocation() + _zOffset);			
		LoadShoot(DeltaSeconds);
		onActivate.Broadcast(true);
	}
	else
	{
		AimAt(targetPoint->GetComponentLocation() + FVector::UpVector *  zOffsetShoot);
		// if (AP_PatternBase* _pattern = patternCaster->CurrentPattern())
		// {
		// 		_pattern->Destroy();
		//		wait = false;
		// }
		if (poolingTrigger)
			poolingTrigger->BeginDisablePoolingTrigger();
		if (firstDelayFinished && waitDelayAfterFirst && shootingTime < shootDelay)
		{
			shootingTime += DeltaSeconds;
			if (shootingTime >= shootDelay)
			{
				firstDelayFinished = false;
				shootingTime -= shootDelay;
			}
		}
		onActivate.Broadcast(false);
	}
}
bool AGPE_Shot::ShouldTickIfViewportsOnly() const
{
	return showDebug;
}

void AGPE_Shot::SpawnPoolingTrigger()
{
	if (poolingTriggerType.Get())
	{
		poolingTrigger = GetWorld()->SpawnActor<AP_PoolingTriggerSphere>(poolingTriggerType, GetActorLocation(), GetActorRotation());
		poolingTrigger->SetTriggerSphereRadius(range + additionalRadiusForPulling);
	}
	else
		LOG("AGPE_Shot::SpawnPoolingTrigger => Pooling Trigger is not valid.")
}

void AGPE_Shot::Shoot()
{
	if (!patternDataTable)return;

	if (poolingTrigger) poolingTrigger->EnablePoolingTrigger();

	patternCaster->CastPattern(patternDataTable);
	wait = true;
	patternCaster->CurrentPattern()->OnComplete().AddDynamic(this,&AGPE_Shot::ChangeWait);
	if (AP_PatternShoot* _shootPattern = Cast<AP_PatternShoot>(patternCaster->CurrentPattern()))
	{
		_shootPattern->OnPatternShoot().AddUniqueDynamic(this, &AGPE_Shot::PatternShootOnShootEvent);
	}
}
void AGPE_Shot::LoadShoot(const float& _deltaTime)
{
	if (wait)
		return;	
	shootingTime+= _deltaTime;
	if (shootingTime>= firstShotDelay && !firstDelayFinished)
	{
		Shoot();
		firstDelayFinished = true;
		shootingTime -= firstShotDelay;
		return;		
	}
	if (!firstDelayFinished)
	{
		return;
	}
	if (shootingTime >= shootDelay)
	{
		Shoot();
		shootingTime -= shootDelay;
	}
}
bool AGPE_Shot::IsPlayerInRange(const AActor* _target) //TODO move to a timer instead of tick
{
	if (!_target )
	{
		if (!waitDelayAfterFirst)
			shootingTime = 0;
		return false;
	}
	
	const FVector& _turretLocation = ShootStartPosition();
	const FVector& _targetLocation =_target->GetActorLocation();	
	if (IsInRange(_targetLocation,_turretLocation,useBox))
	{
		if (wallLayer.Num() ==0)
			return true;
		FHitResult _result;
		const bool _traceHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
																		_turretLocation,
																		_targetLocation,
																		wallLayer,
																		true,
																		toIgnoreActors,
																		showDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
																		_result,
																		true,
																		FLinearColor::Blue,
																		FLinearColor::Red,
																		1.0f);
		if (_traceHit && !waitDelayAfterFirst)
			firstDelayFinished = false;
		return !_traceHit;
	}
	if (!waitDelayAfterFirst)
		firstDelayFinished = false;
	return false;
	
}
void AGPE_Shot::ChangeWait(AP_PatternBase* _pattern)
{
	_pattern->OnComplete().RemoveDynamic(this,&AGPE_Shot::ChangeWait);
	wait = !wait;
}

void AGPE_Shot::Init()
{
	if (!target)
	{
		if (const UWorld* _world = GetWorld())
			if (const AM_GameMode* _gameMode = _world->GetAuthGameMode<AM_GameMode>())
				target = _gameMode->GetPlayer();
	}

	if (gameSave)
	{
		gameSave->GetGPEShotData().Add(this->GetName(),false);
		
	}
}
void AGPE_Shot::AimAt(const FVector& _position) //TODO Lerp Rotation
{
	const FVector& _turretToPosition = _position - ShootStartPosition();
	const FRotator& _rotation = _turretToPosition.Rotation();
	staticMesh->SetWorldRotation(FRotator(0.0f,_rotation.Yaw,0.0f));
	directionArrow->SetWorldRotation(_rotation);
	niagaraAimLaser->SetWorldRotation(FRotator::ZeroRotator);
	niagaraAimLaser->SetVectorParameter("Beam End", _turretToPosition);
	
}
void AGPE_Shot::TurretDeath()
{
	if (gameSave)
	{
		if (gameSave->GetGPEShotData().Contains(this->GetName()))
			gameSave->GetGPEShotData().Emplace(this->GetName(), true);

		gameSave->CheckForTurretDeathSuccess(GetWorld());
	}

	DisableGPE();
	//Destroy();
}

void AGPE_Shot::PatternShootOnShootEvent()
{
	onShoot.Broadcast(true);	
}

bool AGPE_Shot::IsInRange(const FVector& _target, const FVector& _currentLocation, const bool& _useBox)
{
	if (!_useBox)return FVector::Dist(_currentLocation,_target)<range;
	const FVector& _turretToPlayer = _target  - _currentLocation;
	return (_turretToPlayer.ProjectOnTo(GetActorForwardVector()).Size() < (GetActorForwardVector() * boxRange.X).Size() &&
			_turretToPlayer.ProjectOnTo(GetActorRightVector()).Size() < (GetActorRightVector() * boxRange.Y).Size() &&
			_turretToPlayer.ProjectOnTo(GetActorUpVector()).Size() < (GetActorUpVector() * boxRange.Z).Size());
}
