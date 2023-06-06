
#include "UC_PlayerAttackComponent.h"

#include "B_Boss.h"
#include "A_ArenaSlice.h"
#include "B_BossCritPoint.h"
#include "M_GameMode.h"
#include "IU_Player.h"
#include "IU_PlayerArrow.h"
#include "IU_PlayerMovementComponent.h"
#include "NiagaraComponent.h"
#include "P_Projectile.h"
#include "C_ThornPowerOrb.h"
#include "C_HarpoonPowerOrb.h"
#include "IU_Camera.h"
#include "IU_CameraManager.h"
#include "IU_Macros.h"
#include "IU_PunchingBall.h"
#include "T_BossTuto.h"
#include "T_TutoLogic.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UC_PlayerAttackComponent::UC_PlayerAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UC_PlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	fBaseLoadingTime = fLoadingTime;
	fBaseMaxDropRadius = fMaxDropRadius;
	fBaseMaxShootTime = fMaxShootTime;
	onPowerOrbCanSpawn.AddDynamic(this, &UC_PlayerAttackComponent::SpawnPowerOrb);

	FTimerHandle _timerEnemyDetection;
	GetWorld()->GetTimerManager().SetTimer(_timerEnemyDetection, this, &UC_PlayerAttackComponent::CheckEnemyDetection,
	                                       0.1f, true, FMath::RandRange(0.1f, 0.3f));
}

void UC_PlayerAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	IncreaseTimer();
	IncreaseShootTimer();
	IncreasePowerShootTimer();
}

void UC_PlayerAttackComponent::CheckEnemyDetection()
{
	if (!playerCamera || !arrowToSpawn.Get()) return;
	
	const FVector& _spawnPoint = GetSpawnPoint();
	const FVector& _maxDistancePoint = GetMaxDistancePoint(arrowToSpawn);
	
	FHitResult _hitResult;
	bool _enemy = UKismetSystemLibrary::LineTraceSingleForObjects(	GetWorld(), _spawnPoint, _maxDistancePoint, enemyLayer,
																	false, { }, EDrawDebugTrace::None, _hitResult,
																	true);
	
	if (_enemy)	// Check if there's nothing that can block the player attack
	{
		if (blockingLayers.Num() > 0)
		{
			FHitResult _blockResult;		
			_enemy = !UKismetSystemLibrary::LineTraceSingleForObjects(	GetWorld(), _spawnPoint, _hitResult.Location, blockingLayers,
																		false, { }, EDrawDebugTrace::None, _blockResult,
																		true);
		}
	}
	
	bIsAimingAtEnemy = _enemy;
	//DrawDebugSphere(GetWorld(), _spawnPoint, _maxArrowDistance, 200, FColor::Yellow, false, 0.1f);
	//DrawDebugSphere(GetWorld(), _spawnPoint, 10, 15, FColor::Yellow, false, 0.1f);
	
	//DrawDebugLine(GetWorld(), _spawnPoint, _direction, FColor::Blue, false, 0.1f);
	if (_enemy)
	{
		const AIU_PlayerArrow* _arrow = arrowToSpawn.GetDefaultObject();
		const float _arrowSize = _arrow->GetBoxComponent()->GetScaledBoxExtent().X;
		const float _maxArrowDistance = _arrow->GetArrowRange() + _arrowSize;
		_enemy = FMath::Abs(FVector::Dist(_spawnPoint, _hitResult.ImpactPoint)) < _maxArrowDistance;
		
		//DrawDebugSphere(GetWorld(), _hitResult.ImpactPoint, 10, 15, _enemy ? FColor::Green : FColor::Red, false, 0.1f);
		//if (_enemy)
			//DrawDebugLine(GetWorld(), _spawnPoint, _hitResult.ImpactPoint, FColor::Green, false, 0.1f);
	}
	
	onEnemyDetection.Broadcast(_enemy);
}

FVector UC_PlayerAttackComponent::GetSpawnPoint() const
{
	const AActor* _owner = GetOwner();
	return _owner->GetActorLocation() + playerCamera->GetActorForwardVector() * 100 + _owner->GetActorUpVector() * 20;
}

FVector UC_PlayerAttackComponent::GetMaxDistancePoint(const TSubclassOf<AIU_PlayerArrow>& _arrowType) const
{
	if (_arrowType.Get() && playerCamera)
	{
		const AIU_PlayerArrow* _arrow = _arrowType.GetDefaultObject();
		const float _arrowSize = _arrow->GetBoxComponent()->GetScaledBoxExtent().X;
		const float _maxArrowDistance = _arrow->GetArrowRange() + _arrowSize;
		return playerCamera->GetActorLocation() + playerCamera->GetActorForwardVector() * (_maxArrowDistance + 1000);
	}
	return FVector::ZeroVector;
}

AIU_PlayerArrow* UC_PlayerAttackComponent::SpawnArrow(const TSubclassOf<AIU_PlayerArrow>& _arrowType) const
{
	AIU_PlayerArrow* _arrow = nullptr;
	if (_arrowType.Get())
	{
		const FVector& _location = GetSpawnPoint();
		const FVector& _maxDistancePoint = GetMaxDistancePoint(_arrowType);
		const FRotator& _rotation = UKismetMathLibrary::FindLookAtRotation(_location, _maxDistancePoint);
		_arrow = GetWorld()->SpawnActor<AIU_PlayerArrow>(
			arrowToSpawn,
			_location,
			_rotation);
	}
	
	return _arrow;
}

void UC_PlayerAttackComponent::Shoot()
{
	if (!bCanShoot || !arrowToSpawn.Get() || !bShootInput)return;

	bHasPlayerCompletelyChargedShot = false;

	AActor* _owner = GetOwner();
	if (!_owner)return;

	if (!playerCamera)return;
	
	AIU_PlayerArrow* _myArrow = SpawnArrow(arrowToSpawn);

	if (shootActivationNiagaraSys)
	{
		UNiagaraComponent* _niagComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			shootActivationNiagaraSys, _myArrow->GetRootComponent(), NAME_None, FVector(0.f), FRotator(0.f),
			EAttachLocation::Type::KeepRelativeOffset, true);
	}

	if (!_myArrow)return;
	_myArrow->SetDamage(fBaseDamage * (fCurrentMultiplierTimer / fMaxShootTime));
	_myArrow->SetPrimaryShootingState(true);
	_myArrow->SetArrowState(Simple);
	_myArrow->SetArrowFX();

	AIU_Player* _player = Cast<AIU_Player>(_owner);
	if (_player)
		_myArrow->SetCurrentDifficulty(_player->GetDifficulty());


	onPrimaryShoot.Broadcast();

	const TArray<AActor*> _actorsToIgnore;
	FHitResult _outHit;

	bool _Isarena = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _owner->GetActorLocation(),
	                                                                _owner->GetActorLocation() - _owner->
	                                                                GetActorUpVector() * 1000,
	                                                                groundLayer, false, _actorsToIgnore,
	                                                                EDrawDebugTrace::None, _outHit, true);


	if (Cast<AA_ArenaSlice>(_outHit.GetActor()))
		_myArrow->SetIsInArena(true);

	if (fCurrentMultiplierTimer >= fMaxShootTime * fPercentNeededToDropEnhancementOrb && (Cast<AA_ArenaSlice>(
		_outHit.GetActor()) || GetIsInTuto()))
	{
		_myArrow->SetCanSpawnPowerOrb(true);
	}

	bCanShoot = false;
	
	
	if (fCurrentMultiplierTimer >= fMaxShootTime)
	{
		onPlayerShootFullyCharged.Broadcast();
		if(isInTuto)
		{
			AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
			if(!_gm)return;
			
			AM_GameLogic* _gl = _gm->GetGameLogic();
			if(!_gl)return;
			
			if(_gl->GetTutoLogic() && _gl->GetTutoLogic()->GetCurrentPhase() == PlayerThreeShoot && bIsAimingAtEnemy)
			{
				fCurrentTutoIteration++;
				_gl->GetTutoLogic()->GetCurrentCountElementWidget()->UpdateCurrentCount(fCurrentTutoIteration);
				if(fCurrentTutoIteration >= 3)
				{
					bIsTutoConditionValid = true;
					_gl->GetTutoLogic()->GetCurrentCountElementWidget()->ResetCurrentCount();
				}
				
			}
		}
		
	}

	else
	{
		AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
		if (!_gm)return;

		AM_GameLogic* _gl = _gm->GetGameLogic();
		if (!_gl)return;

		if (_gl->GetTutoLogic() && _gl->GetTutoLogic()->GetCurrentPhase() == PlayerThreeShoot)
		{
			fCurrentTutoIteration = 0;								//FOR TUTO
			bIsTutoConditionValid = false;							//FOR TUTO
		}

	}

	fCurrentTimer = 0;
}

void UC_PlayerAttackComponent::ShootHarpoon()
{
	if (!isInHarpoonShootState)return;

	if (!bCanShoot)return;

	AActor* _owner = GetOwner();
	if (!_owner)return;

	if (!playerCamera)return;

	AIU_Player* _player = Cast<AIU_Player>(_owner);
	
	AIU_PlayerArrow* _myArrow2 = SpawnArrow(harpoonToSpawn);

	if (!_myArrow2)return;
	_myArrow2->SetDamage(fHarpoonDamage * (fCurrentMultiplierTimer / fMaxShootTime));
	_myArrow2->SetArrowState(Harpoon);
	_myArrow2->SetArrowFX();
	onFXHarpoonShoot.Broadcast();

	if (bIsChargeSuperiorToPercent)
	{
		_myArrow2->SetCanStun(true);
		_myArrow2->SetHarpoonProperty(iNumberOfHarpoonNeededToStunBoss, fStunDuration);
	}

	if (fCurrentMultiplierTimer >= fMaxShootTime * fPercentNeededToDropEnhancementOrb)
		_myArrow2->SetCanSpawnPowerOrb(true);

	if (_player)
		_myArrow2->SetCurrentDifficulty(_player->GetDifficulty());

	bCanShoot = false;
	bIsChargeSuperiorToPercent = false;
	fCurrentTimer = 0;

	AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
	if (!_gm)return;

	if (isInTuto && _gm->GetTutoLogic()->GetCurrentPhase() == ShootUntilHarpoonStun)
	{
		if (!bIsAimingAtEnemy)
		{
			_gm->GetTutoLogic()->ResetTutoCondition();
			return;
		}
	}
}

void UC_PlayerAttackComponent::ShootDoubleGuided()
{
	if (!isInThornRainState)return;

	if (!bCanShoot)return;

	AActor* _owner = GetOwner();
	if (!_owner)return;

	if (!playerCamera)return;

	AActor* _target = nullptr;
	
	const FVector _cameraLocation = playerCamera->GetActorLocation(), _cameraForward = playerCamera->
		              GetActorForwardVector();
	FHitResult _enemyResult;

	// const bool _enemyHit = UKismetSystemLibrary::LineTraceSingleForObjects(
	// 	GetWorld(), _cameraLocation, _cameraLocation + _cameraForward * fMaxDoubleGuidedEnemyDetectionDistance,
	// 	enemyLayer, false, TArray<AActor*>(),
	// 	EDrawDebugTrace::None, _enemyResult, true);
	
	AIU_PlayerArrow* _myArrow3 = SpawnArrow(thornToSpawn);

	if (!_myArrow3)return;
	fFirstGuidedArrowDamage = fDoubleGuidedDamage * (fCurrentMultiplierTimer / fMaxShootTime);
	_myArrow3->SetDamage(fFirstGuidedArrowDamage);
	_myArrow3->SetArrowState(DoubleGuided);
	_myArrow3->SetRotationSpeedAndTime(fArrowRotationSpeed, fTimeBeforeArrowRotationStop);
	_myArrow3->SetArrowFX();

	AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
	if (!_gm)return;

	AB_Boss* _boss = _gm->GetBoss();
	if (!_boss)return;

	// if (_enemyHit)
	// {
	// 	_target = _enemyResult.GetActor();
	// 	_myArrow3->SetCurrentTarget(_target);
	// }
	//else
	//{
	//_target = nullptr;
	//}

	//_target = _gm->GetBoss();

	if (!isInTuto)
		_target = GetClosestCritPoint(_boss);
	

	else if(isInTuto)
	{
		_target = _gm->GetTutoLogic()->GetTutoBoss();
		_myArrow3->SetIsInTuto(true);
	}

	if(_target)
		_myArrow3->SetCurrentTarget(_target);

	AIU_Player* _player = Cast<AIU_Player>(_owner);
	if (_player)
		_myArrow3->SetCurrentDifficulty(_player->GetDifficulty());

	onFXThornShoot.Broadcast();


	if (bIsChargeSuperiorToPercent)
	{
		//rien pour l'instant
	}

	if (fCurrentMultiplierTimer > fMaxShootTime * 0.5 && fCurrentMultiplierTimer < fMaxShootTime * 0.9)
	{
		FTimerDelegate _delegate;
		_delegate.BindUFunction(this, "SpawnDoubleGuidedSecondArrow", false, _target,isInTuto);
		GetWorld()->GetTimerManager().SetTimer(doubleGuidedSecondArrowSpawnTimer, _delegate,
		                                       fDelayBetweenArrows,
		                                       false, fDelayBetweenArrows);
	}

	if (fCurrentMultiplierTimer > fMaxShootTime * 0.9)
	{
		FTimerDelegate _delegate;
		_delegate.BindUFunction(this, "SpawnDoubleGuidedSecondArrow", true, _target,isInTuto);
		GetWorld()->GetTimerManager().SetTimer(doubleGuidedSecondArrowSpawnTimer,
		                                       _delegate, fDelayBetweenArrows,
		                                       false, fDelayBetweenArrows);
		_myArrow3->SetCanSpawnPowerOrb(true);
	}

	bCanShoot = false;
	bIsChargeSuperiorToPercent = false;
	fCurrentTimer = 0;
}

AActor* UC_PlayerAttackComponent::GetClosestCritPoint(AB_Boss* boss)
{
	const TArray<AB_BossCritPoint*>& _critPoints = boss->GetCritPoints();
	const float& _distToBoss = FVector::Dist(GetOwner()->GetActorLocation(), boss->GetActorLocation());
	const FVector& _shootLocation = playerCamera->GetActorLocation() + playerCamera->GetActorForwardVector() *
		_distToBoss;

	float _maxEffectValue = -BIG_FLOAT;

	//Search biggest Effect value
	const uint& _max = _critPoints.Num();
	for (uint i = 0; i < _max; ++i)
	{
		AB_BossCritPoint* _critPoint = _critPoints[i];
		if (!_critPoint)continue;

		const float& _currentEffectValue = _critPoint->GetEffectValue();

		if (_currentEffectValue > _maxEffectValue)
			_maxEffectValue = _currentEffectValue;
	}

	float _closestDistance = BIG_FLOAT;
	AB_BossCritPoint* _toReturn = nullptr;

	// Search the closest
	for (uint i = 0; i < _max; ++i)
	{
		AB_BossCritPoint* _critPoint = _critPoints[i];
		if (!_critPoint)continue;

		if (_critPoint->GetEffectValue() == _maxEffectValue)
		{
			const float& _dist = FVector::Dist(_shootLocation, _critPoint->GetActorLocation());

			if (_dist < _closestDistance)
			{
				_closestDistance = _dist;
				_toReturn = _critPoint;
			}
		}
	}

	return _toReturn;
}

void UC_PlayerAttackComponent::PlayHitFXOnPlayer()
{
	if (FXOnPlayerWhenHitSomething)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			FXOnPlayerWhenHitSomething, GetOwner()->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator(0.f),
			EAttachLocation::Type::KeepRelativeOffset, true);
	}
}

void UC_PlayerAttackComponent::IncreaseAndCheckTutoCondition()
{
	fCurrentTutoIteration++;
	if(fCurrentTutoIteration >= 3)
		bIsTutoConditionValid = true;
}

void UC_PlayerAttackComponent::OnOrbDestroyed(AActor* _orb)
{
	if (_orb)
	{
		_orb->OnDestroyed.RemoveDynamic(this, &UC_PlayerAttackComponent::OnOrbDestroyed);
	}

	if (!bOrbUsed)
		onPowerOrbNotUsed.Broadcast();
}


void UC_PlayerAttackComponent::SpawnDoubleGuidedSecondArrow(bool _canSpawnOrb, AActor* _target, bool _isInTuto)
{
	AActor* _owner = GetOwner();
	if (!_owner || !_target)return;

	if (!playerCamera)return;

	AIU_PlayerArrow* _myArrow4 = SpawnArrow(thornToSpawn);

	if (!_myArrow4)return;

	_myArrow4->SetDamage(fFirstGuidedArrowDamage);
	_myArrow4->SetArrowState(DoubleGuided);
	_myArrow4->SetCurrentTarget(_target);
	_myArrow4->SetRotationSpeedAndTime(fArrowRotationSpeed, fTimeBeforeArrowRotationStop);
	_myArrow4->SetArrowFX();
	_myArrow4->SetIsInTuto(_isInTuto);
	
	_myArrow4->SetCanSpawnPowerOrb(_canSpawnOrb);

	AIU_Player* _player = Cast<AIU_Player>(_owner);
	if (_player)
		_myArrow4->SetCurrentDifficulty(_player->GetDifficulty());

	onFXThornShoot.Broadcast();
}

void UC_PlayerAttackComponent::SetCurrentShootingState()
{
	StopAnimation();
	SetGoodAnimation();
	SetGoodRotation();



	if (fCurrentMultiplierTimer < fMaxShootTime * fMinimumPercentNeededToShoot)
		return;


	SetCurrentMaxShootTime();

	if (fCurrentMultiplierTimer > fMaxShootTime)
		fCurrentMultiplierTimer = fMaxShootTime;


	if (isInHarpoonShootState)
	{
		ShootHarpoon();
	}

	else if (isInThornRainState)
	{
		ShootDoubleGuided();
	}

	else if (!isInThornRainState && !isInHarpoonShootState)
	{
		Shoot();
	}

	bHAsShootedOnce = true;
}

void UC_PlayerAttackComponent::SpawnPowerOrb()
{
	AActor* _owner = GetOwner();
	if (!_owner)return;
	FVector _actorLoc = _owner->GetActorLocation();

	if (!playerCamera)return;

	bOrbUsed = false;
	AC_ThornPowerOrb* _myThornOrb = GetWorld()->SpawnActor<AC_ThornPowerOrb>(
		thornPowerOrb,
		_owner->GetActorLocation() + (_owner->GetActorForwardVector() * orbForwardSpawnOffset) + (playerCamera->
			GetActorRightVector() * orbSpawnOffset),
		_owner->GetActorRotation());

	_myThornOrb->OnDestroyed.AddUniqueDynamic(this, &UC_PlayerAttackComponent::OnOrbDestroyed);
	AC_HarpoonPowerOrb* _myHarpoonOrb = GetWorld()->SpawnActor<AC_HarpoonPowerOrb>(
		harpoonPowerOrb,
		_owner->GetActorLocation() + (_owner->GetActorForwardVector() * orbForwardSpawnOffset) + (playerCamera->
			GetActorRightVector() * -orbSpawnOffset),
		_owner->GetActorRotation());

	if (!_myThornOrb || !_myHarpoonOrb)return;

	_myThornOrb->SetTwin(_myHarpoonOrb);
	_myHarpoonOrb->SetTwin(_myThornOrb);

	onPowerOrbSpawn.Broadcast(_myThornOrb->GetActorLocation(), _myHarpoonOrb->GetActorLocation());

	const TArray<AActor*> _actorsToIgnore;
	FHitResult _outHit;
	FHitResult _outHit2;

	bool _hitThorn = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _myThornOrb->GetActorLocation(),
	                                                                 _myThornOrb->GetActorLocation() - FVector(
		                                                                 0, 0, 1000),
	                                                                 groundLayer, false, _actorsToIgnore,
	                                                                 EDrawDebugTrace::ForDuration, _outHit, true);

	bool _hitHarpoon = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _myHarpoonOrb->GetActorLocation(),
	                                                                   _myHarpoonOrb->GetActorLocation() - FVector(
		                                                                   0, 0, 1000),
	                                                                   groundLayer, false, _actorsToIgnore,
	                                                                   EDrawDebugTrace::ForDuration, _outHit2, true);


	if (!_hitThorn || !_hitHarpoon)
	{
		_myThornOrb->Destroy();
		_myHarpoonOrb->Destroy();
		return;
	}

	if (_hitThorn && _hitHarpoon)
	{
		_myThornOrb->SetActorLocation(_outHit.Location + FVector(0, 0, zSpawnOffset));
		_myHarpoonOrb->SetActorLocation(_outHit2.Location + FVector(0, 0, zSpawnOffset));
	}
}

void UC_PlayerAttackComponent::DestroySecondPowerOrb(AC_PowerOrb* _otherOrb)
{
	_otherOrb->Destroy();
}

void UC_PlayerAttackComponent::IncreasePowerShootTimer()
{
	if (!isInHarpoonShootState && !isInThornRainState)
		return;

	if(isInTuto)return;

	if (isInHarpoonShootState)
	{
		fHarpoonTimer += GetWorld()->DeltaTimeSeconds;

		if (fHarpoonTimer > fHarpoonShootDuration)
		{
			isInHarpoonShootState = false;
			onHarpoonShoot.Broadcast(false);
			fHarpoonTimer = 0;
			fMaxShootTime = fBaseMaxShootTime;
			onPrimaryShootChange.Broadcast(NORMAL);
		}

		onPrimaryShootAltTimer.Broadcast(fHarpoonTimer, fHarpoonShootDuration);
	}

	else if (isInThornRainState)
	{
		fDoubleGuidedTimer += GetWorld()->DeltaTimeSeconds;


		if (fDoubleGuidedTimer > fDoubleGuidedDuration)
		{
			isInThornRainState = false;
			onThornShoot.Broadcast(true);
			fDoubleGuidedTimer = 0;
			fMaxShootTime = fBaseMaxShootTime;
			onPrimaryShootChange.Broadcast(NORMAL);
		}

		onPrimaryShootAltTimer.Broadcast(fDoubleGuidedTimer, fDoubleGuidedDuration);
	}
}

bool UC_PlayerAttackComponent::GetValidSlowMoOrbCount(int _orbConsumed)
{
	if (iCurrentSlowMoOrbCount < _orbConsumed)
		return false;

	iCurrentSlowMoOrbCount -= _orbConsumed;
	onOrbCountChange.Broadcast(iCurrentSlowMoOrbCount, iMaxSlowMoOrbCount, 1);
	return true;
}

void UC_PlayerAttackComponent::IncreaseTimer()
{
	if (bCanShoot || bIsDead)
		return;

	fCurrentTimer += GetWorld()->DeltaTimeSeconds;

	if (fCurrentTimer > fLoadingTime)
	{
		fCurrentTimer = 0;
		bCanShoot = true;
		onHasShootedAndChargingShoot.Broadcast(true);
	}
}

void UC_PlayerAttackComponent::IncreaseShootTimer()
{
	if (!bIsPlayerChargingShoot || !bCanShoot)return;


	if (!bHasShootFXAlreadySpawned)
	{
		bHasShootFXAlreadySpawned = true;
	}

	if (bHAsShootedOnce)
	{
		SetGoodFX();
		bHAsShootedOnce = false;
	}


	fCurrentMultiplierTimer += GetWorld()->DeltaTimeSeconds;

	if (fCurrentMultiplierTimer >= fMaxShootTime)
	{
		onPlayerShootFullyCharged.Broadcast();
		
	}

	if (fCurrentMultiplierTimer > fMaxShootTime + (fMaxShootTime * fAdditionnalPercentageTimeAfterMaxShootTime))
	{
		onHasShootedAndChargingShoot.Broadcast(false);

		StopAnimation();
		bIsChargeSuperiorToPercent = true;

		SetCurrentShootingState();
		bHasShootFXAlreadySpawned = false;
		fCurrentMultiplierTimer = 0;
	}

	onPrimaryShootTimer.Broadcast(fCurrentMultiplierTimer, fMaxShootTime);
}

void UC_PlayerAttackComponent::CheckHoldingTime()
{
	if(bIsOpeningChest)return;
	onPlayerChargingShoot.Broadcast(false);
	onPlayerStopShooting.Broadcast();
	onShooting.Broadcast(false);
	StopAnimation();


	bIsPlayerChargingShoot = false;


	//fCurrentTutoIteration = 0;								//FOR TUTO
	//bIsTutoConditionValid = false;							//FOR TUTO
	
	if (fCurrentMultiplierTimer >= fMaxShootTime * fPercentNeededToApplyPowerOrbEffect)
	{
		bIsChargeSuperiorToPercent = true;
	}

	if (!bHAsShootedOnce)
	{
		SetCurrentShootingState();
		
	}

	bHAsShootedOnce = false;
	fCurrentMultiplierTimer = 0;
	bHasShootFXAlreadySpawned = false;
	onPrimaryShootTimer.Broadcast(fCurrentMultiplierTimer, fMaxShootTime);
}

void UC_PlayerAttackComponent::StartShooting()
{
	if(bIsOpeningChest || !bShootInput)return;
	bIsPlayerChargingShoot = true;
	onPlayerChargingShoot.Broadcast(true);
	
	if (!bCanShoot)return;
	onShooting.Broadcast(true);
	SetGoodRotation();
	SetGoodAnimation();
	SetGoodFX();
}

void UC_PlayerAttackComponent::SetCurrentMaxShootTime()
{
	if (isInHarpoonShootState)
	{
		fMaxShootTime = fMaxHarpoonShootTime;
		return;
	}

	if (isInThornRainState)
	{
		fMaxShootTime = fMaxDoubleGuidedShootTime;
		return;
	}
	else
	{
		fMaxShootTime = fBaseMaxShootTime;
	}
}

void UC_PlayerAttackComponent::StopShooting()
{
	bIsShooting = false;
	bHAsShootedOnce = false;
}

void UC_PlayerAttackComponent::IncreasedCurrentSlowMoOrbCount()
{
	if (iCurrentSlowMoOrbCount >= iMaxSlowMoOrbCount)
		return;

	iCurrentSlowMoOrbCount++;
	onOrbCountChange.Broadcast(iCurrentSlowMoOrbCount, iMaxSlowMoOrbCount, 1);
}

void UC_PlayerAttackComponent::IncreasedCurrentShootingModeCount()
{
	if (isInHarpoonShootState || isInThornRainState)return;

	iCurrentShootingModeOrbCount++;

	onAlternativeShotCharging.Broadcast(iCurrentShootingModeOrbCount, iOrbAmountNeededToDropShootingModeOrb);

	if (iCurrentShootingModeOrbCount >= iOrbAmountNeededToDropShootingModeOrb)
	{
		iCurrentShootingModeOrbCount = 0;
		onPowerOrbCanSpawn.Broadcast();
		onAlternativeShotCharged.Broadcast();
	}
}

void UC_PlayerAttackComponent::ResetShoot()
{
	fCurrentTimer = 0;
	fCurrentMultiplierTimer = 0;
	bHAsShootedOnce = false;
	bCanShoot = true;

	onPrimaryShootTimer.Broadcast(fCurrentMultiplierTimer, fMaxShootTime);
}

void UC_PlayerAttackComponent::SetGoodRotation()
{
	if (stopRotationForDebug || !bShootInput)return;

	
	AActor* _owner = GetOwner();
	if (!_owner || !playerCamera) return;

	FRotator _rot = _owner->GetActorRotation();
	FRotator _newRot = FRotator(_rot.Pitch, playerCamera->GetActorRotation().Yaw, _rot.Roll);

	_owner->SetActorRotation(_newRot);
}


void UC_PlayerAttackComponent::SetGoodFX()
{
	if (isInHarpoonShootState)
	{
		onStartHarpoonLoadingShoot.Broadcast();
	}
	if (isInThornRainState)
	{
		onStartThornLoadingShoot.Broadcast();
	}
	if (!isInHarpoonShootState && !isInThornRainState)
	{
		onStartLoadingShoot.Broadcast();
	}
}

void UC_PlayerAttackComponent::SetGoodAnimation()
{
	if (isInHarpoonShootState)
	{
		onHarpoonShoot.Broadcast(true);
	}
	if (isInThornRainState)
	{
		onThornShoot.Broadcast(true);
	}
	if (!isInHarpoonShootState && !isInThornRainState)
	{
		onSimpleShoot.Broadcast(true);
	}
}

void UC_PlayerAttackComponent::StopAnimation()
{
	if (isInHarpoonShootState)
		onHarpoonShoot.Broadcast(false);

	if (isInThornRainState)
		onThornShoot.Broadcast(false);

	if (!isInHarpoonShootState && !isInThornRainState)
		onSimpleShoot.Broadcast(false);
}
